#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

// Define the button's position and size
SDL_Rect machineButtonRect = {300, 250, 200, 50};
SDL_Rect userButtonRect = {300, 340, 200, 50}; // Added the second button


// Function to check if a point is inside the button
int isInsideButton(int x, int y,SDL_Rect buttonRect) {
    return (x >= buttonRect.x && x <= buttonRect.x + buttonRect.w &&
            y >= buttonRect.y && y <= buttonRect.y + buttonRect.h);
}



  int main() {

     //init image
      if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    // Handle initialization error
    }

    //init ttf
    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("MEMO GAME!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    //renderer init

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);

        SDL_Quit();
        return 1;
    }

    // Load the background image
    SDL_Surface* backgroundImage = IMG_Load("./assets/bg_purple.png"); // Replace with your image path
    if (!backgroundImage) {
        fprintf(stderr, "IMG_Load Error: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundImage);
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



    //main loop
    int quit = 0;
    SDL_Event event;

    while (!quit) {
            while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (isInsideButton(mouseX, mouseY, machineButtonRect)) {
                    printf("Machine Button Clicked!\n");
                } else if (isInsideButton(mouseX, mouseY, userButtonRect)) {
                    printf("User Button Clicked!\n");
                }
            }
        }



        // Render the background image
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

       // Render the buttons (rectangle)
       SDL_SetRenderDrawColor(renderer, 238, 223, 223, 255);
       SDL_RenderFillRect(renderer, &machineButtonRect);
       SDL_RenderFillRect(renderer, &userButtonRect);

       SDL_Color textColor = {0, 0, 0, 255}; // Black color
       // Render the text on the Machine button
      SDL_Surface* machineTextSurface = TTF_RenderText_Solid(font2, "Machine", textColor);
      SDL_Texture* machineTextTexture = SDL_CreateTextureFromSurface(renderer, machineTextSurface);
      SDL_FreeSurface(machineTextSurface);

      SDL_Rect machineTextRect;
       machineTextRect.x = machineButtonRect.x + (machineButtonRect.w - machineTextSurface->w) / 2;
       machineTextRect.y = machineButtonRect.y + (machineButtonRect.h - machineTextSurface->h) / 2;
       machineTextRect.w = machineTextSurface->w;
       machineTextRect.h = machineTextSurface->h;

     SDL_RenderCopy(renderer, machineTextTexture, NULL, &machineTextRect);
     SDL_DestroyTexture(machineTextTexture);

     // Render the text on the User button
     SDL_Surface* userTextSurface = TTF_RenderText_Solid(font2, "User", textColor);
     SDL_Texture* userTextTexture = SDL_CreateTextureFromSurface(renderer, userTextSurface);
     SDL_FreeSurface(userTextSurface);

     SDL_Rect userTextRect;
     userTextRect.x = userButtonRect.x + (userButtonRect.w - userTextSurface->w) / 2;
     userTextRect.y = userButtonRect.y + (userButtonRect.h - userTextSurface->h) / 2;
     userTextRect.w = userTextSurface->w;
     userTextRect.h = userTextSurface->h;

     SDL_RenderCopy(renderer, userTextTexture, NULL, &userTextRect);
     SDL_DestroyTexture(userTextTexture);


        // Render the centered message
        SDL_Color textColor2 = {255, 255, 255, 255}; // Black color
        SDL_Surface *messageSurface = TTF_RenderText_Solid(font, "Choose the game mode", textColor2);
        SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);

        SDL_Rect messageRect;
        messageRect.x = (800 - messageSurface->w) / 2; // Centered horizontally
        messageRect.y = 160; // Above the buttons
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






