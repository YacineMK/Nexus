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
        800,
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

    TTF_Font *font = TTF_OpenFont("./assets/Terminal.ttf", 40);
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
    SDL_Surface *backgroundSurface = IMG_Load("./assets/bg_purple.png");

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

        // Render the background image
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Render text
        SDL_Color textColor = {255, 255, 255}; // White text color
        textSurface = TTF_RenderText_Solid(font, "Welcome to Memo with Smaili", textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {100, 100, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_RenderPresent(renderer);
    }

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    cleanup(win, renderer);

    return 0;
}
