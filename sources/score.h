#ifndef SCORE_H
#define SCORE_H

// Structure for a player score (used in score history)
typedef struct {
    int score;
    char pseudo[50];
} Score;

// Structure for the best score record
typedef struct {
    int score;
    char pseudo[50];
} HighScore;

// Save a score to the score file (used for history)
void save_scores_to_file(const char* name, int score, const char* filename);

// Load all saved scores into an array of Score structs
int load_scores_from_file(const char* filename, Score* scores);

// Increase the current score based on lines cleared (+100 per line)
void update_score(int* currentScore, int linesCleared);

// If the current score is a new high score, save it
void check_and_save_highscore(int currentScore, const char* playerName, const char* filename);

// Load the best high score from file (returns 1 if found, 0 if not)
int load_highscore(HighScore* hs, const char* filename);

// Search for the last recorded score of a player by name
int find_score_by_name(const char* filename, const char* name);


#endif

