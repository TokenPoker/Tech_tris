#ifndef PIECES_H
#define PIECES_H

#include <stdbool.h>
#include "game.h" 
#define MAX_PIECE_SIZE 5
#define MAX_PIECES 6

typedef struct {
    char shape[MAX_PIECE_SIZE][MAX_PIECE_SIZE]; // representation 2D de la pièce
    int width;     // largeur réelle
    int height;    // hauteur réelle
    int offset_x;  // position x (colonne) de la pièce dans la grille
    int offset_y;  // position y (ligne) dans la grille
} Piece;

// Charge les pièces depuis un fichier texte
int load_pieces_from_file(const char* filename, Piece* pieces);

// Calcule les dimensions réelles de la pièce
void compute_piece_size(Piece* piece);

// Vérifie si une pièce peut être placée à une position
bool is_valid_position(const Grid *grid, const Piece *p, int new_offset_x, int new_offset_y);

// Tente de déplacer une pièce sur la grille
bool move_piece(const Grid *grid, Piece *p, int dx, int dy);


#endif // PIECES_H