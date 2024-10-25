#include <Servo.h> // Include the Servo library for controlling servo motors

// Define pin pairs for each "mole" servo and button. Each pair represents:
// {servo pin, button pin}
const int molePairs[4][2] = {
  {3, 2},  // Mole 0: Servo on pin 3, button on pin 2
  {5, 4},  // Mole 1: Servo on pin 5, button on pin 4
  {6, 7},  // Mole 2: Servo on pin 6, button on pin 7
  {9, 8}   // Mole 3: Servo on pin 9, button on pin 8
};

// Create an array of Servo objects, one for each mole
Servo moles[4];

// Pin for the piezo buzzer
const int Piezo = 10;

// Pin for the reset button
const int resetButton = 13;

// Game state variables
int starttime = 0;     // Stores the start time for the mole pop-up period
int elapsed = 0;       // Tracks elapsed time since mole popped up
bool gameActive = false; // Tracks if the game is active or not
int score = 0;         // Tracks the player's score

// Servo positions for mole movement
int upPosition = 0;    // Angle position for the "up" state
int downPosition = 90; // Angle position for the "down" state

// Function to move all moles to specified positions
// position[]: Array with position for each mole servo (up or down)
void moveMoles(int position[4]) {
  for (int i = 0; i < 4; i++) {
    moles[i].write(position[i]); // Set each mole's position
  }
}

// Function to check if a mole was hit by the player
// mole: The index of the mole to check
void checkMole(int mole) {
  // Check if the selected mole is in the "up" position
  if (moles[mole].read() == upPosition) {
    // If the mole's corresponding button is pressed
    if (digitalRead(molePairs[mole][1]) == HIGH) {
      // Flash all moles up momentarily for feedback
      int allMolesUp[4] = {upPosition, upPosition, upPosition, upPosition};
      moveMoles(allMolesUp);
      delay(250); // Keep moles up for 250 ms

      // Move all moles back to the down position
      int allMolesDown[4] = {downPosition, downPosition, downPosition, downPosition};
      moveMoles(allMolesDown);
      delay(150); // Pause briefly to show moles down

      // Play a tone to indicate a successful hit
      tone(Piezo, 250);  // 250 Hz tone for hit feedback
      delay(200);        // Play tone for 200 ms
      noTone(Piezo);     // Stop the tone

      // Increment the player's score and reset mole to "down" position
      score++;
      moles[mole].write(downPosition); // Lower the hit mole
    }
  }
}

// Setup function to initialize game components
void setup() {
  // Initialize each mole servo and its corresponding button
  for (int i = 0; i < 4; i++) {
    moles[i].attach(molePairs[i][0]);       // Attach servo to pin
    pinMode(molePairs[i][1], INPUT);        // Set button pin as input
    moles[i].write(downPosition);           // Set initial position to "down"
  }
  pinMode(Piezo, OUTPUT);                   // Set piezo pin as output
  pinMode(resetButton, INPUT_PULLUP);       // Set reset button with pull-up resistor
  randomSeed(analogRead(0));                // Seed random generator with analog input
}

// Main game loop
void loop() {
  // Check if reset button is pressed to start the game
  if (digitalRead(resetButton) == LOW) {
    gameActive = true;  // Set game as active
    score = 0;          // Reset score for a new game
  }

  // Run the game logic if the game is active and score is less than 4
  if (gameActive && score < 4) {
    // Choose a random mole to pop up
    int randomMole = random(0, 4);
    moles[randomMole].write(upPosition); // Move the chosen mole to "up" position

    // Track the time the mole stays up
    starttime = millis();  // Set start time for pop-up
    elapsed = 0;           // Reset elapsed time

    // Check for a hit within the allowed 1-second window
    while (elapsed <= 1000) {
      // Loop through all moles to check if any have been hit
      for (int i = 0; i < 4; i++) {
        if (moles[i].read() == upPosition) { // If mole is up
          checkMole(i);                      // Check if it's hit
        }
      }
      // Update elapsed time since mole popped up
      elapsed = millis() - starttime;
      delay(150); // Delay to limit update rate and reduce CPU usage
    }

    // After the time limit, set all moles to "down" position
    int allMolesDown[4] = {downPosition, downPosition, downPosition, downPosition};
    moveMoles(allMolesDown);
  }

  // Check if score threshold is met to end the game
  if (score >= 4) {
    gameActive = false; // End the game

    // Final animation to indicate game end
    for (int i = 0; i < 3; i++) {
      int allMolesUp[4] = {upPosition, upPosition, upPosition, upPosition};
      moveMoles(allMolesUp); // Raise all moles
      delay(300);            // Hold up position

      int allMolesDown[4] = {downPosition, downPosition, downPosition, downPosition};
      moveMoles(allMolesDown); // Lower all moles
      delay(300);              // Hold down position
    }
  }
}
