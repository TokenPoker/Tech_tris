#ifndef GAME_H
#define GAME_H


#include <stdbool.h>
#include <SDL.h>
#include "pieces.h"

/**
 * Clears all full lines in the grid and shifts down upper lines.
 *
 * Parameters:
 *   - grid : Pointer to the game grid to process.
 *   - score: Pointer to the current score variable, incremented when a line is cleared.
 *
 * Returns: void.
 */
void clear_full_lines(Grid *grid, int* score);

/**
 * Checks for collision between a piece and the grid at a given position.
 *
 * Parameters:
 *   - piece : Pointer to the piece to check.
 *   - grid  : Pointer to the grid where collision is checked.
 *   - newX  : Proposed horizontal position (X offset).
 *   - newY  : Proposed vertical position (Y offset).
 *
 * Returns:
 *   - true  if there is a collision.
 *   - false if the piece can be placed at the position.
 */
bool check_collision(const Piece *piece, const Grid *grid, int newX, int newY);

/**
 * Attempts to move the piece one cell to the left if no collision occurs.
 *
 * Parameters:
 *   - piece : Pointer to the piece to move.
 *   - grid  : Pointer to the grid for collision checking.
 *
 * Returns: void.
 */
void move_piece_left(Piece *piece, const Grid *grid);

/**
 * Attempts to move the piece one cell to the right if no collision occurs.
 *
 * Parameters:
 *   - piece : Pointer to the piece to move.
 *   - grid  : Pointer to the grid for collision checking.
 *
 * Returns: void.
 */
void move_piece_right(Piece *piece, const Grid *grid);

/**
 * Moves the piece one cell down if no collision occurs.
 *
 * Parameters:
 *   - piece : Pointer to the piece to move.
 *   - grid  : Pointer to the grid for collision checking.
 *
 * Returns:
 *   - true  if the piece was moved down successfully.
 *   - false if the piece cannot move further (must be locked).
 */
bool move_piece_down(Piece *piece, const Grid *grid);

/**
 * Locks the piece into the grid: its filled cells become part of the static grid.
 *
 * Parameters:
 *   - piece : Pointer to the piece to lock.
 *   - grid  : Pointer to the grid where the piece is inserted.
 *
 * Returns: void.
 */
void lock_piece(const Piece *piece, Grid *grid);

/**
 * Rotates a piece by a given angle (90, 180, or 270 degrees clockwise).
 *
 * Parameters:
 *   - p     : Pointer to the piece to rotate.
 *   - angle : Rotation angle in degrees. Must be a multiple of 90.
 *
 * Returns: void. The shape, width, and height of the piece are updated.
 */
void rotate_piece(Piece *p, int angle);

/**
 * Checks if the game is over. This happens if a new piece overlaps with the grid.
 *
 * Parameters:
 *   - grid       : Pointer to the game grid.
 *   - next_piece : Pointer to the piece to check at its starting position.
 *
 * Returns:
 *   - true  if the piece cannot be placed (game over).
 *   - false otherwise.
 */
bool is_game_over(const Grid* grid, const Piece* next_piece);

/**
 * Selects a random piece from a list and initializes its position at the top center of the grid.
 *
 * Parameters:
 *   - grid         : Pointer to the game grid (used to center the piece horizontally).
 *   - currentPiece : Pointer to the piece variable to initialize.
 *   - allPieces    : Array of available pieces to choose from.
 *   - totalPieces  : Total number of pieces in the list.
 *
 * Returns: void.
 */
void spawn_random_piece_from_list(Grid *grid, Piece *currentPiece, Piece *allPieces, int totalPieces);

/**
 * Automatically moves the piece down at intervals depending on the game mode.
 *
 * Parameters:
 *   - piece         : Pointer to the piece to move.
 *   - grid          : Pointer to the game grid.
 *   - mode          : Game mode (0 = Classic, 1 = Hard, 2 = Zen).
 *   - lastDropTime  : Pointer to the last drop time (updated inside the function).
 *
 * Returns:
 *   - true  if the piece is still falling.
 *   - false if the piece cannot move and should be locked.
 */
bool auto_drop_piece( Piece *piece, const Grid *grid, int mode, Uint32 *lastDropTime);

/**
 * Clears all cells in the grid (sets them to '0').
 *
 * Parameters:
 *   - grid : Pointer to the game grid to reset.
 *
 * Returns: void.
 */
void clear_grid(Grid *grid);

#endif // GAME_H


