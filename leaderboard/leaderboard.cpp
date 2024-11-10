#include <iostream>

const int MAX_PLAYERS = 5;

// Declare the leaderboard globally (outside of any function)
struct Player {
    char name[10];
    int score;
};

// Declare the global leaderboard array
Player leaderboard[MAX_PLAYERS] = {{"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}}; // Initialize with empty players

void printLeaderboard() {
    std::cout << "Current Leaderboard:" << std::endl;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (leaderboard[i].score > 0) {  // Only print players with positive scores
            std::cout << (i + 1) << ". " << leaderboard[i].name << " - " << leaderboard[i].score << std::endl;
        }
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

int main() {
    // Test data
    leaderboard[0] = {"Alice", 100};
    leaderboard[1] = {"Bob", 90};
    leaderboard[2] = {"Charlie", 80};
    leaderboard[3] = {"David", 70};
    leaderboard[4] = {"Eve", 60};

    // Insert a new player with a score higher than the lowest player (Eve)
    Player newPlayer = {"Frank", 75};
    insertPlayer(newPlayer);

    // Print the leaderboard after insertion
    printLeaderboard();

    return 0;
}
