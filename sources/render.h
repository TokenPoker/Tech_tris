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

/**
 * Renders the main menu, including background and buttons.
 *
 * Parameters:
 *   - renderer   : Pointer to the SDL_Renderer used for rendering.
 *   - bgTexture : Pointer to the SDL_Texture for the background image.
 *   - buttons    : Array of SDL_Rects representing the clickable menu buttons.
 *
 * Returns: void.
 */
void renderMenu(SDL_Renderer* renderer, SDL_Texture* bgTexture, SDL_Rect buttons[]);

/**
 * Renders the options menu, including the background, mode selection, and volume controls.
 *
 * Parameters:
 *   - renderer    : Pointer to the SDL_Renderer used for rendering.
 *   - font        : Pointer to the TTF_Font used to render text.
 *   - selectedMode: Integer representing the currently selected game mode.
 *   - musicVolume : Integer representing the current music volume (0–128).
 *   - isMuted     : Integer flag indicating if music is muted (1 = muted, 0 = not).
 *   - modesText   : Array of strings representing the text for each mode.
 *
 * Returns: void.
 */
void renderOptions(SDL_Renderer* renderer, TTF_Font* font, int selectedMode, int musicVolume, int isMuted, const char* modesText[]);

/**
 * Renders the player name input screen, allowing the player to enter their name.
 *
 * Parameters:
 *   - renderer   : Pointer to the SDL_Renderer used for rendering.
 *   - font       : Pointer to the TTF_Font used to render the player name.
 *   - playerName : Character array holding the player’s name to be displayed.
 *
 * Returns: void.
 */
void renderPseudo(SDL_Renderer* renderer, TTF_Font* font, char playerName[]);

/**
 * Renders the game state, including the grid and the current piece.
 *
 * Parameters:
 *   - renderer    : Pointer to the SDL_Renderer used for rendering.
 *   - font        : Pointer to the TTF_Font used to render text (for the score).
 *   - grid        : Pointer to the Grid structure that holds the locked pieces.
 *   - currentPiece: Pointer to the current piece being controlled by the player.
 *   - score       : Integer representing the current player’s score.
 *
 * Returns: void.
 */
void renderGame(SDL_Renderer* renderer, TTF_Font* font, Grid* grid, Piece* currentPiece, int score);

/**
 * Renders the pause menu, including the background and pause options.
 *
 * Parameters:
 *   - renderer      : Pointer to the SDL_Renderer used for rendering.
 *   - bgTexturePause: Pointer to the SDL_Texture for the pause background image.
 *   - buttons       : Array of SDL_Rects representing the clickable pause menu buttons.
 *
 * Returns: void.
 */
void renderPause(SDL_Renderer* renderer, SDL_Texture* bgTexturePause, SDL_Rect buttons[]);

/**
 * Renders the exit screen, allowing the player to confirm or cancel exiting the game.
 *
 * Parameters:
 *   - renderer        : Pointer to the SDL_Renderer used for rendering.
 *   - font            : Pointer to the TTF_Font used to render text.
 *   - bgTexturesave   : Pointer to the SDL_Texture for the background image when saving.
 *
 * Returns: void.
 */
void renderExit(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* bgTexturesave);

/**
 * Renders the game over screen, displaying the final score and options for the player.
 *
 * Parameters:
 *   - renderer         : Pointer to the SDL_Renderer used for rendering.
 *   - font             : Pointer to the TTF_Font used to render the score and text.
 *   - bgTexturegameover: Pointer to the SDL_Texture for the game over background.
 *   - score            : Integer representing the final score.
 *   - buttons          : Array of SDL_Rects representing the clickable game over buttons.
 *
 * Returns: void.
 */
void renderGameover(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* bgTexturegameover, int score, SDL_Rect buttons[]);


#endif
