#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include "game.h"
#include "pieces.h"sudo apt update
#include "score.h"


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define GRID_WIDTH 10
#define GRID_HEIGHT 20
#define BLOCK_SIZE 25
#define MAX_PIECES 6

Grid grid = { .width = GRID_WIDTH, .height = GRID_HEIGHT };
Piece currentPiece;
Piece allPieces[MAX_PIECES];

int score = 0;
typedef enum {
    STATE_MENU,
    STATE_OPTIONS,
    STATE_GAME
} GameState;

// Function to render text easily
SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Function to render the game grid + score
void renderGame(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* blockTexture, Grid* grid, int score) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (grid->shape[i][j] == '1') {
                SDL_Rect blockRect = { j * BLOCK_SIZE + 50, i * BLOCK_SIZE + 20, BLOCK_SIZE, BLOCK_SIZE };
                SDL_RenderCopy(renderer, blockTexture, NULL, &blockRect);
            }
        }
    }

    SDL_Rect panelRect = { SCREEN_WIDTH - 250, 0, 250, SCREEN_HEIGHT };
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &panelRect);

    char scoreText[50];
    sprintf(scoreText, "Score: %d", score);
    SDL_Texture* scoreTexture = renderText(renderer, font, scoreText, (SDL_Color){255, 255, 255, 255});
    int texW = 0, texH = 0;
    SDL_QueryTexture(scoreTexture, NULL, NULL, &texW, &texH);
    SDL_Rect scoreRect = { SCREEN_WIDTH - 220, 50, texW, texH };
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
    SDL_DestroyTexture(scoreTexture);

    SDL_Texture* helpTexture = renderText(renderer, font, "Controls:", (SDL_Color){200, 200, 200, 255});
    SDL_QueryTexture(helpTexture, NULL, NULL, &texW, &texH);
    SDL_Rect helpRect = { SCREEN_WIDTH - 220, 150, texW, texH };
    SDL_RenderCopy(renderer, helpTexture, NULL, &helpRect);
    SDL_DestroyTexture(helpTexture);

    SDL_Texture* controls[4];
    const char* controlsText[] = { "Arrows = Move", "A/S = Rotate", "Space = Pause", "Enter = Drop" };

    for (int i = 0; i < 4; i++) {
        controls[i] = renderText(renderer, font, controlsText[i], (SDL_Color){180, 180, 180, 255});
        SDL_QueryTexture(controls[i], NULL, NULL, &texW, &texH);
        SDL_Rect ctrlRect = { SCREEN_WIDTH - 220, 200 + i * 40, texW, texH };
        SDL_RenderCopy(renderer, controls[i], NULL, &ctrlRect);
        SDL_DestroyTexture(controls[i]);
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Tetris Redux", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* logoTexture = IMG_LoadTexture(renderer, "logo.png");
    SDL_Texture* blockTexture = IMG_LoadTexture(renderer, "3ba76cac-81f6-49c9-90c4-167388ef3cd2.png");
    TTF_Font* font = TTF_OpenFont("font.ttf", 28);

    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return 1;
    }

    const char* menuText[3] = { "START", "OPTIONS", "EXIT" };
    const char* modesText[3] = { "CLASSIC", "LA PLS", "CHILL" };

    int selectedItem = 0;
    int selectedMode = 0;
    GameState currentState = STATE_MENU;

    bool running = true;
    SDL_Event event;
    SDL_Rect logoRect = { 150, 20, 500, 200 };

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            else if (event.type == SDL_KEYDOWN) {
                switch (currentState) {
                    case STATE_MENU:
                        if (event.key.keysym.sym == SDLK_UP)
                            selectedItem = (selectedItem - 1 + 3) % 3;
                        else if (event.key.keysym.sym == SDLK_DOWN)
                            selectedItem = (selectedItem + 1) % 3;
                        else if (event.key.keysym.sym == SDLK_RETURN) {
                            if (selectedItem == 0) {
                                currentState = STATE_GAME;
                                load_pieces_from_file("pieces.txt", allPieces);
                                spawn_random_piece_from_list(&grid, &currentPiece, allPieces, MAX_PIECES);
                            } else if (selectedItem == 1) {
                                currentState = STATE_OPTIONS;
                            } else if (selectedItem == 2) {
                                running = false;
                            }
                        }
                        break;
                    case STATE_OPTIONS:
                        if (event.key.keysym.sym == SDLK_UP)
                            selectedMode = (selectedMode - 1 + 3) % 3;
                        else if (event.key.keysym.sym == SDLK_DOWN)
                            selectedMode = (selectedMode + 1) % 3;
                        else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_ESCAPE)
                            currentState = STATE_MENU;
                        break;
                    case STATE_GAME:
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            currentState = STATE_MENU;
                        }
                        if (event.key.keysym.sym == SDLK_LEFT)
                            move_piece_left(&currentPiece, &grid);
                        if (event.key.keysym.sym == SDLK_RIGHT)
                            move_piece_right(&currentPiece, &grid);
                        if (event.key.keysym.sym == SDLK_DOWN) {
                            if (!move_piece_down(&currentPiece, &grid)) {
                                lock_piece(&currentPiece, &grid);
                                clear_full_lines(&grid);
                                spawn_random_piece_from_list(&grid, &currentPiece, allPieces, MAX_PIECES);
                            }
                        }
                        if (event.key.keysym.sym == SDLK_a)
                            rotate_piece(&currentPiece, 90 );
                        break;
                        
                }
            }
        }

        // RENDERING
        if (currentState == STATE_MENU) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);
            for (int i = 0; i < 3; ++i) {
                SDL_Color color = (i == selectedItem) ? (SDL_Color){255, 255, 0, 255} : (SDL_Color){255, 255, 255, 255};
                SDL_Texture* textTexture = renderText(renderer, font, menuText[i], color);
                int texW = 0, texH = 0;
                SDL_QueryTexture(textTexture, NULL, NULL, &texW, &texH);
                SDL_Rect dstRect = { SCREEN_WIDTH / 2 - texW / 2, 300 + i * 60, texW, texH };
                SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);
                SDL_DestroyTexture(textTexture);
            }
        } else if (currentState == STATE_OPTIONS) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255);
            SDL_RenderClear(renderer);
            for (int i = 0; i < 3; ++i) {
                SDL_Color color = (i == selectedMode) ? (SDL_Color){0, 255, 255, 255} : (SDL_Color){255, 255, 255, 255};
                SDL_Texture* modeTexture = renderText(renderer, font, modesText[i], color);
                int texW = 0, texH = 0;
                SDL_QueryTexture(modeTexture, NULL, NULL, &texW, &texH);
                SDL_Rect dstRect = { SCREEN_WIDTH / 2 - texW / 2, 250 + i * 60, texW, texH };
                SDL_RenderCopy(renderer, modeTexture, NULL, &dstRect);
                SDL_DestroyTexture(modeTexture);
            }
        } else if (currentState == STATE_GAME) {
            renderGame(renderer, font, blockTexture, &grid, score);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(logoTexture);
    SDL_DestroyTexture(blockTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}