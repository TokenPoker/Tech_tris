#include <stdio.h>
#include <string.h>
#include "pieces.h"



int load_pieces_from_file(const char* filename, Piece* pieces) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erreur ouverture fichier pièces");
        return 0;
    }

    char line[100];
    int piece_count = 0;
    int row = 0;

    while (fgets(line, sizeof(line), file) && piece_count < MAX_PIECES){
        if(strncmp(line, "---", 3) == 0){
            piece_count++;
            row = 0;
            continue;
        }

        // remove newline character from the line
        line[strcspn(line, "\n")] = '\0';

        if (row < MAX_PIECE_SIZE) {
            strncpy(pieces[piece_count].shape[row], line, MAX_PIECE_SIZE);

            // Fill in the missing characters with spaces if the line is too short
            for (int i = strlen(line); i < MAX_PIECE_SIZE; i++) {
                pieces[piece_count].shape[row][i] = ' ';
            }
            row++;
        }
    }

    fclose(file);
    return piece_count + 1;

    
}


void rotate_piece(Piece *p, int angle) {
    char temp[MAX_PIECE_SIZE][MAX_PIECE_SIZE]; // Piece temporaire qui ca réceptionner la pièce tournée. 
    int n = MAX_PIECE_SIZE;

    // Nombre de rotations à 90° (ex : 180° = 2 fois)
    int num_rotations = (angle / 90) % 4;

    for (int r = 0; r < num_rotations; r++) {
        // Rotation à 90° dans temp
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                temp[j][n - 1 - i] = p->shape[i][j];
            }
        }

        // Copie de temp dans shape
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                p->shape[i][j] = temp[i][j];
            }
        }

        // Mettre à jour width/height
        int tmp = p->width;
        p->width = p->height;
        p->height = tmp;
    }
}