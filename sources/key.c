#include "key.h"

// Check whether the mouse coordinates (x, y) are within a given rectangle
bool isMouseOver(SDL_Rect rect, int x, int y) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

void handleEvents(SDL_Event *event, GameState *currentState, GameState *previousState, GameState *futurState,
                  Piece *currentPiece, Grid *lockGrid, Piece allPieces[], int *score, char playerName[],
                  int *nameLength, int *musicVolume, int *isMuted, int *selectedMode,
                  Mix_Music* bgm, bool *running, SDL_Rect mainMenubtn[], SDL_Rect pauseMenubtn[], SDL_Rect gameOverbtn[]) {

    int mx, my;

    switch (event->type) {
        // Handle SDL_QUIT event: the user wants to close the window
        case SDL_QUIT:
            *running = false;
            break;

        // Handle SDL_KEYDOWN event: a key is pressed
        case SDL_KEYDOWN:
            switch (*currentState) {
                // In the main menu: pressing ESC exits the game
                case STATE_MENU:
                    if (event->key.keysym.sym == SDLK_ESCAPE) {
                        *running = false;
                    }
                    break;
                 // In the options menu: control music volume and navigation
                case STATE_OPTIONS:
                    // Increase volume
                    if (event->key.keysym.sym == SDLK_UP && *musicVolume < 128) {
                        *musicVolume += 8;
                        Mix_VolumeMusic(*musicVolume);
                        *isMuted = 0;
                    // Decrease volume
                    } else if (event->key.keysym.sym == SDLK_DOWN && *musicVolume > 0) {
                        *musicVolume -= 8;
                        Mix_VolumeMusic(*musicVolume);
                        *isMuted = 0;
                    // Mute or unmute music
                    } else if (event->key.keysym.sym == SDLK_m) {
                        Mix_VolumeMusic(0);
                        *isMuted = 1;
                    } else if (event->key.keysym.sym == SDLK_p) {
                        Mix_VolumeMusic(*musicVolume);
                        *isMuted = 0;
                    // Change selected mode to the right
                    } else if (event->key.keysym.sym == SDLK_RIGHT && *previousState != STATE_PAUSE) {
                        *selectedMode = (*selectedMode + 1) % 3;
                    // Change selected mode to the left
                    } else if (event->key.keysym.sym == SDLK_LEFT && *previousState != STATE_PAUSE) {
                        *selectedMode = (*selectedMode - 1 + 3) % 3;
                    // Return to previous state
                    } else if (event->key.keysym.sym == SDLK_ESCAPE) {
                        *currentState = *previousState;
                    }
                    break;
                 // Handle name input screen
                case STATE_PSEUDO:
                    // Remove last character if backspace is pressed
                    if (event->key.keysym.sym == SDLK_BACKSPACE && *nameLength > 0) {
                        playerName[--(*nameLength)] = '\0';
                    // Confirm name and start the game
                    } else if (event->key.keysym.sym == SDLK_RETURN && *nameLength > 0) {
                        *currentState = STATE_GAME;
                        if (Mix_PausedMusic()) {
                            Mix_ResumeMusic();
                        } else {
                            Mix_PlayMusic(bgm, -1);
                        }
                        // Load pieces and spawn a new one
                        load_pieces_from_file("assets/pieces.txt", allPieces);
                        spawn_random_piece_from_list(lockGrid, currentPiece, allPieces, MAX_PIECES);
                        SDL_StopTextInput();
                    }
                    break;
                // In-game controls
                case STATE_GAME:
                    // Move piece left
                    if (event->key.keysym.sym == SDLK_LEFT)
                        move_piece_left(currentPiece, lockGrid);
                    // Move piece right
                    if (event->key.keysym.sym == SDLK_RIGHT)
                        move_piece_right(currentPiece, lockGrid);
                     // Move piece right, check for locking and line clearing
                    if (event->key.keysym.sym == SDLK_DOWN) {
                        if (!move_piece_down(currentPiece, lockGrid)) {
                            lock_piece(currentPiece, lockGrid);
                            clear_full_lines(lockGrid, score);
                            spawn_random_piece_from_list(lockGrid, currentPiece, allPieces, MAX_PIECES);
                            if (is_game_over(lockGrid, currentPiece)) {
                                *currentState = STATE_GAMEOVER;
                                Mix_PauseMusic();
                            }
                        }
                    // Rotate piece clockwise
                    } else if (event->key.keysym.sym == SDLK_a) {
                        Piece temp = *currentPiece;
                        rotate_piece(&temp, 90);
                        if (!check_collision(&temp, lockGrid, temp.offset_x, temp.offset_y)) {
                            *currentPiece = temp;
                        }
                    // Rotate piece counter-clockwise
                    } else if (event->key.keysym.sym == SDLK_s) {
                        Piece temp = *currentPiece;
                        rotate_piece(&temp, 270);
                        if (!check_collision(&temp, lockGrid, temp.offset_x, temp.offset_y)) {
                            *currentPiece = temp;
                        }
                    // Pause the game
                    } else if (event->key.keysym.sym == SDLK_ESCAPE) {
                        *currentState = STATE_PAUSE;
                        Mix_PauseMusic();
                    // Drop piece immediately
                    } else if (event->key.keysym.sym == SDLK_SPACE) {
                        while (move_piece_down(currentPiece, lockGrid)) {}
                        lock_piece(currentPiece, lockGrid);
                        clear_full_lines(lockGrid, score);
                        spawn_random_piece_from_list(lockGrid, currentPiece, allPieces, MAX_PIECES);
                        if (is_game_over(lockGrid, currentPiece)) {
                            *currentState = STATE_GAMEOVER;
                            Mix_PauseMusic();
                        }
                    }
                    break;

                // Confirmation before exiting: save score if 'y' or discard if 'n'
                case STATE_EXIT:
                    if (event->key.keysym.sym == SDLK_y) {
                        *currentState = *futurState;
                        *previousState = *futurState;
                        save_scores_to_file(playerName, *score, "assets/score.txt");
                        *score = 0;
                        playerName[0] = '\0';
                        *nameLength = 0;
                        clear_grid(lockGrid);
                        if (*futurState == STATE_PSEUDO) {
                            SDL_StartTextInput();
                        }
                    } else if (event->key.keysym.sym == SDLK_n) {
                        *currentState = *futurState;
                        *previousState = *futurState;
                        *score = 0;
                        playerName[0] = '\0';
                        *nameLength = 0;
                        clear_grid(lockGrid);
                        if (*futurState == STATE_PSEUDO) {
                            SDL_StartTextInput();
                        }
                    }
                    break;
            }
            break;
        
        // Handle mouse click events
        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&mx, &my);
            if (event->button.button == SDL_BUTTON_LEFT) {
                switch (*currentState) {
                    
                    // Main menu buttons
                    case STATE_MENU:
                        if (isMouseOver(mainMenubtn[0], mx, my)) {
                            *currentState = STATE_PSEUDO;
                            playerName[0] = '\0';
                            *nameLength = 0;
                            SDL_StartTextInput();
                        } else if (isMouseOver(mainMenubtn[1], mx, my)) {
                            *currentState = STATE_OPTIONS;
                            *previousState = STATE_MENU;
                        } else if (isMouseOver(mainMenubtn[2], mx, my)) {
                            *running = false;
                        }
                        break;
                    
                    // Pause menu buttons
                    case STATE_PAUSE:
                        if (isMouseOver(pauseMenubtn[0], mx, my)) {
                            *currentState = STATE_EXIT;
                            *futurState = STATE_MENU;
                        } else if (isMouseOver(pauseMenubtn[1], mx, my)) {
                            *currentState = STATE_OPTIONS;
                            *previousState = STATE_PAUSE;
                        } else if (isMouseOver(pauseMenubtn[2], mx, my)) {
                            *currentState = STATE_GAME;
                            Mix_ResumeMusic();
                        }
                        break;

                    // Game over menu buttons
                    case STATE_GAMEOVER:
                        if (isMouseOver(gameOverbtn[0], mx, my)) {
                            *currentState = STATE_EXIT;
                            *futurState = STATE_PSEUDO;
                        } else if (isMouseOver(gameOverbtn[1], mx, my)) {
                            *currentState = STATE_OPTIONS;
                            *previousState = STATE_GAMEOVER;
                        } else if (isMouseOver(gameOverbtn[2], mx, my)) {
                            *currentState = STATE_EXIT;
                            *futurState = STATE_MENU;
                        }
                        break;
                    default:
                        break;
                }
            }
            break;

        // Handle mouse wheel: adjust volume in options
        case SDL_MOUSEWHEEL:
            if (*currentState == STATE_OPTIONS) {
                if (event->wheel.y > 0 && *musicVolume < 128)
                    *musicVolume += 8;
                else if (event->wheel.y < 0 && *musicVolume > 0)
                    *musicVolume -= 8;

                Mix_VolumeMusic(*musicVolume);
            }
            break;

        // Handle text input: add characters to player name
        case SDL_TEXTINPUT:
            if (*currentState == STATE_PSEUDO && *nameLength < 49) {
                strcat(playerName, event->text.text);
                (*nameLength)++;
            }
            break;

        // Default case: ignore other events
        default:
            break;
    }
}
