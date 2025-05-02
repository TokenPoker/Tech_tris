
#define MAX_PIECE_SIZE 5
#define MAX_PIECES 7


typedef struct{
    char shape[MAX_PIECE_SIZE][MAX_PIECE_SIZE];
    int width;     // real width of the piece
    int height;    // real height of the piece
    int offset_x;  // position of the upleft corner of the piece
    int offset_y;
}Piece;

int load_pieces_from_file(const char* filename, Piece* pieces);
void rotate_piece(Piece *p, int angle);


bool is_game_over(const Grid* grid, const Piece* next_piece);
