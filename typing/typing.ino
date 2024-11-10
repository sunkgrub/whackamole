#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

bool typingActive = false;

LiquidCrystal_I2C lcd(0x27, 20, 4);
const byte ROWS = 4;
const byte COLS = 4;

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

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Function to get the character based on the key pressed
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
    static char currentMsg[30] = ""; // Store the full message
    int currentIndex[6] = {0};       // Track the current index for each number
    char lastKeyPressed = '\0';      // Track the last key pressed
    bool lastWasHash = false;        // Flag to track if the previous key was '#'
    char currentChar;                // Current character to display
    int cursorIndex = 10;

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
                continue; // Break out of the typing loop to return to the main loop
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
                    currentMsg[strlen(currentMsg)] = currentChar; // Add new character to the message
                    currentMsg[strlen(currentMsg) + 1] = '\0'; // Null-terminate for printing
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
                // Clear the last character from the display and update it
                lcd.setCursor(cursorIndex, 0); // Move back to the start of the second row

                // Display the current character
                lcd.print(currentChar); // Show the current character on the display
            }
        }
    }
    return currentMsg;  // Return the current message (if needed)
}

void setup() {
    Serial.begin(9600); // Start serial communication
    lcd.init();
    lcd.clear();
    lcd.backlight();

    // Introductory message
    lcd.setCursor(0, 0);
    lcd.print("Welcome!"); // Display welcome message
    delay(2000);           // Show for 2 seconds
    lcd.clear();          // Clear the display for the main functionality
    typingActive = true;  // Set typingActive to true to start typing mode
}

void loop() {
    if (typingActive) {
        char* newUserName = typing(); // Start the typing function
        Serial.println(newUserName);
    } else {
        // Check for * to restart typing
        if (customKeypad.getKey() == '*') {
            typingActive = true; // Restart typing
        }
    }
}

