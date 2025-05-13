#ifndef KEY_H
#define KEY_H

#include <SDL.h>
#include <SDL_mixer.h>
#include "pieces.h"
#include "game.h"
#include "score.h"

// Enumeration of all possible game states
typedef enum {
    STATE_MENU,       // Main menu
    STATE_OPTIONS,    // Options menu
    STATE_GAME,       // Game in progress
    STATE_PSEUDO,     // Player name input
    STATE_PAUSE,      // Game paused
    STATE_GAMEOVER,   // Game over screen
    STATE_EXIT        // Exit confirmation
} GameState;

/**
 * Handles all SDL input events (keyboard, mouse, text) and updates game state accordingly.
 *
 * Parameters:
 *   - event        : Pointer to the SDL_Event to process.
 *   - currentState : Pointer to the current game state (may be updated based on input).
 *   - previousState: Pointer to the previous game state (used for returning from submenus).
 *   - futurState   : Pointer to the future state after confirmation dialogs.
 *   - currentPiece : Pointer to the currently active Tetris piece (modified during gameplay).
 *   - lockGrid     : Pointer to the game grid storing locked pieces.
 *   - allPieces    : Array of all available Tetris pieces (used to spawn new ones).
 *   - score        : Pointer to the player’s score (incremented on line clears).
 *   - playerName   : Character array for storing the player’s name (modified via text input).
 *   - nameLength   : Pointer to the current length of the player name.
 *   - musicVolume  : Pointer to the current background music volume (0–128).
 *   - isMuted      : Pointer to a flag indicating if the music is muted (1 = muted).
 *   - selectedMode : Pointer to the selected game mode (modified in options menu).
 *   - bgm          : Pointer to the background music.
 *   - running      : Pointer to the main game loop flag (set to false to quit).
 *   - mainMenubtn  : Array of SDL_Rects representing the clickable main menu buttons.
 *   - pauseMenubtn : Array of SDL_Rects representing the clickable pause menu buttons.
 *   - gameOverbtn  : Array of SDL_Rects representing the clickable game over screen buttons.
 *
 * Returns: void.
 */
void handleEvents(SDL_Event *event, GameState *currentState, GameState *previousState, GameState *futurState,
                  Piece *currentPiece, Grid *lockGrid, Piece allPieces[], int *score, char playerName[],
                  int *nameLength, int *musicVolume, int *isMuted, int *selectedMode,
                  Mix_Music* bgm, bool *running, SDL_Rect mainMenubtn[], SDL_Rect pauseMenubtn[], SDL_Rect gameOverbtn[]);

#endif
