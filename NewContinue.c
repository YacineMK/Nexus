#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

// Define the button's position and size
SDL_Rect ContinueButtonRect = {0, 0, 200, 50};
SDL_Rect NewGameButtonRect = {0, 0, 200, 50}; // Added the second button

// Function to check if a point is inside the button
int isInsideButton(int x, int y, SDL_Rect buttonRect) {
    return (x >= buttonRect.x && x <= buttonRect.x + buttonRect.w &&
            y >= buttonRect.y && y <= buttonRect.y + buttonRect.h);
}

int main(int argc, char *argv[]) {
    // Initialize image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        // Handle initialization error
    }

    // Initialize TTF
    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("GAME STATUS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Renderer init
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load the background image
    SDL_Surface *backgroundImage = IMG_Load("./assets/bg_purple.png"); // Replace with your image path
    if (!backgroundImage) {
        fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundImage);
    SDL_FreeSurface(backgroundImage);

    // Load a font
    TTF_Font *font = TTF_OpenFont("./assets/Terminal.ttf", 36);
    TTF_Font *font2 = TTF_OpenFont("./assets/Terminal.ttf", 24);
    if (!font) {
        fprintf(stderr, "Failed to load font! TTF_Error: %s\n", TTF_GetError());
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Calculate the center of the screen
    int centerX = 800 / 2;
    int centerY = 600 / 2;

    // Position the buttons relative to the center
    ContinueButtonRect.x = centerX - 210;
    ContinueButtonRect.y = centerY - 25;
    NewGameButtonRect.x = centerX + 10;
    NewGameButtonRect.y = centerY - 25;

    // Main loop
    int quit = 0;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (isInsideButton(mouseX, mouseY, ContinueButtonRect)) {
                    printf("Continue Button Clicked!\n");
                } else if (isInsideButton(mouseX, mouseY, NewGameButtonRect)) {
                    printf("New Game Button Clicked!\n");
                }
            }
        }

        // Render the background image
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Render the buttons (rectangle)
        SDL_SetRenderDrawColor(renderer, 238, 223, 223, 255);
        SDL_RenderFillRect(renderer, &ContinueButtonRect);
        SDL_RenderFillRect(renderer, &NewGameButtonRect);

        SDL_Color textColor = {0, 0, 0, 255}; // Black color

        // Render the text on the Continue button
        SDL_Surface *ContinueTextSurface = TTF_RenderText_Solid(font2, "Continue", textColor);
        SDL_Texture *ContinueTextTexture = SDL_CreateTextureFromSurface(renderer, ContinueTextSurface);
        SDL_FreeSurface(ContinueTextSurface);

        SDL_Rect ContinueTextRect;
        ContinueTextRect.x = ContinueButtonRect.x + (ContinueButtonRect.w - ContinueTextSurface->w) / 2;
        ContinueTextRect.y = ContinueButtonRect.y + (ContinueButtonRect.h - ContinueTextSurface->h) / 2;
        ContinueTextRect.w = ContinueTextSurface->w;
        ContinueTextRect.h = ContinueTextSurface->h;

        SDL_RenderCopy(renderer, ContinueTextTexture, NULL, &ContinueTextRect);
        SDL_DestroyTexture(ContinueTextTexture);

        // Render the text on the New Game button
        SDL_Surface *NewGameTextSurface = TTF_RenderText_Solid(font2, "New Game", textColor);
        SDL_Texture *NewGameTextTexture = SDL_CreateTextureFromSurface(renderer, NewGameTextSurface);
        SDL_FreeSurface(NewGameTextSurface);

        SDL_Rect NewGameTextRect;
        NewGameTextRect.x = NewGameButtonRect.x + (NewGameButtonRect.w - NewGameTextSurface->w) / 2;
        NewGameTextRect.y = NewGameButtonRect.y + (NewGameButtonRect.h - NewGameTextSurface->h) / 2;
        NewGameTextRect.w = NewGameTextSurface->w;
        NewGameTextRect.h = NewGameTextSurface->h;

        SDL_RenderCopy(renderer, NewGameTextTexture, NULL, &NewGameTextRect);
        SDL_DestroyTexture(NewGameTextTexture);

        // Render the centered message
        SDL_Color textColor2 = {255, 255, 255, 255}; // White color
        SDL_Surface *messageSurface = TTF_RenderText_Solid(font, "Choose your game status", textColor2);
        SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);

        SDL_Rect messageRect;
        messageRect.x = (800 - messageSurface->w) / 2; // Centered horizontally
        messageRect.y = centerY - 90; // Move it 90 pixels above the center
        messageRect.w = messageSurface->w;
        messageRect.h = messageSurface->h;

        SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
        SDL_FreeSurface(messageSurface);
        SDL_DestroyTexture(messageTexture);

        // Present the renderer to display the button and text
        SDL_RenderPresent(renderer);
    }

    // Wait for the user to close the window
    SDL_Event event1;
    while (SDL_WaitEvent(&event1)) {
        if (event1.type == SDL_QUIT) {
            break;
        }
    }

    // Cleanup and quit SDL
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
