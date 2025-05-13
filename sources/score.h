#ifndef SCORE_H
#define SCORE_H

// Structure for a player score (used in score history)
typedef struct {
    int score;// Player's score
    char pseudo[50];// Player's name (nickname)
} Score;

// Structure for the best score record
typedef struct {
    int score;// Highest score recorded
    char pseudo[50];// Name of the player who achieved it
} HighScore;

/**
 * Saves a player's score to a file (in append mode).
 *
 * Parameters:
 *   - name      : Name of the player to save.
 *   - score     : Score to associate with the player.
 *   - filename  : Path to the file where the score should be recorded.
 *
 * Returns:
 *   - void
 *
 * Remarks:
 *   - The score is appended to the file in the format "name:score".
 *   - The function opens the file in append mode to preserve previous entries.
 */
void save_scores_to_file(const char* name, int score, const char* filename);

/**
 * Loads all scores from a file into a provided array of Score structures.
 *
 * Parameters:
 *   - filename  : Path to the file containing the saved scores.
 *   - scores    : Array of Score structs where loaded scores will be stored.
 *
 * Returns:
 *   - The number of scores successfully loaded from the file.
 *
 * Remarks:
 *   - The function reads entries in the format "name:score".
 *   - Each line in the file is parsed into a Score object.
 */
int load_scores_from_file(const char* filename, Score* scores);

/**
 * Increments the current score when lines are cleared.
 *
 * Parameters:
 *   - currentScore : Pointer to the current score to update.
 *
 * Returns:
 *   - void
 *
 * Remarks:
 *   - Adds 1000 points per call (corresponding to 10 lines cleared).
 *   - Modify this function to match the actual scoring logic of the game.
 */
void update_score(int* currentScore);

/**
 * Loads scores from a file, sorts them in descending order,
 * and stores the top three scores in the provided top3 array.
 *
 * Parameters:
 *   - inputFile : Path to the file containing saved scores.
 *   - top3      : Array of 3 pointers to Score structures where
 *                 the top 3 scores will be stored.
 *
 * Returns:
 *   - void
 *
 * Remarks:
 *   - If there are fewer than 3 scores in the file, the remaining
 *     positions are filled with "N/A" and score = 0.
 *   - The original score list is sorted in-place using bubble sort.
 */
void get_top_three_scores_and_save(const char* inputFile, Score* top3[3]);

#endif

