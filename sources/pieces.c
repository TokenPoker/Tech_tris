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