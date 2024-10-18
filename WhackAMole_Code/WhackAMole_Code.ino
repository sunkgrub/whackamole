// Initial Code for Whack-A-Mole
// Written for EDD 111 2024
// Written By: Katherine O'Keefe
// Edited by: Koen Gieskes
// Edited further by: Aiden Rivera

#include <Servo.h>
using namespace std;

// Declare individual servos
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// molePairs is a 2d array that sorts the outputs and inputs of the game
// The format is {outputPin,inputPin}
const int molePairs[4][2] = {
  {2, 9},
  {3, 8},
  {4, 7},
  {5, 6}
};

Servo* servos[4] = {&servo1,&servo2,&servo3,&servo4};

// defining the piezo buzzer
const int Piezo = 10;

// defining time variables
int starttime = 0;
int elapsed = 0;

// defining the score
int score = 0;

// Global light states
bool molesUp[4] = {1, 1, 1, 1};
bool molesDown[4] = {0, 0, 0, 0};

// sets up the variables as inputs or outputs
void displayMoles(bool moles[4]) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(molePairs[i][0], moles[i]);
  }
}

void checkMole(int mole) {
  if (servos[mole]->read() >= 179) {
    // reads if the button corresponding to the mole is up
    // if correct button is ON proceed into if statement
    if (digitalRead(molePairs[mole][1]) == HIGH) {
      // shows that a point was gotten
      displayMoles(molesUp);
      delay(250); // in milliseconds
      
      displayMoles(molesDown);
      delay(150); // in milliseconds
      
      // turns the piezo button ON
      tone(Piezo, 250);
      
      delay(200); // in milliseconds
      
      // turns the piezo button OFF
      noTone(Piezo);
      
      // adds 1 point to your score
      score++;

      // turns OFF the LED moles
      servos[mole]->write(0);
    }
  }
}

void setup() {
  // initialize the servos
  // initialize the corresponding buttons as INPUTS
  //(can also initialize as INPUT_PULLUP 
  // which sets the buttons to HIGH when unpressed)
  for (int i = 0; i < 4; i++) {
    pinMode(molePairs[i][1], INPUT);
  }

  for (int i = 1; i < 4; i++) {
    servos[i]->attach(molePairs[i][0]);
  }
  
  // initialize the random variable pick
  randomSeed(analogRead(0));
  
  // initialize the piezo buzzer
  pinMode(Piezo, OUTPUT);
}

// main function loop
void loop() {
  // starts the game with a score of 0
  // will go through this loop until score hits 4
  while (score < 4) {
    // define randomMole as integer variable 
    int randomMole;
    
    // generate random # from 0 to 4 that matches servo Pins
    randomMole = random(0, 4);
    
    servos[randomMole]->write(180);
    
    starttime = millis();
    elapsed = 0;

    while (elapsed <= 1000) {
      for (int i = 0; i < 4; i++) {
        if (servos[i]->read() >= 179) {
          checkMole(i);
        }
      }
      elapsed = millis() - starttime;
      delay(150);
    }
    displayMoles(molesDown); // Turn off all moles
  }

  score = 0;
  // Shows game is over
  displayMoles(molesUp); // Turn on all moles to indicate game over
  delay(500);

  displayMoles(molesDown); // Turn off moles
  delay(400);

  displayMoles(molesUp); // Turn on moles
  delay(300);

  displayMoles(molesDown); // Turn off moles
  delay(200);

  displayMoles(molesUp); // Turn on moles
  delay(200);

  displayMoles(molesDown); // Turn off moles
  delay(100);

  displayMoles(molesUp); // Turn on moles
  delay(500);

  displayMoles(molesDown); // Final turn off
}
