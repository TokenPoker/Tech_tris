#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "game.h"
#include "pieces.h"
#include "score.h"


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRID_WIDTH 10
#define GRID_HEIGHT 10
#define BLOCK_SIZE 40
#define MAX_PIECES 6


Grid lockGrid = { .width = GRID_WIDTH, .height = GRID_HEIGHT };
Piece currentPiece;
Piece allPieces[MAX_PIECES];

int score = 0;
typedef enum {
    STATE_MENU,
    STATE_OPTIONS,
    STATE_GAME,
    STATE_PSEUDO,
    STATE_PAUSE,
    STATE_GAMEOVER,
    STATE_EXIT
} GameState;


/*

function to check if the mouse is over a given rectangle
entry:
    - rect: SDL_Rect structure representing the button area
    - x: x-coordinate of the mouse
    - y: y-coordinate of the mouse
exit:
    - true if the mouse is over the rectangle, false otherwise

*/
bool isMouseOver(SDL_Rect rect, int x, int y) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}
/*

function to render text using SDL_ttf

entry: 
    - renderer: SDL_Renderer pointer
    - font: TTF_Font pointer
    - text: text to render
    - color: color of the text
exit:
    - SDL_Texture pointer to the rendered text texture
*/

SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

/*

function to render the game grid and UI
entry:
    - renderer: SDL_Renderer pointer
    - font: TTF_Font pointer
    - blockTexture: SDL_Texture pointer for the block texture
    - grid: Grid pointer to the game grid
    - score: current score
exit:
    - none

*/
void renderGame(SDL_Renderer* renderer, TTF_Font* font,  Grid* grid, int score) {
    // Set background color (dark blue) and clear the screen

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderClear(renderer);

    insert_piece_in_grid(&currentPiece, &lockGrid); // Insert the current piece into the grid

    // Draw all blocks in the game grid
    for (int i = 0; i < GRID_HEIGHT+2; i++) {
        for (int j = 0; j < GRID_WIDTH+2; j++) {
            // Draw the grid borders
            if(i == 0 || i == GRID_HEIGHT+1 || j == 0 || j == GRID_WIDTH+1){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // black color
                SDL_Rect borderRect = {
                    j * BLOCK_SIZE + 401, // x-position (add offset for centering)
                    i * BLOCK_SIZE + 121 , // y-position (add offset for top margin)
                    BLOCK_SIZE-2,          // width
                    BLOCK_SIZE-2           // height
                };
                SDL_RenderFillRect(renderer, &borderRect); // draw the block
            }else if (grid->shape[i-1][j-1] == '1') {
                SDL_SetRenderDrawColor(renderer, 40, 40,40, 255); // black color
                SDL_Rect blockRect = {
                    j * BLOCK_SIZE + 401, // x-position (add offset for centering)
                    i * BLOCK_SIZE + 121, // y-position (add offset for top margin)
                    BLOCK_SIZE-2,          // width
                    BLOCK_SIZE-2           // height
                };
                SDL_RenderFillRect(renderer, &blockRect); // draw the block
            }
        }
    }
    remove_piece_from_grid(&currentPiece, &lockGrid); // Remove the current piece from the grid

    // Draw a grey panel on the right side of the screen for UI
    SDL_Rect panelRect = { SCREEN_WIDTH - 250, 0, 250, SCREEN_HEIGHT };
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // grey color
    SDL_RenderFillRect(renderer, &panelRect);

    // Display the score
    char scoreText[50];
    sprintf(scoreText, "Score: %d", score); // format the score text
    SDL_Texture* scoreTexture = renderText(renderer, font, scoreText, (SDL_Color){255, 255, 255, 255}); // white text
    int texW = 0, texH = 0;
    SDL_QueryTexture(scoreTexture, NULL, NULL, &texW, &texH); // get size of the texture
    SDL_Rect scoreRect = { SCREEN_WIDTH - 220, 50, texW, texH }; // position the score
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect); // draw the score
    SDL_DestroyTexture(scoreTexture); // free texture memory

    // Display "Controls:" label
    SDL_Texture* helpTexture = renderText(renderer, font, "Controls:", (SDL_Color){200, 200, 200, 255}); // light grey text
    SDL_QueryTexture(helpTexture, NULL, NULL, &texW, &texH);
    SDL_Rect helpRect = { SCREEN_WIDTH - 220, 150, texW, texH };
    SDL_RenderCopy(renderer, helpTexture, NULL, &helpRect);
    SDL_DestroyTexture(helpTexture);

    // Display control instructions one by one
    SDL_Texture* controls[4];
    const char* controlsText[] = {
        "Arrows = Move",
        "A/S = Rotate",
        "Esc = Pause",
        "Space = Drop"
    };

    for (int i = 0; i < 4; i++) {
        controls[i] = renderText(renderer, font, controlsText[i], (SDL_Color){180, 180, 180, 255}); // slightly darker grey
        SDL_QueryTexture(controls[i], NULL, NULL, &texW, &texH);
        SDL_Rect ctrlRect = {
            SCREEN_WIDTH - 220,     // x-position
            200 + i * 40,           // y-position (with spacing)
            texW, texH
        };
        SDL_RenderCopy(renderer, controls[i], NULL, &ctrlRect);
        SDL_DestroyTexture(controls[i]); // free texture after drawing
    }
}


int main() {
    if(SDL_Init(SDL_INIT_VIDEO| SDL_INIT_AUDIO)!=0){
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Tech-tris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer) {
        printf("Failed to create window or renderer: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }



    // Load textures and font
    
    TTF_Font* font = TTF_OpenFont("font.ttf", 28);
    SDL_Texture* bgTextureMenu = IMG_LoadTexture(renderer, "fond_menu.png");
    SDL_Texture* bgTexturePause = IMG_LoadTexture(renderer, "Menu_pause.png");
    SDL_Texture* bgTexturesave = IMG_LoadTexture(renderer, "fond_save.png");

    if (!bgTextureMenu || !bgTexturePause || !bgTexturesave) {
        printf("Failed to load background: %s\n", IMG_GetError());
        return 1;
    }


    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
       
    }


    // Define interactive button areas
    SDL_Rect startBtn = { 480, 450, 320, 50 };
    SDL_Rect optionsBtn = { 480, 510, 320, 50 };
    SDL_Rect exitBtn = { 480, 570, 320, 50 };
    SDL_Rect menuButton = { 460, 450, 250, 90 };     // x, y, width, height
    SDL_Rect optionsButton = { 415, 330, 345, 70 };
    SDL_Rect resumeButton = { 370, 185, 430 , 90 };
    Mix_Music* bgm = Mix_LoadMUS("Tetris Theme Music.mp3");
    int musicVolume = 64; // Valeur initiale (50%)
    Mix_VolumeMusic(musicVolume);
    int isMuted = false;

    if (!bgm) {
        printf("Failed to load music: %s\n", Mix_GetError());
    }


    const char* modesText[3] = { "CLASSIC", "HARD", "ZEN" };
    Uint32 LastdropTime = 0;
    char playerName[50] = "";
    int nameLength = 0;
    int selectedMode = 0;
    GameState currentState = STATE_MENU;
    GameState previousState = STATE_MENU;
    
    bool running = true;
    SDL_Event event;


    while (running){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                running = false;
            }else if (event.type == SDL_KEYDOWN) {
                switch (currentState) {
                    
                    case STATE_MENU:
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            running = false;
                        }
                    
                        break;
                    case STATE_OPTIONS:
                        if (event.key.keysym.sym == SDLK_UP && musicVolume < 128) {
                            musicVolume += 8; // Increase volume
                            Mix_VolumeMusic(musicVolume);
                            isMuted = 0;

                        } else if (event.key.keysym.sym == SDLK_DOWN && musicVolume > 0) {
                            musicVolume -= 8; // Decrease volume
                            Mix_VolumeMusic(musicVolume);
                            isMuted = 0;

                        }else if(event.key.keysym.sym == SDLK_m){
                            Mix_VolumeMusic(0);
                            isMuted = 1;

                        }else if(event.key.keysym.sym == SDLK_p){
                            Mix_VolumeMusic(musicVolume);
                            isMuted = 0;

                        }else if(event.key.keysym.sym == SDLK_RIGHT && previousState != STATE_PAUSE){
                            selectedMode = (selectedMode + 1) % 3; // Cycle through modes

                        }else if(event.key.keysym.sym == SDLK_LEFT && previousState != STATE_PAUSE){
                            selectedMode = (selectedMode - 1 + 3) % 3; // Cycle through modes

                        }else if (event.key.keysym.sym == SDLK_ESCAPE){
                            currentState = previousState;

                        }
                        break;
                    case STATE_PSEUDO:
                        if (event.key.keysym.sym == SDLK_BACKSPACE && nameLength > 0) {
                            playerName[--nameLength] = '\0';
                        } else if (event.key.keysym.sym == SDLK_RETURN && nameLength > 0) {
                            currentState = STATE_GAME;
                            if(Mix_PausedMusic() == 1){
                                Mix_ResumeMusic();
                            }else{
                                Mix_PlayMusic(bgm, -1);
                            }
                            
                            load_pieces_from_file("pieces.txt", allPieces);
                            spawn_random_piece_from_list(&lockGrid, &currentPiece, allPieces, MAX_PIECES);
                            SDL_StopTextInput();
                        }
                        break;
                    case STATE_GAME:
                        if (event.key.keysym.sym == SDLK_LEFT)
                            move_piece_left(&currentPiece, &lockGrid);
                        if (event.key.keysym.sym == SDLK_RIGHT)
                            move_piece_right(&currentPiece, &lockGrid);
                        if (event.key.keysym.sym == SDLK_DOWN) {
                            if (!move_piece_down(&currentPiece, &lockGrid)) {
                                lock_piece(&currentPiece, &lockGrid);
                                clear_full_lines(&lockGrid, &score);
                                
                                spawn_random_piece_from_list(&lockGrid, &currentPiece, allPieces, MAX_PIECES);
                            }
                        }else if (event.key.keysym.sym == SDLK_a){
                            rotate_piece(&currentPiece, 90 );
                        }else if (event.key.keysym.sym == SDLK_s){
                            rotate_piece(&currentPiece, -90);
                        }else if (event.key.keysym.sym == SDLK_ESCAPE){ 
                            currentState = STATE_PAUSE;
                            Mix_PauseMusic();
                        }else if(event.key.keysym.sym == SDLK_SPACE){
                            // Drop the piece immediately
                            while (move_piece_down(&currentPiece, &lockGrid)) {
                                // Keep moving down until it can't anymore
                            }
                            lock_piece(&currentPiece, &lockGrid);
                            clear_full_lines(&lockGrid, &score);
                            spawn_random_piece_from_list(&lockGrid, &currentPiece, allPieces, MAX_PIECES);
                        }
                        break;
                    case STATE_EXIT:
                        if(event.key.keysym.sym == SDLK_y){
                            currentState = STATE_MENU;
                            previousState = STATE_MENU;
                            save_scores_to_file(playerName, score, "score.txt");
                            score = 0;
                            playerName[0] = '\0';
                            nameLength = 0;

                        }else if(event.key.keysym.sym == SDLK_n){
                        
                            currentState = STATE_MENU;
                            previousState = STATE_MENU;
                            score = 0;
                            playerName[0] = '\0';
                            nameLength = 0;
                        }
                        break;
                    }
                }else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;
                
                // Check if the mouse is over any button
            
                if (currentState == STATE_MENU) {
                    if (isMouseOver(startBtn, mx, my)) {
                        currentState = STATE_PSEUDO;
                        playerName[0] = '\0';
                        nameLength = 0;
                        SDL_StartTextInput();
                    } else if (isMouseOver(optionsBtn, mx, my)) {
                        currentState = STATE_OPTIONS;
                        previousState = STATE_MENU;
                    } else if (isMouseOver(exitBtn, mx, my)) {
                        running = false;
                    }
                }else if(currentState == STATE_PAUSE){
                    if (isMouseOver(menuButton, mx, my)) {
                        currentState = STATE_EXIT;
                    } else if (isMouseOver(optionsButton, mx, my)) {
                        currentState = STATE_OPTIONS;
                        previousState = STATE_PAUSE;
                    }else if(isMouseOver(resumeButton, mx, my)){
                        currentState = STATE_GAME;
                        Mix_ResumeMusic();
                    }
                }
            }else if(event.type == SDL_MOUSEWHEEL){
                if (currentState == STATE_OPTIONS) {
                    if (event.wheel.y > 0 && musicVolume < 128) {
                        musicVolume += 8;
                    } else if (event.wheel.y < 0 && musicVolume > 0) {
                        musicVolume -= 8;
                    }
                    Mix_VolumeMusic(musicVolume);
                }
            }else if (event.type == SDL_TEXTINPUT && currentState == STATE_PSEUDO) {
                if (nameLength < (int)(sizeof(playerName) - 1)) {
                    strcat(playerName, event.text.text);
                    nameLength++;
                }
            }

        }

        if (currentState == STATE_GAME && !auto_drop_piece(&currentPiece, &lockGrid, selectedMode, &LastdropTime)) {
            lock_piece(&currentPiece, &lockGrid);
            clear_full_lines(&lockGrid, &score);
            spawn_random_piece_from_list(&lockGrid, &currentPiece, allPieces, MAX_PIECES);
        }
        // RENDERING
        if (currentState == STATE_MENU) {

                
            // Draw background image
            SDL_RenderCopy(renderer, bgTextureMenu, NULL, NULL); 

            // Get current mouse position
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);


            // Highlight buttons on hover
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow outline
            if (isMouseOver(startBtn, mouseX, mouseY)){
                SDL_RenderDrawRect(renderer, &startBtn);
            }
            if (isMouseOver(optionsBtn, mouseX, mouseY)){
                SDL_RenderDrawRect(renderer, &optionsBtn);
            }
            if (isMouseOver(exitBtn, mouseX, mouseY)){
                SDL_RenderDrawRect(renderer, &exitBtn);
            }





        } else if (currentState == STATE_OPTIONS) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255);
            SDL_RenderClear(renderer);

            const char* labels[] = {
                "Mode:",
                modesText[selectedMode],
                "→ / ← to change mode",
                "",
                "Volume: ↑ (up), ↓ (down)",
                "Mute: M, Power: P"
            };

            for (int i = 0; i < 6; ++i) {
                SDL_Color color = (i == 1) ? (SDL_Color){255, 255, 0, 255} : (SDL_Color){255, 255, 255, 255};
                SDL_Texture* text = renderText(renderer, font, labels[i], color);
                int texW = 0, texH = 0;
                SDL_QueryTexture(text, NULL, NULL, &texW, &texH);
                SDL_Rect dst = { SCREEN_WIDTH / 2 - texW / 2, 150 + i * 50, texW, texH };
                SDL_RenderCopy(renderer, text, NULL, &dst);
                SDL_DestroyTexture(text);
            }

            // Draw the volume bar
            SDL_Rect volumeBar = { SCREEN_WIDTH / 2 - 200, 500, 400, 20 };
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &volumeBar);
            SDL_SetRenderDrawColor(renderer, 255*isMuted, 255*(1-isMuted), 0, 255);
            SDL_Rect filledBar = { SCREEN_WIDTH / 2 - 200, 500, (int)(400 * (musicVolume / 128.0f)), 20 };
            SDL_RenderFillRect(renderer, &filledBar);

        }else if (currentState == STATE_PSEUDO) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255);
            SDL_RenderClear(renderer);
        
            SDL_Texture* prompt = renderText(renderer, font, "Enter your name:", (SDL_Color){255, 255, 255, 255});
            SDL_Texture* nameText = renderText(renderer, font, playerName, (SDL_Color){255, 255, 0, 255});
        
            int pw, ph, nw, nh;
            SDL_QueryTexture(prompt, NULL, NULL, &pw, &ph);
            SDL_QueryTexture(nameText, NULL, NULL, &nw, &nh);
        
            SDL_Rect promptRect = { SCREEN_WIDTH / 2 - pw / 2, 200, pw, ph };
            SDL_Rect nameRect = { SCREEN_WIDTH / 2 - nw / 2, 300, nw, nh };
        
            SDL_RenderCopy(renderer, prompt, NULL, &promptRect);
            SDL_RenderCopy(renderer, nameText, NULL, &nameRect);
        
            SDL_DestroyTexture(prompt);
            SDL_DestroyTexture(nameText);
        }else if (currentState == STATE_GAME) {
            renderGame(renderer, font, &lockGrid, score);
        }else if (currentState == STATE_PAUSE) {

            // Draw background image
            SDL_RenderCopy(renderer, bgTexturePause, NULL, NULL); 

            // Get current mouse position
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);


            // Highlight buttons on hover
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow outline
            if (isMouseOver(menuButton, mouseX, mouseY)){
                SDL_RenderDrawRect(renderer, &menuButton);
            }
            if (isMouseOver(optionsButton, mouseX, mouseY)){
                SDL_RenderDrawRect(renderer, &optionsButton);
            }
            if (isMouseOver(resumeButton, mouseX, mouseY)){
                SDL_RenderDrawRect(renderer, &resumeButton);
            }


        }else if(currentState == STATE_EXIT){
            
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, bgTexturesave, NULL, NULL); 
        
            // Main question text
            SDL_Texture* question = renderText(renderer, font, "Do you want to save?", (SDL_Color){255, 255, 255, 255});
            SDL_Texture* options = renderText(renderer, font, "Y. Yes      N. No", (SDL_Color){255, 255, 0, 255});
        
            int qw, qh, ow, oh;
            SDL_QueryTexture(question, NULL, NULL, &qw, &qh);
            SDL_QueryTexture(options, NULL, NULL, &ow, &oh);
        
            SDL_Rect questionRect = { SCREEN_WIDTH / 2 - qw / 2, SCREEN_HEIGHT / 2 - 60, qw, qh };
            SDL_Rect optionsRect = { SCREEN_WIDTH / 2 - ow / 2, SCREEN_HEIGHT / 2, ow, oh };
        
            SDL_RenderCopy(renderer, question, NULL, &questionRect);
            SDL_RenderCopy(renderer, options, NULL, &optionsRect);
        
            SDL_DestroyTexture(question);
            SDL_DestroyTexture(options);
        
           
        }

        SDL_RenderPresent(renderer);
    }


    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}