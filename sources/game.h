#define MAX_GRID_SIZE 10

typedef struct{
    char shape[MAX_GRID_SIZE][MAX_GRID_SIZE];
    int width= MAX_GRID_SIZE;
    int height=MAX_GRID_SIZE;
}Grid;