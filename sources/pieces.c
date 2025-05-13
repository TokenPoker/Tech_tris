#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "game.h" 



// Load pieces from a text file into the array `allpieces`.
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
                compute_piece_size(&allpieces[piece_count]);// Calculate size of the piece
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
    return piece_count + 1; // Return total pieces loaded

    
}


// Calculate the actual size and center of a piece
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

// Insert the piece's current shape into the grid
void insert_piece_in_grid(Piece* piece, Grid* grid) {
    for (int i = piece->start_y; i < piece->height+piece->start_y; i++) {
        for (int j = piece->start_x; j < piece->width+piece->start_x; j++) {
            if (piece->shape[i][j] == '1') {
                int gx = piece->offset_x + j-1;
                int gy = piece->offset_y + i-1;

                // Check bounds
                if (gx >= 0 && gx < grid->width && gy >= 0 && gy < grid->height) {
                    grid->shape[gy][gx] = '1';// Mark the grid cell
                }
            }
        }
    }
}
// Remove the piece from the grid (restore empty cells).
void remove_piece_from_grid(Piece* piece, Grid* grid) {
    for (int i = piece->start_y; i < piece->height+piece->start_y; i++) {
        for (int j = piece->start_x; j < piece->width+piece->start_x; j++) {
            if (piece->shape[i][j] == '1') {
                int gx = piece->offset_x + j-1;
                int gy = piece->offset_y + i-1;

                // Check bounds
                if (gx >= 0 && gx < grid->width && gy >= 0 && gy < grid->height) {
                    grid->shape[gy][gx] = '0';// Clear the cell
                }
            }
        }
    }
}