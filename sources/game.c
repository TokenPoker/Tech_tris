
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>

// Render the Game Over screen with a "Play Again" button
bool renderGameOverScreen(SDL_Renderer* renderer, TTF_Font* font) {
    bool waiting = true;
    SDL_Event event;
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color red = {200, 0, 0, 255};

    // Coordinates of the "Play Again" button
    SDL_Rect buttonRect = {300, 350, 200, 60};

    while (waiting) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                // Check if the click was inside the button
                if (x >= buttonRect.x && x <= buttonRect.x + buttonRect.w &&
                    y >= buttonRect.y && y <= buttonRect.y + buttonRect.h) {
                    return true; // Replay!
                }
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw "Game Over"
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, "GAME OVER", red);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {250, 200, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        // Draw "Play Again" button
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // gray button
        SDL_RenderFillRect(renderer, &buttonRect);

        SDL_Surface* btnSurface = TTF_RenderUTF8_Blended(font, "Play Again", white);
        SDL_Texture* btnTexture = SDL_CreateTextureFromSurface(renderer, btnSurface);
        SDL_Rect btnTextRect = {buttonRect.x + 25, buttonRect.y + 15, btnSurface->w, btnSurface->h};
        SDL_RenderCopy(renderer, btnTexture, NULL, &btnTextRect);
        SDL_FreeSurface(btnSurface);
        SDL_DestroyTexture(btnTexture);

        // Present to screen
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Limit to ~60 FPS
    }

    return false; // Exit by default
}
