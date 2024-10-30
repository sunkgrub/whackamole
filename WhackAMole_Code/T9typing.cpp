#include <stdio.h>

void typing();
char currentKey;

const char* numDict[] = {
    "abc", 
    "def", 
    "ghi",
    "pqrs",
    "tuv", 
    "wxyz"
};

int main () {
    printf("typing active\n");
    typing();
    return 0;
}

void typing() {
    while (true) {
        scanf("%c",&currentKey);
        if (currentKey == '*') {
            break;
        }
    }
}
