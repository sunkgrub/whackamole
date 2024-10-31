#include <stdio.h>

char* typing(); // Function prototype with char* return type
bool typingActive = false;

const char* numDict[] = {
    "abc",  // Corresponds to 1
    "def",  // Corresponds to 2
    "ghi",  // Corresponds to 3
    "pqrs", // Corresponds to 4
    "tuv",  // Corresponds to 5
    "wxyz"  // Corresponds to 6
};

// Custom string length function
int customStrLen(const char* str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

// Function to read a character from input
char checkCurKey() {
    char currentKey;
    scanf(" %c", &currentKey); // Read a character, ignoring leading whitespace
    return currentKey;
}

// Function to get the current letter for a key press
char getLetterForKey(char number, int *currentIndex, char *lastKeyPressed) {
    int index = number - '1';

    if (index >= 0 && index < sizeof(numDict) / sizeof(numDict[0])) {
        // If the same key is pressed, cycle through characters
        if (number == *lastKeyPressed) {
            *currentIndex = (*currentIndex + 1) % customStrLen(numDict[index]);
        } else {
            // If a new key is pressed, reset the index
            *currentIndex = 0;
            *lastKeyPressed = number;
        }
        return numDict[index][*currentIndex];
    }
    return '\0';
}

char* typing() {
    static char currentMsg[30] = ""; // Store the full message, static to persist after function ends
    int currentIndex[6] = {0};       // Track the current index for each number
    char lastKeyPressed = '\0';      // Track the last key pressed
    int msgLength = 0;               // Track the current message length
    bool lastWasHash = false;        // Flag to track if the previous key was '#'

    while (typingActive) {
        char currentKey = checkCurKey(); // Get the current key
        
        if (currentKey == '*') {
            typingActive = false;
            break;
        }

        if (currentKey == '#') {
            if (lastWasHash) {
                printf("Final message: %s\n", currentMsg); // Print final message
                typingActive = false;                      // End typing mode on double #
                break;
            }
            // Finalize the last character in the message
            if (msgLength < sizeof(currentMsg) - 1 && currentMsg[msgLength] != '\0') {
                msgLength++; // Move to the next position for future letters
            }
            printf("Current Msg (finalized so far): %s\n", currentMsg);
            lastWasHash = true; // Set the flag for a single #
            continue;
        } else {
            lastWasHash = false; // Reset if any key other than # is pressed
        }

        // Get the letter for the current key
        char currentChar = getLetterForKey(currentKey, &currentIndex[currentKey - '1'], &lastKeyPressed);

        // Update the current message to include the latest character
        if (currentChar != '\0') {
            currentMsg[msgLength] = currentChar;
            currentMsg[msgLength + 1] = '\0'; // Null-terminate for printing
            printf("Current Msg: %s\n", currentMsg); // Print the current full message
        } else {
            printf("Invalid input! Please enter a number between 1 and 6.\n");
        }
    }
    return currentMsg;
}

int main() {
    printf("Typing active\n");
    typingActive = true;
    char* display_str = typing(); // Call the typing function
    printf("Final displayed message: %s\n", display_str); // Show final result
    return 0;
}
