#ifndef PIECES_H
#define PIECES_H

#include <stdbool.h>
#define GRID_WIDTH 10
#define GRID_HEIGHT 10
#define MAX_PIECE_SIZE 5
#define MAX_PIECES 6

typedef struct {
    char shape[MAX_PIECE_SIZE][MAX_PIECE_SIZE];
    int width;
    int height;
    int offset_x;
    int offset_y;
    int start_x;
    int start_y;
} Piece;


/**
 * Structure representing the game grid.
 * Each cell can contain:
 *   - '0' for an empty cell
 *   - '1' for a filled cell
 * 
 * Members:
 *   shape  : 2D array representing the grid cells.
 *   width  : Width of the grid (usually GRID_WIDTH).
 *   height : Height of the grid (usually GRID_HEIGHT).
 */
typedef struct {
    char shape[GRID_HEIGHT][GRID_WIDTH];
    int width;
    int height;
} Grid;

// Charge les pièces à partir d’un fichier texte
int load_pieces_from_file(const char* filename, Piece* allpieces);

// Calcule la taille (width/height) et les offsets d’une pièce
void compute_piece_size(Piece* piece);

// Vérifie si une position est valide pour une pièce donnée
bool is_valid_position(const Grid *grid, const Piece *p, int new_offset_x, int new_offset_y);

// Tente de déplacer une pièce sur la grille (renvoie true si réussi)
bool move_piece(const Grid *grid, Piece *p, int dx, int dy);

// Insère une pièce dans la grille (en modifiant grid)
void insert_piece_in_grid(Piece* piece, Grid* grid);

// Retire une pièce de la grille
void remove_piece_from_grid(Piece* piece, Grid* grid);

#endif // PIECES_H
