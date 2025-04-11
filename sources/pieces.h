
#include "pieces.h"
#include <string.h> // pour memcpy

#define MAX_PIECE_SIZE 5
#define MAX_PIECES 7


typedef struct{
    char shape[MAX_PIECE_SIZE][MAX_PIECE_SIZE];
    int width;
    int height;
}Piece;

<<<<<<< HEAD:sources/pieces.h
int load_pieces_from_file(const char* filename, Piece* pieces);
=======


void rotate_piece(Piece *p, int angle) {
    char temp[MAX_PIECEa_SIZE][MAX_PIECE_SIZE]; // Piece temporaire qui ca réceptionner la pièce tournée. 
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
>>>>>>> c619189 (Rotationpiece):sources/pieces_management.c
