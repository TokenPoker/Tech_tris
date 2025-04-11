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

        // remove newline character from the line
        line[strcspn(line, "\n")] = '\0';

        //check if there is the separator
        if(strncmp(line, "---", 3) == 0){
            if (row > 0) {
                compute_piece_size(&pieces[piece_count]);
                piece_count++;
                row = 0;
            }
            //pass to the next piece, or just next line if there wasn't any piece
            continue;
            
        }

        

        if (row < MAX_PIECE_SIZE) {
            strncpy(pieces[piece_count].shape[row], line, MAX_PIECE_SIZE);

            // Fill in the missing characters with spaces if the line is too short
            for (int i = strlen(line); i < MAX_PIECE_SIZE; i++) {
                pieces[piece_count].shape[row][i] = '0';
            }
            row++;
        }
    }

    fclose(file);
    return piece_count + 1;

    
}


void rotate_piece(Piece *p, int angle) {
    char temp[MAX_PIECE_SIZE][MAX_PIECE_SIZE]; // temporary array to store the rotated piece
    int n = MAX_PIECE_SIZE;

    // number of 90 degree rotations
    int num_rotations = (angle / 90) % 4;

    for (int r = 0; r < num_rotations; r++) {
        // Rotation at 90 degrees in temp
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                temp[j][n - 1 - i] = p->shape[i][j];
            }
        }

        // Copy temp in shape
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                p->shape[i][j] = temp[i][j];
            }
        }

        //Put up to date the width and height
        int tmp = p->width;
        p->width = p->height;
        p->height = tmp;
    }
}


void compute_piece_size(Piece* piece) {
    int top = MAX_PIECE_SIZE, bottom = -1;
    int left = MAX_PIECE_SIZE, right = -1;

    for (int i = 0; i < MAX_PIECE_SIZE; i++) {
        for (int j = 0; j < MAX_PIECE_SIZE; j++) {
            if (piece->shape[i][j] != '0') {
                if (i < top) top = i;
                if (i > bottom) bottom = i;
                if (j < left) left = j;
                if (j > right) right = j;
            }
        }
    }


    // Compute the height and width of the active area of the piece
    piece->height = (bottom >= top) ? (bottom - top + 1) : 0;
    piece->width  = (right >= left) ? (right - left + 1) : 0;

    // Save the offsets of the top-left corner of the bounding box
    piece->offset_y = (bottom >= top) ? top : 0;
    piece->offset_x = (right >= left) ? left : 0;
}
