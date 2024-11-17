#include <Servo.h> // Include the Servo library for controlling servo motors
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>


//MISCELLANEOUS SETUP

const int MAX_PLAYERS = 10;
const int screenHeight = 2;

// Pin for the piezo buzzer
const int Piezo = 10;

// Pin for the reset button
const int resetButton = 13;

// Game state variables
bool danceTime = true;
char* current_username = "";
char currentMsg[7] = ""; // Global declaration
int starttime = 0;       // Stores the start time for the mole pop-up period
int elapsed = 0;         // Tracks elapsed time since mole popped up
bool gameActive = false; // Tracks if the game is active or not
int misses = 0;          // Tracks the player's missed moles
int score = 0;           // Tracks the players score
int moleTimeLimit = 1500; // Initial time limit to hit mole (ms)
int lastMole = -1;       // Keeps track of the last mole's index (initialized to -1)
struct Player {
public:
    char name[10];
    int score;
};

void insertPlayer(Player newPlayer);

// Function to reset the game

// Servo positions for mole movement
const int upPosition = 0;      // Angle position for the "up" state
const int downPosition = 90;   // Angle position for the "down" state




//LCD DISPLAY SETUP
LiquidCrystal_I2C lcd(0x27, 20, 4);


//SERVO SETUP

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
      score++;
    }
  }


}

//KEYPAD SETUP

bool typingActive = false;
const byte ROWS = 4;
const byte COLS = 4;

byte rowPins[ROWS] = {A0, A1, A2, A3};
byte colPins[COLS] = {13,12,11,10};

// Declare the global leaderboard array
Player leaderboard[MAX_PLAYERS] = {
    {"Alice", 500},
    {"Bob", 400},
    {"Charlie", 300},
    {"David", 200},
    {"Eve", 100}
};

const char* numDict[] = {
    "abc",  // Corresponds to 1
    "def",  // Corresponds to 2
    "ghi",  // Corresponds to 3
    "jkl",
    "mno",
    "pqrs", // Corresponds to 4
    "tuv",  // Corresponds to 5
    "wxyz"  // Corresponds to 6
};

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

char getLetterForKey(char number, int *currentIndex, char *lastKeyPressed) {
    int index = number - '1';

    if (index >= 0 && index < sizeof(numDict) / sizeof(numDict[0])) {
        if (number == *lastKeyPressed) {
            // Cycle through characters if the same key is pressed
            *currentIndex = (*currentIndex + 1) % strlen(numDict[index]);
        } else {
            // Reset the index for a new key press
            *currentIndex = 0;
            *lastKeyPressed = number;
        }
        return numDict[index][*currentIndex];
    }
    return '\0';
}

char* typing() {
    memset(currentMsg, 0, sizeof(currentMsg)); // Always clear at the start
    int currentIndex[6] = {0};       // Track the current index for each number
    char lastKeyPressed = '\0';      // Track the last key pressed
    bool lastWasHash = false;        // Flag to track if the previous key was '#'
    char currentChar;                // Current character to display
    int cursorIndex = 10;
    typingActive = true;

    lcd.clear();                     // Clear the LCD at the start
    lcd.print("Username: ");

    while (typingActive) {
        char currentKey = customKeypad.getKey(); // Get the current key

        if (currentKey) {  // Check if a key is pressed
            Serial.print("Key Pressed: ");
            Serial.println(currentKey); // Debugging output

            if (currentKey == '*') {
                // Exit typing mode
                memset(currentMsg, 0, sizeof(currentMsg)); // Clear currentMsg
                cursorIndex = 0;
                lcd.clear();  
                break; // Exit the typing loop
            }

            if (currentKey == '#') {
                if (lastWasHash) {
                    // Show the final message before exiting
                    lcd.clear();
                    lcd.setCursor(0, 0);  // Reset cursor to top left
                    lcd.print("Username: ");
                    lcd.print(currentMsg); // Show the final message
                    delay(2000);           // Wait for 2 seconds before exiting
                    typingActive = false; // End typing mode on double #
                    break;
                }
                // Append the current character to the message
                if (currentChar != '\0' && strlen(currentMsg) < sizeof(currentMsg) - 1) {
                    size_t len = strlen(currentMsg);
                    currentMsg[len] = currentChar; // Add new character
                    currentMsg[len + 1] = '\0';   // Null-terminate
                    cursorIndex++;
                }
                lastWasHash = true; // Set the flag for a single #
                continue;
            } else {
                lastWasHash = false; // Reset if any key other than # is pressed
            }

            // Get the letter for the current key
            currentChar = getLetterForKey(currentKey, &currentIndex[currentKey - '1'], &lastKeyPressed);

            // Display the current character on the LCD
            if (currentChar != '\0') {
                lcd.setCursor(cursorIndex, 0); // Move back to the start of the second row
                lcd.print(currentChar);       // Show the current character on the display
            }
        }
    }
    return currentMsg;  // Return the current message (if needed)
}


void printLeaderboard(int startIndex) {

    lcd.setCursor(0, 0);
    lcd.print("Leaderboard!");

    for (int i = 0; i < screenHeight; i++) { // Assuming a 2-line LCD
        if (startIndex + i < MAX_PLAYERS) {
            lcd.setCursor(0, i + 1);  // Move to the next row
            lcd.print(startIndex + i + 1);
            lcd.print(". ");
            lcd.print(leaderboard[startIndex + i].name);
            lcd.print(": ");
            lcd.print(leaderboard[startIndex + i].score);
        }
    }
}

void scrollLeaderboard() {
    int startIndex = 0;  // Start displaying from the top
    bool exit = false;
    
    while (!exit) {
        // Display the leaderboard from the current start index
        printLeaderboard(startIndex);

        char key = customKeypad.getKey();  // Assuming customKeypad is defined elsewhere

        if (key == 'A' && startIndex > 0) {
            startIndex--;  // Scroll up
            lcd.clear();
        }
        if (key == 'B' && startIndex < MAX_PLAYERS - 1) {
            startIndex++;  // Scroll down
            lcd.clear();
        }
        if (key == '*') {
            exit = true;  // Exit the loop
        }
        
        delay(200);  // Short delay to debounce button presses
    }
}

void insertPlayer(Player newPlayer) {
    // Insert the new player into the leaderboard at the correct position
    int i;
    for (i = MAX_PLAYERS - 1; i > 0; i--) {
        if (leaderboard[i - 1].score > newPlayer.score) {
            break;
        }
        leaderboard[i] = leaderboard[i - 1];
    }
    leaderboard[i] = newPlayer;

    // Sort the leaderboard by score in descending order
    for (i = 0; i < MAX_PLAYERS; i++) {
        for (int j = i + 1; j < MAX_PLAYERS; j++) {
            if (leaderboard[i].score < leaderboard[j].score) {
                Player temp = leaderboard[i];
                leaderboard[i] = leaderboard[j];
                leaderboard[j] = temp;
            }
        }
    }
}

void resetGame() {
    gameActive = true;      // Activate the game
    misses = 0;             // Reset misses
    score = 0;              // Resets score
    moleTimeLimit = 1500;   // Reset the initial mole time limit
    int allMolesDown[4] = {downPosition, downPosition, downPosition, downPosition};
    moveMoles(allMolesDown); // Set all moles to the down position
    delay(200);              // Small delay to let everything reset
    lastMole = -1;           // Reset last mole index to ensure randomness on game start
    danceTime = true;
    lcd.clear();

    // Explicitly clear currentMsg and reset current_username
    current_username = ""; // Reset pointer
    memset(currentMsg, 0, sizeof(currentMsg)); // Clear the static array in typing()
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

  Serial.begin(9600); // Start serial communication
  lcd.begin(16,2);
  lcd.init();
  lcd.clear();
  lcd.backlight();

  // Introductory message
    lcd.setCursor(0, 0);
    lcd.print("Welcome!"); // Display welcome message
    delay(2000);           // Show for 2 seconds
    lcd.clear();          // Clear the display for the main functionality
    typingActive = false;  // Set typingActive to true to start typing mode
}

//Main game loop
void loop() {
  char currentKey = customKeypad.getKey();
  if (!gameActive) {
    lcd.setCursor(0, 0);
    lcd.print("Press A to Start!");
    lcd.setCursor(0, 1);
    lcd.print("Press B for Scores!");
  }

  // Check if reset button is pressed to start or reset the game
  if (currentKey == 'A') {
    lcd.clear();
    lcd.print("Game Started!");
    delay(1000);
    resetGame(); // Reset the game state
    lcd.clear();
  }
  
  if (currentKey == 'B') {
    lcd.clear();
    lcd.setCursor(0,0);
	lcd.print("* to exit");
    lcd.setCursor(0,1);
  	lcd.print("A to scroll up");
    lcd.setCursor(0,2);
    lcd.print("B to scroll down");
    delay(3000);
    
    lcd.clear();
    scrollLeaderboard();
  }

  // Run the game logic if the game is active and misses are less than 3
  if (gameActive && misses < 3) {
    int randomMole = random(0, 4);
    if (randomMole == lastMole) {
      randomMole = (randomMole + 1) % 4; // Ensure it's not the same mole
    }
    lastMole = randomMole;

    moles[randomMole].write(upPosition); // Move the chosen mole to "up"
    starttime = millis();
    elapsed = 0;

    while (elapsed <= moleTimeLimit) {
      for (int i = 0; i < 4; i++) {
        if (moles[i].read() == upPosition) {
          checkMole(i); // Check if the mole was hit
        }
      }
      elapsed = millis() - starttime;
      delay(100);
    }

    if (moles[randomMole].read() == upPosition) {
      misses++; // Increment misses if the mole wasn't hit
      moles[randomMole].write(downPosition);
      tone(Piezo, 100); // Play miss tone
      delay(200);
      noTone(Piezo);
    }

    // Display score and misses
    lcd.setCursor(0, 0);
    lcd.print("Score: ");
    lcd.print(score * 100);
    lcd.setCursor(0, 1);
    lcd.print("Misses: ");
    lcd.print(misses);

    if (moleTimeLimit > 200) {
      moleTimeLimit -= 50; // Decrease mole time limit
    }
  }

  // Game over sequence
  if (misses >= 3 && gameActive) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over!");
    delay(1000);
    lcd.clear();
    gameActive = false; // Set game as inactive

    Player lastPlacePlayer = {"", 0};
	int validPlayerCount = 0;

    // Check the leaderboard
    for (int i = MAX_PLAYERS - 1; i >= 0; i--) {
        // Count valid players based on name and score
        if (leaderboard[i].name[0] != '\0' && leaderboard[i].score > 0) {
            validPlayerCount++;
        }
      
      	else {
            // If the player has no name, treat as invalid
            Serial.print("Empty Slot Detected at Index ");
            Serial.println(i);
        }

        // Find the worst valid player's score
        if (leaderboard[i].score > 0 && leaderboard[i].name[0] != '\0') {
            lastPlacePlayer = leaderboard[i];
            Serial.print("Last Place Player = ");
            Serial.println(lastPlacePlayer.name);
            break;
        }
    }

    // Check if the leaderboard is full
    if (validPlayerCount < MAX_PLAYERS) {
        Serial.println("Leaderboard is not full. Defaulting worst player's score to 0.");
        lastPlacePlayer.score = 0; // Default to 0 for incomplete leaderboards
    }

    
	Serial.print("Player's Score: ");
	Serial.println(score * 100);
	Serial.print("Last Place Score: ");
	Serial.println(lastPlacePlayer.score);
    
    // Check for high score
    if (score * 100 > lastPlacePlayer.score) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("New Highscore!");
      delay(2500);
      lcd.clear();
      lcd.print("Enter Username!");
      delay(1500);

      typingActive = true;
      char* current_username = typing();
      typingActive = false;
	  
      Player newPlayer;
      strncpy(newPlayer.name, current_username, sizeof(newPlayer.name) - 1); // Copy username
      newPlayer.name[sizeof(newPlayer.name) - 1] = '\0'; // Ensure null termination
      newPlayer.score = score * 100; // Set the score

      insertPlayer(newPlayer);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Added to");
      lcd.setCursor(0, 1);
      lcd.print("Leaderboard!");
      Serial.print("Current Username: ");
      Serial.println(current_username);
      delay(2000);
    }

    // Final animation
    if (danceTime) {
      for (int i = 0; i < 3; i++) {
        int allMolesUp[4] = {upPosition, upPosition, upPosition, upPosition};
        moveMoles(allMolesUp);
        delay(300);
        int allMolesDown[4] = {downPosition, downPosition, downPosition, downPosition};
        moveMoles(allMolesDown);
        delay(300);
      }
      danceTime = false;
      lcd.clear();
    }
  }
}
