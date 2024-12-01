#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Constants */
#define MAX_HIGH_SCORES 5
#define MAX_LIVES 3
#define TOTAL_LEVELS 10

/* Struct for Player High Scores */
typedef struct {
    char playerID[9];
    int score;
} PlayerScore;

/* Function Declarations */
void displayMainMenu();
void playGame(PlayerScore highScores[], int *highScoreCount);
void displayHighScores(PlayerScore highScores[], int highScoreCount);
void quitGame();
int isValidPlayerID(char playerID[]);
int randomCount();
float calculateTotalShots(float facehugger, float chestbuster, float xenomorph, float queen);
void updateHighScores(PlayerScore highScores[], int *highScoreCount, char playerID[], int score);
void displayLevel(int level, int facehugger, int chestbuster, int xenomorph, int queen);
void printAlien(char *alien, int count);


/* Validate Player ID */
int isValidPlayerID(char playerID[]) {
    if (strlen(playerID) != 8) return 0; // Must be 8 characters long
    for (int i = 0; i < 8; i++) {
        if (playerID[i] < '0' || playerID[i] > '9') return 0; // Ensure all characters are digits
    }
    return 1;
}

/* Generate Random Alien Count */
int randomCount() {
    return (rand() % 5) + 1; // Random number between 1 and 5
}

/* Calculate Total Shots */
float calculateTotalShots(float facehugger, float chestbuster, float xenomorph, float queen) {
    return (facehugger * 1) + (chestbuster * 2) + (xenomorph * 4.2) + (queen * 7.3);
}

/* Print Aliens */
void printAlien(char *alien, int count) {
    for (int i = 0; i < count; i++) {
        printf("%s ", alien);
    }
    puts(""); // Use puts for a newline
}

/* Update High Scores */
void updateHighScores(PlayerScore highScores[], int *highScoreCount, char playerID[], int score) {

    // Check if player ID already exists in high scores
    for (int i = 0; i < *highScoreCount; i++) {
        if (strcmp(highScores[i].playerID, playerID) == 0) {
            if (score > highScores[i].score) {
                highScores[i].score = score;
            }
            return;
        }
    }

    // If the player is not in the list and there is space, add the new score
    if (*highScoreCount < MAX_HIGH_SCORES) {
        
        // strcpy(destination, source) 
        strcpy(highScores[*highScoreCount].playerID, playerID);
        highScores[*highScoreCount].score = score;
        (*highScoreCount)++;
    } 
    
    else {

        // If the list is full, find the lowest score on the list
        int minIndex = 0;
        for (int i = 1; i < *highScoreCount; i++) {
            if (highScores[i].score < highScores[minIndex].score) {
                minIndex = i;
            }
        }

        // Replace the lowest score if the new score is higher
        if (score > highScores[minIndex].score) {
            strcpy(highScores[minIndex].playerID, playerID);
            highScores[minIndex].score = score;
        }
    }

    // Sort the high scores in descending order
    for (int i = 0; i < *highScoreCount - 1; i++) {
        for (int j = i + 1; j < *highScoreCount; j++) {
            if (highScores[j].score > highScores[i].score) {
                PlayerScore temp = highScores[i];
                highScores[i] = highScores[j];
                highScores[j] = temp;
            }
        }
    }
}

/* Display Level Information */
void displayLevel(int level, int facehugger, int chestbuster, int xenomorph, int queen) {
    printf("\n===== Level %d =====\n", level);
    printf("--------------------Aliens------------------\n");

    if (facehugger > 0) {
        printf("Facehugger: ");
        printAlien("//(~O~)\\\\", facehugger);
    }
    if (chestbuster > 0) {
        printf("Chestbuster: ");
        printAlien("\\\\<^o^>//", chestbuster);
    }
    if (xenomorph > 0) {
        printf("Xenomorph: ");
        printAlien("(((((((O,,,o)", xenomorph);
    }
    if (queen > 0) {
        printf("Queen: ");
        printAlien(")))O***O(((", queen);
    }
}

/* Play the Game */
void playGame(PlayerScore highScores[], int *highScoreCount) {
    char playerID[9];
    int lives = MAX_LIVES;
    int score = 0;
    int gameOver = 0;

    printf("\nEnter Player ID (8 digits): ");
    scanf("%s", playerID);

    if (!isValidPlayerID(playerID)) {
        printf("Invalid Player ID. Returning to Main Menu.\n");
        return;
    }

    for (int level = 1; level <= TOTAL_LEVELS && !gameOver; level++) {
        int facehugger = randomCount();
        int chestbuster = (level % 2 == 0) ? 0 : randomCount();
        int xenomorph = (level % 2 == 0) ? randomCount() : 0;
        int queen = (level == 10) ? randomCount() : 0;

        displayLevel(level, facehugger, chestbuster, xenomorph, queen);

        float totalShots = calculateTotalShots(facehugger, chestbuster, xenomorph, queen);
        printf("Enter your guess for total shots: ");
        float guess;
        scanf("%f", &guess);

        if (guess != totalShots) {
            lives--;
            printf("Incorrect! Lives remaining: %d\n", lives);
            if (lives == 0) {
                printf("Game Over! Final Score: %d\n", score);
                gameOver = 1;
            }
        } else {
            score++;
            printf("Correct! Current Score: %d\n", score);
        }
    }

    if (!gameOver) {
        printf("Congratulations! You completed the game with a score of %d\n", score);
    }
    updateHighScores(highScores, highScoreCount, playerID, score);
}

/* Display High Scores */
void displayHighScores(PlayerScore highScores[], int highScoreCount) {
    printf("\n===== High Scores =====\n");
    for (int i = 0; i < highScoreCount; i++) {
        printf("%d. Player ID: %s | Score: %d\n", i + 1, highScores[i].playerID, highScores[i].score);
    }
    if (highScoreCount == 0) {
        printf("No high scores recorded yet.\n");
    }
    printf("========================\n");
}

/* Quit the Game */
void quitGame() {
    printf("\nThank you for playing! Goodbye!\n");
}

/* Display the Main Menu */
void displayMainMenu() {
    printf("\n===== Main Menu =====\n");
    printf("1. Play Game\n");
    printf("2. View High Scores\n");
    printf("3. Quit Game\n");
    printf("=====================\n");
    printf("Enter your choice: ");
}

/* Main Function */
int main() {
    int choice;
    PlayerScore highScores[MAX_HIGH_SCORES] = {0}; // High score list initialized to zero
    int highScoreCount = 0; // Current count of recorded high scores
    srand(time(NULL)); // Seed for random number generation
    do {
        displayMainMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1: playGame(highScores, &highScoreCount); break;
            case 2: displayHighScores(highScores, highScoreCount); break;
            case 3: quitGame(); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
    return 0;
}