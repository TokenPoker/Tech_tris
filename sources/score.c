#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "score.h"



void save_scores_to_file(const char* name, int score, const char* filename){
    FILE* file = fopen(filename, "a");
    if (!file) {
        perror("Failed to open score file");
        return;
    }
    fprintf(file, "%s:%d\n", name, score);
    fclose(file);
}
int load_scores_from_file(const char* filename, Score* scores){
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Could not open score file");
        return 0;
    }

    int count = 0;
    char line[100];

    while (fgets(line, sizeof(line), file) ) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline
        char* separator = strchr(line, ':');
        if (separator) {
            *separator = '\0';
            strcpy(scores[count].pseudo, line);
            scores[count].score = atoi(separator + 1);
            count++;
        }
    }

    fclose(file);
    return count;
}
