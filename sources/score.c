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


// Load all scores, sort them, and copy top 3 into top3 array
void get_top_three_scores_and_save(const char* inputFile, Score* top3[3]) {
    static Score allScores[100];// Static array to store all loaded scores
    int count = load_scores_from_file(inputFile, allScores);// Load scores from file

    // If no scores found, initialize top3 with defaults
    for (int i = 0; i < 3; i++) {
        static Score defaultScore; 
        defaultScore.score = 0;
        strcpy(defaultScore.pseudo, "N/A");
        top3[i] = &defaultScore;
    }

    if (count == 0) return;// If no scores, exit early

    // Sort scores in descending order (highest first)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (allScores[j].score > allScores[i].score) { // Swap if out of order
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
