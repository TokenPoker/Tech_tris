#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


#include "score.h"
#include "key.h"
#include "render.h"
#include "game.h"
#include "pieces.h"

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
    
    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 28);
    SDL_Texture* bgTextureMenu = IMG_LoadTexture(renderer, "assets/fond_menu.png");
    SDL_Texture* bgTexturePause = IMG_LoadTexture(renderer, "assets/Menu_pause.png");
    SDL_Texture* bgTexturesave = IMG_LoadTexture(renderer, "assets/fond_save.png");
    SDL_Texture* bgTexturegameover = IMG_LoadTexture(renderer, "assets/front_Gameover.png");

    if (!bgTextureMenu || !bgTexturePause || !bgTexturesave || !bgTexturegameover) {
        printf("Failed to load background: %s\n", IMG_GetError());
        return 1;
    }


    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
       
    }


    // Define interactive button areas
    SDL_Rect mainMenuButtons[] = {
        {480, 450, 320, 50}, // startBtn
        {480, 510, 320, 50}, // optionsBtn
        {480, 570, 320, 50}  // exitBtn
    };
    
    SDL_Rect pauseMenuButtons[] = {
        {460, 450, 250, 90}, // menuButton
        {415, 330, 345, 70}, // optionsButton
        {370, 185, 430, 90}  // resumeButton
    };
    
    SDL_Rect gameOverButtons[] = {
        {400, 580, 160, 60}, // replayButton
        {580, 580, 160, 60}, // options2Button
        {770, 580, 160, 60}  // exit2Button
    };
    
    Mix_Music* bgm = Mix_LoadMUS("assets/Tetris Theme Music.mp3");
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
    GameState futurState = STATE_MENU;
    
    bool running = true;
    SDL_Event event;


    while (running){
        while (SDL_PollEvent(&event)){
            handleEvents(&event, &currentState, &previousState, &futurState, &currentPiece, &lockGrid, allPieces, &score, playerName, &nameLength, &musicVolume, &isMuted, &selectedMode, bgm, &running, mainMenuButtons, pauseMenuButtons, gameOverButtons);
        }
        if (currentState == STATE_GAME && !auto_drop_piece(&currentPiece, &lockGrid, selectedMode, &LastdropTime)) {
            lock_piece(&currentPiece, &lockGrid);
            clear_full_lines(&lockGrid, &score);
            spawn_random_piece_from_list(&lockGrid, &currentPiece, allPieces, MAX_PIECES);
            if (is_game_over(&lockGrid, &currentPiece)) {
                currentState = STATE_GAMEOVER;
                Mix_PauseMusic(); // facultatif
            }
        }
        // RENDERING
        switch (currentState) {
            case STATE_MENU:
                renderMenu(renderer, bgTextureMenu, mainMenuButtons);
                break;
            case STATE_OPTIONS:
                renderOptions(renderer, font, selectedMode, musicVolume, isMuted, modesText);
                break;
            case STATE_PSEUDO:
                renderPseudo(renderer, font, playerName);
                break;
            case STATE_GAME:
                renderGame(renderer, font, &lockGrid, &currentPiece, score);
                break;
            case STATE_PAUSE:
                renderPause(renderer, bgTexturePause, pauseMenuButtons);
                break;
            case STATE_EXIT:
                renderExit(renderer, font, bgTexturesave);
                break;
            case STATE_GAMEOVER:
                renderGameover(renderer, font, bgTexturegameover, score, gameOverButtons);
                break;
            default:
                break;
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