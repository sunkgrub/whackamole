#include <stdio.h>
#include <stdbool.h>

// Function prototypes
void typing();
bool typingActive = false;

// Number-to-letter mapping
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
    char currentKey; // Declare currentKey as char
    scanf(" %c", &currentKey); // Read a character, ignoring leading whitespace
    return currentKey; // Return the read character
}

// Function to get the current letter based on the number pressed
char getLetterForKey(char number, int *currentIndex, char *lastKeyPressed) {
    // Convert number to index for numDict (e.g., '1' to 0, '2' to 1, etc.)
    int index = number - '1';

    // Ensure the index is valid
    if (index >= 0 && index < sizeof(numDict) / sizeof(numDict[0])) {
        // Check if the same key is pressed
        if (number == *lastKeyPressed) {
            // Cycle through the letters
            *currentIndex = (*currentIndex + 1) % customStrLen(numDict[index]); // Get the next letter
        } else {
            // If a different key is pressed, reset index to 0
            *currentIndex = 0;
            *lastKeyPressed = number; // Update the last key pressed
        }
        return numDict[index][*currentIndex]; // Return the current letter
    }
    return '\0'; // Return null if index is invalid
}

int main() {
    printf("Typing active\n");
    typingActive = true;
    typing(); // Call the typing function
    return 0;
}

void typing() {
    char currentKey;
    char currentMsg[30] = ""; // Initialize an empty string with space for 30 characters
    int currentIndex[6] = {0}; // Keep track of the current index for each number
    char lastKeyPressed = '\0'; // To track the last key pressed

    while (typingActive) { // Infinite loop
        currentKey = checkCurKey(); // Get the current key from the function
        
        if (currentKey == '*') {
            typingActive = false; // Exit the loop if the user enters '*'
            break; // Optional: Break out of the loop
        }

        if (currentKey == '#') {
            // Finalize the message by printing only the last character
            if (customStrLen(currentMsg) > 0) {
                printf("Final message: %s\n", currentMsg); // Print the final message
            } else {
                printf("Final message: (no characters entered)\n");
            }
            return; // End the function after finalizing the message
        }

        // Get the current letter for the pressed key
        char letter = getLetterForKey(currentKey, &currentIndex[currentKey - '1'], &lastKeyPressed);

        // Check if the letter is valid (not null)
        if (letter != '\0') {
            // Reset the message to only include the latest character
            currentMsg[0] = letter; // Only keep the latest letter
            currentMsg[1] = '\0';   // Null-terminate the string
            printf("Current Msg: %s\n", currentMsg); // Print the current message
        } 
        else {
            printf("Invalid input! Please enter a number between 1 and 6.\n");
        }
    }
}
