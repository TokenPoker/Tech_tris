
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL.h>
#include "game.h"
#include "pieces.h"

// Clear full lines and increment score
void clear_full_lines(Grid *grid) {
    for (int i = 0; i < grid->height; i++) {
        bool full = true;
        for (int j = 0; j < grid->width; j++) {
            if (grid->shape[i][j] != '1') {
                full = false;
                break;
            }
        }

        if (full) {
            for (int k = i; k > 0; k--) {
                for (int l = 0; l < grid->width; l++) {
                    grid->shape[k][l] = grid->shape[k - 1][l];
                }
            }
            for (int l = 0; l < grid->width; l++) {
                grid->shape[0][l] = '0';
            }
        }
    }
}

// Check if piece collides at a given position
bool check_collision(const Piece *piece, const Grid *grid, int newX, int newY) {
    for (int i = 0; i < piece->height; i++) {
        for (int j = 0; j < piece->width; j++) {
            if (piece->shape[i][j] == '1') {
                int gx = newX + j;
                int gy = newY + i;
                if (gx < 0 || gx >= grid->width || gy >= grid->height)
                    return true;
                if (gy >= 0 && grid->shape[gy][gx] == '1')
                    return true;
            }
        }
    }
    return false;
}

// Move piece to the left
void move_piece_left(Piece *piece, const Grid *grid) {
    if (!check_collision(piece, grid, piece->offset_x - 1, piece->offset_y)) {
        piece->offset_x--;
    }
}

// Move piece to the right
void move_piece_right(Piece *piece, const Grid *grid) {
    if (!check_collision(piece, grid, piece->offset_x + 1, piece->offset_y)) {
        piece->offset_x++;
    }
}

// Move piece down; return true if moved, false if locked
bool move_piece_down(Piece *piece, const Grid *grid) {
    if (!check_collision(piece, grid, piece->offset_x, piece->offset_y + 1)) {
        piece->offset_y--;
        return true;
    }
    return false;
}

// Lock piece into the grid
void lock_piece(const Piece *piece, Grid *grid) {
    for (int i = 0; i < piece->height; i++) {
        for (int j = 0; j < piece->width; j++) {
            if (piece->shape[i][j] == '1') {
                int gx = piece->offset_x + j;
                int gy = piece->offset_y + i;
                if (gx >= 0 && gx < grid->width && gy >= 0 && gy < grid->height) {
                    grid->shape[gy][gx] = '1';
                }
            }
        }
    }
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

        // Update width and height after rotation
        int tmp = p->width;
        p->width = p->height;
        p->height = tmp;
    }
}

// Game over check: if piece cannot be placed at top
bool is_game_over(const Grid* grid, const Piece* next_piece) {
    for (int i = 0; i < next_piece->height; i++) {
        for (int j = 0; j < next_piece->width; j++) {
            if (next_piece->shape[i][j] == '1') {
                int gx = next_piece->offset_x + j;
                int gy = next_piece->offset_y + i;

                // Si la case est hors limites de la grille (gauche, droite ou bas)
                if (gx < 0 || gx >= grid->width || gy < 0 || gy >= grid->height)
                    return true;

                // Si la case est déjà occupée
                if (grid->shape[gy][gx] == '1')
                    return true;
            }
        }
    }
    return false;
}


// Picks a random piece from the list and spawns it at the top center of the grid
void spawn_random_piece_from_list(Grid *grid, Piece *currentPiece, Piece *allPieces, int totalPieces) {
    // Choose a random index
    int index = rand() % totalPieces;

    // Copy the selected piece into currentPiece
    *currentPiece = allPieces[index];

    // Set initial position (top center)
    currentPiece->offset_y = 0;
    currentPiece->offset_x = (grid->width - currentPiece->width) / 2;

    // Add the piece to the grid
    for (int i = 0; i < currentPiece->height; i++) {
        for (int j = 0; j < currentPiece->width; j++) {
            if (currentPiece->shape[i][j] == '1') {
                int x = currentPiece->offset_x + j;
                int y = currentPiece->offset_y + i;
                if (x >= 0 && x < grid->width && y >= 0 && y < grid->height) {
                    grid->shape[y][x] = '1';
                }
            }
        }
    }
}


bool auto_drop_piece(Piece* piece, const Grid* grid, int mode,Uint32 *lastDropTime) {
    Uint32 currentTime = SDL_GetTicks();

    // initiate lastDropTime if not already done
    int dropDelay;
    switch (mode) {
        case 0: dropDelay = 800; break;  // CLASSIC
        case 1: dropDelay = 300; break;  // HARD
        case 2: dropDelay = 1200; break; // ZEN
        default: dropDelay = 800; break; // DEFAULT
    }

    // Vérify if the time is right to drop the piece
    if (currentTime - *lastDropTime >= (Uint32)dropDelay) {
        *lastDropTime = currentTime;
        return move_piece_down(piece, grid);
    }

    return true; 
}