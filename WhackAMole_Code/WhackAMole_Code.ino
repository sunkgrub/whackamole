#include <Servo.h> // Include the Servo library

// molePairs is a 2d array that sorts the servo and button pins
// The format is {servoPin, inputPin}
const int molePairs[4][2] = {
  {3, 2},   // {Servo for mole 1, Button for mole 1}
  {5, 4},   // {Servo for mole 2, Button for mole 2}
  {6, 7},   // {Servo for mole 3, Button for mole 3}
  {9, 8}    // {Servo for mole 4, Button for mole 4}
};

// Array to hold Servo objects for each mole
Servo moles[4];

// defining the piezo buzzer
const int Piezo = 10;

// defining time variables
int starttime = 0;
int elapsed = 0;

// defining the score
int score = 0;

// Global state of the moles (servo positions)
int upPosition = 0;   // The position for the servo when the mole "pops up"
int downPosition = 90; // The position for the servo when the mole is "down"

// Move the servos based on the mole state (up or down)
void moveMoles(int position[4]) {
  for (int i = 0; i < 4; i++) {
    moles[i].write(position[i]);
  }
}

void checkMole(int mole) {
  if (moles[mole].read() == upPosition) {
    // if the mole (servo) is in the "up" position
    if (digitalRead(molePairs[mole][1]) == HIGH) {
      // Button press is detected for this mole
      // Shows that a point was gotten
      int allMolesUp[4] = {upPosition, upPosition, upPosition, upPosition};
      moveMoles(allMolesUp);
      delay(250); // in milliseconds

      int allMolesDown[4] = {downPosition, downPosition, downPosition, downPosition};
      moveMoles(allMolesDown);
      delay(150); // in milliseconds

      // Play the buzzer sound
      tone(Piezo, 250);
      delay(200); // in milliseconds
      noTone(Piezo); // Stop the sound

      // Adds 1 point to your score
      score++;

      // Move the servo back to the "down" position
      moles[mole].write(downPosition);
    }
  }
}

void setup() {
  // Initialize the servos and buttons
  for (int i = 0; i < 4; i++) {
    moles[i].attach(molePairs[i][0]); // Attach the servo to the corresponding pin
    pinMode(molePairs[i][1], INPUT);  // Set the button pins as inputs
    moles[i].write(downPosition);     // Initialize the mole in the "down" position
  }

  // Initialize the piezo buzzer
  pinMode(Piezo, OUTPUT);

  // Initialize the random seed
  randomSeed(analogRead(0));
}

void loop() {
  // Start the game with a score of 0
  while (score < 4) {
    // Randomly select a mole to pop up
    int randomMole = random(0, 4);
    moles[randomMole].write(upPosition); // Pop up the mole (move the servo up)

    starttime = millis();
    elapsed = 0;

    while (elapsed <= 1000) {
      for (int i = 0; i < 4; i++) {
        if (moles[i].read() == upPosition) {
          checkMole(i); // Check if the mole has been hit
        }
      }
      elapsed = millis() - starttime;
      delay(150);
    }
    // Move all moles to the down position
    int allMolesDown[4] = {downPosition, downPosition, downPosition, downPosition};
    moveMoles(allMolesDown);
  }

  // Reset the game state when the score reaches 4
  score = 0;

  // Indicate the game is over by moving all moles up and down
  for (int i = 0; i < 3; i++) {
    int allMolesUp[4] = {upPosition, upPosition, upPosition, upPosition};
    moveMoles(allMolesUp);
    delay(300);

    int allMolesDown[4] = {downPosition, downPosition, downPosition, downPosition};
    moveMoles(allMolesDown);
    delay(300);
  }
}
