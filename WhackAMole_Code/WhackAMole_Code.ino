// Initial Code for Whack-A-Mole
// Written for EDD 111 2024
// Written By: Katherine O'Keefe
// Edited by: Koen Gieskes
// Edited further by: Aiden Rivera

using namespace std;

// molePairs is a 2d array that sorts the outputs and inputs of the game
// The format is {outputPin,inputPin}
const int molePairs[4][2] = {
  {2, 9},
  {3, 8},
  {4, 7},
  {5, 6}
};

// defining the piezo buzzer
const int Piezo = 10;

// defining time variables
int starttime = 0;
int elapsed = 0;

// defining the score
int score = 0;

// Global light states
bool lightsOn[4] = {1, 1, 1, 1};
bool lightsOff[4] = {0, 0, 0, 0};

// sets up the variables as inputs or outputs
void display_lights(bool lights[4]) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(molePairs[i][0], lights[i]);
  }
}

void checkMole(int mole) {
  if (digitalRead(molePairs[mole][0]) == HIGH) {
    // reads if the button corresponding to the red LED is ON
    // if redButton is ON proceed into if statement
    if (digitalRead(molePairs[mole][1]) == HIGH) {
      // shows that a point was gotten
      display_lights(lightsOn);
      delay(250); // in milliseconds
      
      display_lights(lightsOff);
      delay(150); // in milliseconds
      
      // turns the piezo button ON
      tone(Piezo, 250);
      
      delay(200); // in milliseconds
      
      // turns the piezo button OFF
      noTone(Piezo);
      
      // adds 1 point to your score
      score++;

      // turns OFF the LED light
      digitalWrite(molePairs[mole][0], LOW);
    }
  }
}

void setup() {
  // initialize the LED lights as OUTPUTS
  // initialize the corresponding buttons as INPUTS
  //(can also initialize as INPUT_PULLUP 
  // which sets the buttons to HIGH when unpressed)
  for (int i = 0; i < 4; i++) {
    pinMode(molePairs[i][0], OUTPUT);
    pinMode(molePairs[i][1], INPUT);
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
    // define randomColor as integer variable 
    int randomColor;
    
    // generate random # from 0 to 4 that matches LED Pins
    randomColor = random(0, 4);
    
    digitalWrite(molePairs[randomColor][0], HIGH);
    
    starttime = millis();
    elapsed = 0;

    while (elapsed <= 1000) {
      for (int i = 0; i < 4; i++) {
        if (digitalRead(molePairs[i][0]) == HIGH) {
          checkMole(i);
        }
      }
      elapsed = millis() - starttime;
      delay(150);
    }
    display_lights(lightsOff); // Turn off all moles
  }

  score = 0;
  // Shows game is over
  display_lights(lightsOn); // Turn on all lights to indicate game over
  delay(500);

  display_lights(lightsOff); // Turn off lights
  delay(400);

  display_lights(lightsOn); // Turn on lights
  delay(300);

  display_lights(lightsOff); // Turn off lights
  delay(200);

  display_lights(lightsOn); // Turn on lights
  delay(200);

  display_lights(lightsOff); // Turn off lights
  delay(100);

  display_lights(lightsOn); // Turn on lights
  delay(500);

  display_lights(lightsOff); // Final turn off
}
