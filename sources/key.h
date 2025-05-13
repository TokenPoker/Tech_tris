#ifndef KEY_H
#define KEY_H

#include <SDL.h>
#include <SDL_mixer.h>
#include "pieces.h"
#include "game.h"
#include "score.h"

typedef enum {
    STATE_MENU,
    STATE_OPTIONS,
    STATE_GAME,
    STATE_PSEUDO,
    STATE_PAUSE,
    STATE_GAMEOVER,
    STATE_EXIT
} GameState;

void handleEvents(SDL_Event *event, GameState *currentState, GameState *previousState, GameState *futurState,
                  Piece *currentPiece, Grid *lockGrid, Piece allPieces[], int *score, char playerName[],
                  int *nameLength, int *musicVolume, int *isMuted, int *selectedMode,
                  Mix_Music* bgm, bool *running, SDL_Rect mainMenubtn[], SDL_Rect pauseMenubtn[], SDL_Rect gameOverbtn[]);

#endif
