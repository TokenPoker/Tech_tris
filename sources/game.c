
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL.h>
#include "game.h"
#include "pieces.h"
#include "score.h"


// Clear all full lines in the grid and make upper lines fall down
void clear_full_lines(Grid *grid, int* score) {
    for (int row = grid->height - 1; row >= 0; row--) {
        bool full = true;

        // Check if current row is full
        for (int col = 0; col < grid->width; col++) {
            if (grid->shape[row][col] != '1') {
                full = false;
                break;
            }
        }

        // If full, move all rows above down by 1
        if (full) {
            for (int r = row; r > 0; r--) {
                for (int c = 0; c < grid->width; c++) {
                    grid->shape[r][c] = grid->shape[r - 1][c];
                }
            }

            // Clear top row after shifting
            for (int c = 0; c < grid->width; c++) {
                grid->shape[0][c] = '0';
            }

            // Increment score
            update_score(score);

            // Recheck same row after shifting
            row++;
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
        
        piece->offset_y++;
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


}


bool auto_drop_piece(Piece* piece, const Grid* grid, int mode,Uint32 *lastDropTime) {
    Uint32 currentTime = SDL_GetTicks();

    // initiate lastDropTime if not already done
    int dropDelay;
    switch (mode) {
        case 0: dropDelay = 800; break;  // CLASSIC
        case 1: dropDelay = 400; break;  // HARD
        case 2: dropDelay = 1200; break; // ZEN
        default: dropDelay = 800; break; // DEFAULT
    }

    // Vérify if the time is right to drop the piece
   
    if (currentTime - *lastDropTime >= (Uint32)dropDelay) {
        bool v = move_piece_down(piece, grid);
        // Update last drop time
        *lastDropTime = currentTime;
        return v;
    }

    return true; 
}