#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <SDL.h>

#define GRID_WIDTH 10
#define GRID_HEIGHT 10

typedef struct {
    char shape[GRID_HEIGHT][GRID_WIDTH]; // '0' = empty, '1' = filled
    int width;
    int height;
} Grid;

#include "pieces.h" // pour struct Piece

// Efface les lignes pleines de la grille
void clear_full_lines(Grid *grid, int* score);

// Vérifie les collisions d'une pièce à une position (newX, newY)
bool check_collision(const Piece *piece, const Grid *grid, int newX, int newY);

// Déplacement vers la gauche
void move_piece_left(Piece *piece, const Grid *grid);

// Déplacement vers la droite
void move_piece_right(Piece *piece, const Grid *grid);

// Déplacement vers le bas (retourne true si possible, false si bloqué)
bool move_piece_down(Piece *piece, const Grid *grid);

// Intègre la pièce dans la grille (la verrouille)
void lock_piece(const Piece *piece, Grid *grid);

// Rotation (angle = 90, 180, 270)
void rotate_piece(Piece *p, int angle);

// Vérifie si la partie est finie (impossible de placer la pièce en haut)
bool is_game_over(const Grid* grid, const Piece* next_piece);

// Génère une nouvelle pièce aléatoire et la place en haut de la grille
void spawn_random_piece_from_list(Grid *grid, Piece *currentPiece, Piece *allPieces, int totalPieces);

// Vérifie si la pièce peut être placée à une nouvelle position
bool auto_drop_piece( Piece *piece, const Grid *grid, int mode, Uint32 *lastDropTime);

#endif // GAME_H