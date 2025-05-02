#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "score.h"

// Sauvegarde un score (nom et points) dans un fichier (mode ajout)
void save_scores_to_file(const char* name, int score, const char* filename){
    FILE* file = fopen(filename, "a");
    if (!file) {
        perror("Failed to open score file");
        return;
    }
    fprintf(file, "%s:%d\n", name, score);
    fclose(file);
}

// Charge tous les scores depuis un fichier dans un tableau de Score
int load_scores_from_file(const char* filename, Score* scores){
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Could not open score file");
        return 0;
    }

    int count = 0;
    char line[100];

    while (fgets(line, sizeof(line), file)) {
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

// Met à jour le score actuel (+100 par ligne détruite)
void update_score(int* currentScore, int linesCleared) {
    *currentScore += linesCleared * 100;
}

// Vérifie et enregistre le meilleur score s'il est battu
void check_and_save_highscore(int currentScore, const char* playerName, const char* filename) {
    HighScore hs;
    int found = load_highscore(&hs, filename);

    if (!found || currentScore > hs.score) {
        FILE* file = fopen(filename, "w");
        if (!file) {
            perror("Failed to open highscore file");
            return;
        }

        fprintf(file, "%s:%d\n", playerName, currentScore);
        fclose(file);

        printf("🎉 New High Score: %s with %d points!\n", playerName, currentScore);
    }
}

// Charge le meilleur score depuis un fichier
int load_highscore(HighScore* hs, const char* filename) {
    FILE* file = fopen(filename, "r");
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
