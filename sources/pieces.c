#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "game.h" 
#include "pieces.h"



int load_pieces_from_file(const char* filename, Piece* allpieces) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Erreur ouverture fichier piÃ¨ces");
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
                compute_piece_size(&allpieces[piece_count]);
                piece_count++;
                row = 0;
            }
            //pass to the next piece, or just next line if there wasn't any piece
            continue;
            
        }

        

        if (row < MAX_PIECE_SIZE) {
            strncpy(allpieces[piece_count].shape[row], line, MAX_PIECE_SIZE);

            // Fill in the missing characters with spaces if the line is too short
            for (int i = strlen(line); i < MAX_PIECE_SIZE; i++) {
                allpieces[piece_count].shape[row][i] = '0';
            }
            row++;
        }
    }

    fclose(file);
    return piece_count + 1;

    
}


/*
 * Computes the size of a piece by determining its height and width.
 * The offsets are also calculated to find the top-left corner of the bounding box.
 *
 * Parameters:
 * - piece: pointer to the Piece structure
 */


void compute_piece_size(Piece* piece) {
    int top = MAX_PIECE_SIZE, bottom = -1;
    int left = MAX_PIECE_SIZE, right = -1;

    // Find bounding box of the actual shape
    for (int i = 0; i < MAX_PIECE_SIZE; i++) {
        for (int j = 0; j < MAX_PIECE_SIZE; j++) {
            if (piece->shape[i][j] == '1') {
                if (i < top) top = i;
                if (i > bottom) bottom = i;
                if (j < left) left = j;
                if (j > right) right = j;
            }
        }
    }

    // Compute width and height of the useful area
    piece->height = bottom - top + 1;
    piece->width = right - left + 1;

    // Copy the bounding box to a temp buffer
    char new_shape[MAX_PIECE_SIZE][MAX_PIECE_SIZE];
    for (int i = 0; i < MAX_PIECE_SIZE; i++)
        for (int j = 0; j < MAX_PIECE_SIZE; j++)
            new_shape[i][j] = '0';

    for (int i = 0; i < piece->height; i++) {
        for (int j = 0; j < piece->width; j++) {
            new_shape[i][j] = piece->shape[top + i][left + j];
        }
    }

    // Recenter new_shape inside 5x5
    piece->start_y = (MAX_PIECE_SIZE - piece->height) / 2;
    piece->start_x = (MAX_PIECE_SIZE - piece->width) / 2;

    // Clear original
    for (int i = 0; i < MAX_PIECE_SIZE; i++)
        for (int j = 0; j < MAX_PIECE_SIZE; j++)
            piece->shape[i][j] = '0';

    // Paste centered
    for (int i = 0; i < piece->height; i++) {
        for (int j = 0; j < piece->width; j++) {
            piece->shape[piece->start_y + i][piece->start_x + j] = new_shape[i][j];
        }
    }

    // Update piece position (optional: keep the same pos_x/pos_y)
    piece->offset_x = piece->offset_x;
    piece->offset_y = piece->offset_y;
}


/*
 * Checks if a piece can be placed on the grid at a new position.
 *
 * Parameters:
 * - grid: pointer to the Grid structure
 * - p: pointer to the Piece structure
 * - new_offset_x: intended X position (leftmost column)
 * - new_offset_y: intended Y position (top row)
 *
 * Returns:
 * - true if the piece fits without colliding or going out of bounds
 * - false otherwise
 */
bool is_valid_position(const Grid *grid, const Piece *p, int new_offset_x, int new_offset_y) {
    for (int i = 0; i < MAX_PIECE_SIZE; i++) {
        for (int j = 0; j < MAX_PIECE_SIZE; j++) {
            if (p->shape[i][j] == '1') { // only test occupied blocks
                int gx = new_offset_x + j;
                int gy = new_offset_y + i;

                // check if coordinates are within grid boundaries
                if (gx < 0 || gx >= grid->width || gy < 0 || gy >= grid->height)
                    return false;

                // check if the cell is already occupied in the grid
                if (grid->shape[gy][gx] != '0')
                    return false;
            }
        }
    }
    return true; // all tests passed
}


/*
 * Attempts to move a piece on the grid by dx (left/right) and dy (down).
 *
 * Parameters:
 * - grid: pointer to the Grid structure
 * - p: pointer to the Piece structure
 * - dx: change in X (e.g., -1 for left, +1 for right)
 * - dy: change in Y (e.g., +1 for down)
 *
 * Returns:
 * - true if the move is valid and applied
 * - false if the move would go out of bounds or cause collision
 */
bool move_piece(const Grid *grid, Piece *p, int dx, int dy) {
    int new_x = p->offset_x + dx; // compute the new column position
    int new_y = p->offset_y + dy; // compute the new row position

    // check if the new position is valid
    if (is_valid_position(grid, p, new_x, new_y)) {
        // apply the move
        p->offset_x = new_x;
        p->offset_y = new_y;
        return true;
    }

    // move blocked by collision or border
    return false;
}
void insert_piece_in_grid(Piece* piece, Grid* grid) {
    for (int i = piece->start_y; i < piece->height+piece->start_y; i++) {
        for (int j = piece->start_x; j < piece->width+piece->start_x; j++) {
            if (piece->shape[i][j] == '1') {
                int gx = piece->offset_x + j-1;
                int gy = piece->offset_y + i-1;

                // Check bounds
                if (gx >= 0 && gx < grid->width && gy >= 0 && gy < grid->height) {
                    grid->shape[gy][gx] = '1';
                }
            }
        }
    }
}

void remove_piece_from_grid(Piece* piece, Grid* grid) {
    for (int i = piece->start_y; i < piece->height+piece->start_y; i++) {
        for (int j = piece->start_x; j < piece->width+piece->start_x; j++) {
            if (piece->shape[i][j] == '1') {
                int gx = piece->offset_x + j-1;
                int gy = piece->offset_y + i-1;

                // Check bounds
                if (gx >= 0 && gx < grid->width && gy >= 0 && gy < grid->height) {
                    grid->shape[gy][gx] = '0';
                }
            }
        }
    }
}