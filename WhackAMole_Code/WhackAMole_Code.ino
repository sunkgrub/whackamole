#include <Servo.h> // Include the Servo library for controlling servo motors

// Define pin pairs for each "mole" servo and button. Each pair represents:
// {servo pin, Vibration pin}
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
int starttime = 0;       // Stores the start time for the mole pop-up period
int elapsed = 0;         // Tracks elapsed time since mole popped up
bool gameActive = false; // Tracks if the game is active or not
int misses = 0;          // Tracks the player's missed moles
int moleTimeLimit = 1500; // Initial time limit to hit mole (ms)
int lastMole = -1;       // Keeps track of the last mole's index (initialized to -1)
bool danceTime = true;

// Servo positions for mole movement
int upPosition = 0;      // Angle position for the "up" state
int downPosition = 90;   // Angle position for the "down" state

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
      // Move the hit mole immediately back to the down position
      moles[mole].write(downPosition);
      starttime = millis(); // Reset start time for the next mole
    }
  }
}

// Setup function to initialize game components
void setup() {
  // Initialize each mole servo and its corresponding vibration sensor
  for (int i = 0; i < 4; i++) {
    moles[i].attach(molePairs[i][0]);       // Attach servo to pin
    pinMode(molePairs[i][1], INPUT);        // Set button pin as input
    moles[i].write(downPosition);           // Set initial position to "down"
  }
  pinMode(Piezo, OUTPUT);                   // Set piezo pin as output
  pinMode(resetButton, INPUT_PULLUP);       // Set reset button with pull-up resistor
  randomSeed(analogRead(0));                // Seed random generator with analog input
}

// Function to reset the game
void resetGame() {
  gameActive = true;      // Activate the game
  misses = 0;             // Reset misses
  moleTimeLimit = 1500;   // Reset the initial mole time limit
  int allMolesDown[4] = {downPosition, downPosition, downPosition, downPosition};
  moveMoles(allMolesDown); // Set all moles to the down position
  delay(200);              // Small delay to let everything reset
  lastMole = -1;           // Reset last mole index to ensure randomness on game start
  danceTime = true;
}

// Main game loop
void loop() {
  // Check if reset button is pressed to start or reset the game
  if (digitalRead(resetButton) == LOW) {
    resetGame(); // Call the resetGame function to reset the game state
  }

  // Run the game logic if the game is active and misses are less than 3
  if (gameActive && misses < 3) {
    // Choose a random mole, ensuring it is not the same as the last one
    int randomMole = random(0, 4);
    if (randomMole == lastMole) {
      randomMole = (randomMole + 1) % 4; // If same as last mole, increment and wrap around
    }
    lastMole = randomMole; // Update last mole

    // Move the chosen mole to the "up" position
    moles[randomMole].write(upPosition);

    // Track the time the mole stays up
    starttime = millis();  // Set start time for pop-up
    elapsed = 0;           // Reset elapsed time

    // Check for a hit within the allowed time window
    while (elapsed <= moleTimeLimit) {
      // Loop through all moles to check if any have been hit
      for (int i = 0; i < 4; i++) {
        if (moles[i].read() == upPosition) { // If mole is up
          checkMole(i);                      // Check if it's hit
        }
      }
      // Update elapsed time since mole popped up
      elapsed = millis() - starttime;
      delay(100); // Small delay to reduce CPU usage
    }

    // After the time limit, if the mole is still up, count it as a miss
    if (moles[randomMole].read() == upPosition) {
      misses++;
      moles[randomMole].write(downPosition); // Move the missed mole down

      // Play a "miss" tone
      tone(Piezo, 100);   // 100 Hz tone for miss feedback
      delay(200);         // Play tone for 200 ms
      noTone(Piezo);      // Stop the tone
    }

    // Gradually decrease mole time limit to make the game harder
    if (moleTimeLimit > 200) {
      moleTimeLimit -= 50; // Decrease mole time limit by 20 ms
    }
  }

  // Check if miss threshold is met to end the game
  if (misses >= 3) {
    gameActive = false; // End the game

    if (danceTime && !gameActive) {
    // Final animation to indicate game end
      for (int i = 0; i < 3; i++) {
        int allMolesUp[4] = {upPosition, upPosition, upPosition, upPosition};
        moveMoles(allMolesUp); // Raise all moles
        delay(300);            // Hold up position

        int allMolesDown[4] = {downPosition, downPosition, downPosition, downPosition};
        moveMoles(allMolesDown); // Lower all moles
        delay(300);              // Hold down position
      }
      danceTime = false;
    }
  }
}
