#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

void cleanup(SDL_Window *win, SDL_Renderer *renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
}

void btn(SDL_Renderer *renderer, TTF_Font *font, const char* text, int h) {
    int buttonWidth = 0.6 * 800;
    int buttonHeight = 60;
    int buttonX = (800 - buttonWidth) / 2;
    int buttonY = h - buttonHeight; 
    SDL_Rect buttonRect = {buttonX, buttonY, buttonWidth, buttonHeight};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &buttonRect);

    SDL_Color buttonTextColor = {37, 37, 37};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, buttonTextColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect buttonTextRect = { buttonX + (buttonWidth - textSurface->w) / 2, buttonY + (buttonHeight - textSurface->h) / 2, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &buttonTextRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}



int main() {
    SDL_Window *win;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        printf("IMG_Init failed: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }
    
    win = SDL_CreateWindow(
        "Memo with Smaili",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        700,
        SDL_WINDOW_SHOWN
    );

    if (win == NULL) {
        printf("Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() < 0) {
        printf("TTF_Init() failed: %s\n", TTF_GetError());
        cleanup(win, renderer);
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("./assets/Terminal.ttf", 41);
    if (font == NULL) {
        printf("TTF_OpenFont() failed: %s\n", TTF_GetError());
        cleanup(win, renderer);
        return 1;
    }

    SDL_Surface *textSurface;
    SDL_Texture *textTexture;

    bool quit = false;
    SDL_Event exit;

    SDL_Texture *backgroundTexture = NULL;
    SDL_Surface *backgroundSurface = IMG_Load("./assets/bg.png");

    if (backgroundSurface == NULL) {
        printf("Failed to load background image: %s\n", IMG_GetError());
        cleanup(win, renderer);
        return 1;
    }

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    if (backgroundTexture == NULL) {
        printf("Failed to create background texture: %s\n", SDL_GetError());
        cleanup(win, renderer);
        return 1;
    }

    while (!quit) {
        while (SDL_PollEvent(&exit)) {
            if (exit.type == SDL_QUIT) {
                quit = true;
            }
        }

            SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
            SDL_RenderClear(renderer);

            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

            SDL_Color textColor = {255, 255, 255};
            textSurface = TTF_RenderText_Solid(font, "Welcome to Memo with Smaili", textColor);
            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {120, 120, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            btn(renderer, font, "Start" , 420);
            btn(renderer, font, "Help" , 510);
            btn(renderer, font, "High Score" , 600);
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
            SDL_RenderPresent(renderer); 
    }
    
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    cleanup(win, renderer);

    return 0;
}
