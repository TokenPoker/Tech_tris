// Structure du meilleur score
typedef struct {
    int score;
    char pseudo[50];
} HighScore;

void save_scores_to_file(const char* name, int score, const char* filename);

int load_scores_from_file(const char* filename, Score* scores);

// Ajout de score lors d'une suppression de lignes
void update_score(int* currentScore, int linesCleared);

// Vérifie si le score courant est un nouveau record, et le sauvegarde si c'est le cas
void check_and_save_highscore(int currentScore, const char* playerName, const char* filename);


int load_highscore(HighScore* hs, const char* filename);
