#include "render.h"
#include "score.h"

SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void renderMenu(SDL_Renderer* renderer, SDL_Texture* bgTexture, SDL_Rect buttons[]) {
    
    // Draw background image
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL); 

    // Get current mouse position
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);


    // Highlight buttons on hover
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow outline
    for (int i = 0; i < 3; i++){
        if (mouseX >= buttons[i].x && mouseX <= buttons[i].x + buttons[i].w && mouseY >= buttons[i].y && mouseY <= buttons[i].y + buttons[i].h){
            SDL_RenderDrawRect(renderer, &buttons[i]);
        }
    }
}

void renderOptions(SDL_Renderer* renderer, TTF_Font* font, int selectedMode, int musicVolume, int isMuted, const char* modesText[]){
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
}

void renderPseudo(SDL_Renderer* renderer, TTF_Font* font, char playerName[]){
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
}

void renderGame(SDL_Renderer* renderer, TTF_Font* font, Grid* grid, Piece* currentPiece, int score) {
    // Set background color (dark blue) and clear the screen

    SDL_SetRenderDrawColor(renderer, 96, 80, 220, 255);
    SDL_RenderClear(renderer);

    insert_piece_in_grid(currentPiece, grid); // Insert the current piece into the grid

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
    remove_piece_from_grid(currentPiece, grid); // Remove the current piece from the grid

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

void renderPause(SDL_Renderer* renderer, SDL_Texture* bgTexturePause, SDL_Rect buttons[]){
    // Draw background image
    SDL_RenderCopy(renderer, bgTexturePause, NULL, NULL); 

    // Get current mouse position
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);


    // Highlight buttons on hover
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow outline
    for (int i = 0; i < 3; i++){
        if (mouseX >= buttons[i].x && mouseX <= buttons[i].x + buttons[i].w && mouseY >= buttons[i].y && mouseY <= buttons[i].y + buttons[i].h){
            SDL_RenderDrawRect(renderer, &buttons[i]);
        }
    }
}

void renderExit(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* bgTexturesave){
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

void renderGameover(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* bgTexturegameover, int score, SDL_Rect buttons[]){
    // Draw background image
    SDL_RenderCopy(renderer, bgTexturegameover, NULL, NULL); 

    // Get current mouse position
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);


    // Highlight buttons on hover
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow outline
    for (int i = 0; i < 3; i++){
        if (mouseX >= buttons[i].x && mouseX <= buttons[i].x + buttons[i].w && mouseY >= buttons[i].y && mouseY <= buttons[i].y + buttons[i].h){
            SDL_RenderDrawRect(renderer, &buttons[i]);
        }
    }
    // Display the score in a specific area (between x=540, y=275 and x=790, y=330)
    char scoreText[50];
    sprintf(scoreText, "%d", score); // format the score text

    SDL_Texture* scoreTexture = renderText(renderer, font, scoreText, (SDL_Color){255, 255, 255, 255}); // white text
    int texW = 0, texH = 0;
    SDL_QueryTexture(scoreTexture, NULL, NULL, &texW, &texH);

    // Center the text inside the box (540,275)-(790,330)
    int centerX = 540 + (790 - 540) / 2 - texW / 2;
    int centerY = 275 + (330 - 275) / 2 - texH / 2;

    SDL_Rect scoreRect = { centerX, centerY, texW, texH };
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
    SDL_DestroyTexture(scoreTexture);

    // Display top 3 scores inside box (555,450)-(800,570)
    Score* top[3];
    get_top_three_scores_and_save("assets/score.txt", top);

    int boxX = 555;
    int boxY = 450;
    int boxWidth = 800 - 555;
    int boxHeight = 570 - 450;
    int lineSpacing = boxHeight / 3; // spacing between lines

    for (int i = 0; i < 3; i++) {
        char scoreText[50];
        sprintf(scoreText, "%s: %d", top[i]->pseudo, top[i]->score);

        SDL_Texture* textTexture = renderText(renderer, font, scoreText, (SDL_Color){255, 255, 255, 255});
        int texW = 0, texH = 0;
        SDL_QueryTexture(textTexture, NULL, NULL, &texW, &texH);

        SDL_Rect dstRect = {
            boxX + (boxWidth - texW) / 2,           // center horizontally
            boxY + i * lineSpacing + (lineSpacing - texH) / 2, // vertical spacing
            texW,
            texH
        };
        SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);
        SDL_DestroyTexture(textTexture);
    }
}
