

#define MAX_PIECE_SIZE 5
#define MAX_PIECES 10


typedef struct{
    char shape[MAX_PIECE_SIZE][MAX_PIECE_SIZE];
    int width;
    int height;
}Piece;