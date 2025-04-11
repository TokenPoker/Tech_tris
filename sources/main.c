#include <stdio.h>
#include "pieces.h"

int main() {
    Piece pieces[MAX_PIECES];
    int n = load_pieces_from_file("pieces.txt", pieces);
    
    for (int p = 0; p < n; p++) {
        printf("Pièce %d :\n", p+1);
        for (int i = 0; i < MAX_PIECE_SIZE; i++) {
            for (int j = 0; j < MAX_PIECE_SIZE; j++) {
                printf("%c", pieces[p].shape[i][j]);
            }
            printf("\n");
        }
        printf("----\n");
    }

    return 0;
}