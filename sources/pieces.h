#ifndef PIECES_H
#define PIECES_H

#include <stdbool.h>
#include "game.h" 
#define MAX_PIECE_SIZE 5
#define MAX_PIECES 6

typedef struct {
    char shape[MAX_PIECE_SIZE][MAX_PIECE_SIZE]; // 2D array representing the piece shape.
    int width;     // The actual width of the piece.
    int height;    // The actual height of the piece.
    int offset_x;  // The x-position (column) of the piece in the grid.
    int offset_y;  // The y-position (row) of the piece in the grid.
    int start_x;   // The starting x-position (column) of the piece in the grid.
    int start_y;   // The starting y-position (row) of the piece in the grid.
} Piece;

// ** Function: load_pieces_from_file **
/**
 * Loads all pieces from a text file and stores them in the provided array of pieces.
 *
 * Parameters:
 *   - filename : The path to the text file containing the piece data.
 *   - pieces   : Pointer to the array of pieces where the data will be stored.
 *
 * Returns:
 *   - An integer representing the total number of pieces successfully loaded.
 *     Returns 0 if the file cannot be opened or no pieces are loaded.
 *
 * Function details:
 *   The function reads the piece data from the file line by line, splitting pieces by the separator "---".
 *   It processes each piece and stores its shape in a `Piece` structure.
 *   If the piece is valid, the function also calculates its actual size using the `compute_piece_size` function.
 */
int load_pieces_from_file(const char* filename, Piece* pieces);

// ** Function: compute_piece_size **
/**
 * Computes the actual dimensions of a piece by finding its bounding box.
 *
 * Parameters:
 *   - piece : Pointer to the piece whose dimensions are to be computed.
 *
 * Returns:
 *   - None. The `piece` is modified directly to update its width, height, and position.
 *
 * Function details:
 *   This function analyzes the `shape` of the piece to determine its height and width by checking
 *   the boundaries of the occupied cells ('1'). It then centers the shape inside a fixed grid size
 *   (5x5) and stores the resulting information in the piece structure.
 */
void compute_piece_size(Piece* piece);

// ** Function: insert_piece_in_grid **
/**
 * Inserts a piece's shape into the given game grid at its current offset position.
 *
 * Parameters:
 *   - piece : Pointer to the piece to be inserted into the grid.
 *   - grid  : Pointer to the grid where the piece should be placed.
 *
 * Returns:
 *   - None.
 *
 * Function details:
 *   This function goes through the shape of the piece and places its '1' cells into the grid
 *   at the corresponding positions. The grid is updated to reflect the new placement of the piece.
 */
void insert_piece_in_grid(Piece* piece, Grid* grid);

// ** Function: remove_piece_from_grid **
/**
 * Removes a piece's shape from the given game grid by clearing the cells it occupies.
 *
 * Parameters:
 *   - piece : Pointer to the piece to be removed from the grid.
 *   - grid  : Pointer to the grid where the piece should be removed.
 *
 * Returns:
 *   - None.
 *
 * Function details:
 *   This function iterates through the shape of the piece and removes it from the grid by
 *   resetting the corresponding grid cells to '0'. This is used when a piece has finished its fall
 *   and needs to be cleared from the grid before placing a new piece.
 */
void remove_piece_from_grid(Piece* piece, Grid* grid);


#endif // PIECES_H