#ifndef RENDER_H
#define RENDER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "pieces.h"
#include "game.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define BLOCK_SIZE 40
#define GRID_WIDTH 10
#define GRID_HEIGHT 10




void renderMenu(SDL_Renderer* renderer, SDL_Texture* bgTexture, SDL_Rect buttons[]);

void renderOptions(SDL_Renderer* renderer, TTF_Font* font, int selectedMode, int musicVolume, int isMuted, const char* modesText[]);

void renderPseudo(SDL_Renderer* renderer, TTF_Font* font, char playerName[]);

void renderGame(SDL_Renderer* renderer, TTF_Font* font, Grid* grid, Piece* currentPiece, int score);

void renderPause(SDL_Renderer* renderer, SDL_Texture* bgTexturePause, SDL_Rect buttons[]);

void renderExit(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* bgTexturesave);

void renderGameover(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* bgTexturegameover, int score, SDL_Rect buttons[]);


#endif
