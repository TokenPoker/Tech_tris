
typedef struct{
    int score;
    char pseudo[50];
} Score;
void save_scores_to_file(const char* name, int score, const char* filename);
int load_scores_from_file(const char* filename, Score* scores);
