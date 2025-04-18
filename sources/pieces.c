#include <stdio.h>
#include <string.h>
#include "pieces.h"
#include "game.h"
#include <stdbool.h>


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