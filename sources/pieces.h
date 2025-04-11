
#define MAX_PIECE_SIZE 5
#define MAX_PIECES 7


typedef struct{
    char shape[MAX_PIECE_SIZE][MAX_PIECE_SIZE];
    int width=MAX_PIECE_SIZE;
    int height=MAX_PIECE_SIZE;
}Piece;

int load_pieces_from_file(const char* filename, Piece* pieces);
void rotate_piece(Piece *p, int angle);