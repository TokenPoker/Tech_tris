#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "score.h"

// Save a player's score (name and points) to a file in append mode
void save_scores_to_file(const char* name, int score, const char* filename) {
    FILE* file = fopen(filename, "a");// Open file in append mode
    if (!file) {
        perror("Failed to open score file");
        return;
    }
    fprintf(file, "%s:%d\n", name, score);// Write name and score in "name:score" format
    fclose(file); // Close file
}

// Load all saved scores from a file into a Score array
int load_scores_from_file(const char* filename, Score* scores) {
    FILE* file = fopen(filename, "r");  // Open file in read mode
    if (!file) {
        perror("Could not open score file");
        return 0;
    }

    int count = 0;
    char line[100];

    while (fgets(line, sizeof(line), file)) {  // Read file line by line
        line[strcspn(line, "\n")] = '\0'; // Remove newline
        char* separator = strchr(line, ':'); // Find the ':' separator between name and score
        if (separator) {
            *separator = '\0'; // Replace ':' with null terminator to isolate name
            strcpy(scores[count].pseudo, line); // Copy name into structure
            scores[count].score = atoi(separator + 1);// Convert score string to int
            count++;// Increase score count
        }
    }

    fclose(file);// Close file
    return count;// Return number of loaded scores
}

// Increase current score: +100 points per line cleared
void update_score(int* currentScore) {
    *currentScore += 10 * 100;// Add 1000 points to the current score
}

// Check if the current score is a new high score and save it if true
void check_and_save_highscore(int currentScore, const char* playerName, const char* filename) {
    HighScore hs;
    int found = load_highscore(&hs, filename);// Try to load existing high score

    if (!found || currentScore > hs.score) {// If no high score or current is higher

        FILE* file = fopen(filename, "w");// Open file in write mode (overwrite)
        if (!file) {
            perror("Failed to open highscore file");
            return;
        }

        fprintf(file, "%s:%d\n", playerName, currentScore); // Save new high score
        fclose(file);
    }
}

// Load the best high score from the given file
int load_highscore(HighScore* hs, const char* filename) {
    FILE* file = fopen(filename, "r");// Open file in read mode
    if (!file) return 0;

    char line[100];
    if (fgets(line, sizeof(line), file)) {
        char* sep = strchr(line, ':');
        if (sep) {
            *sep = '\0';
            strcpy(hs->pseudo, line);
            hs->score = atoi(sep + 1);
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// Find the last recorded score for a given player name in the file
int find_score_by_name(const char* filename, const char* name) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Could not open score file");
        return -1;
    }

    char line[100];
    int foundScore = -1;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // remove newline
        char* separator = strchr(line, ':');
        if (separator) {
            *separator = '\0';
            if (strcmp(line, name) == 0) {
                foundScore = atoi(separator + 1); // update latest score found
            }
        }
    }

    fclose(file);
    return foundScore;
}


// Load all scores, sort them, and copy top 3 into top3 array
void get_top_three_scores_and_save(const char* inputFile, Score* top3[3]) {
    static Score allScores[100];
    int count = load_scores_from_file(inputFile, allScores);

    // If no scores found, initialize top3 with defaults
    for (int i = 0; i < 3; i++) {
        static Score defaultScore;
        defaultScore.score = 0;
        strcpy(defaultScore.pseudo, "N/A");
        top3[i] = &defaultScore;
    }

    if (count == 0) return;

    // Sort scores in descending order (highest first)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (allScores[j].score > allScores[i].score) {
                Score temp = allScores[i];
                allScores[i] = allScores[j];
                allScores[j] = temp;
            }
        }
    }

    // Assign top 3 pointers
    for (int i = 0; i < 3 && i < count; i++) {
        top3[i] = &allScores[i];
    }
}
