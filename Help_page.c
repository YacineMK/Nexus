#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


// Function to check if a point is inside a rectangle
bool isInsideRect(int x, int y, SDL_Rect rect) {
    return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
}

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Help?", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Init();

    // Load the background image
    SDL_Surface *backgroundSurface = IMG_Load("./assets/bg_purple.png");
    if (!backgroundSurface) {
        printf("Failed to load the background image.\n");
        return 1;
    }
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    // Load the text file
    const char *filename = "./assets/our_text.txt";
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open the text file.\n");
        return 1;
    }

    // Read the content of the text file
    char text[8192] = {0}; // Adjust the buffer size as needed

    // Read lines from the file and append them to the 'text' variable
    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        strcat(text, line);
    }

    fclose(file);

    // Create a "Back to Home Page" button with increased margins and adjusted dimensions
    TTF_Font *font = TTF_OpenFont("./assets/help_font.ttf", 26); // Adjust font size for the main text
    TTF_Font *buttonFont = TTF_OpenFont("./assets/Terminal.ttf", 30); // Adjust font size for the button text
    SDL_Color buttonColor = {255, 255, 255}; // White background color
    SDL_Color textColor = {0, 0, 0}; // Black text color

    SDL_Surface *buttonTextSurface = TTF_RenderText_Solid(buttonFont, "Home", textColor);
    SDL_Texture *buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);

    // Define the button's position and dimensions with margins
    int buttonX = 275; // X-coordinate of the button
    int buttonY = 530; // Y-coordinate of the button
    int buttonWidth = 250; // Width of the button
    int buttonHeight = 60; // Height of the button

    SDL_Rect buttonTextRect = {buttonX, buttonY, buttonWidth, buttonHeight};

    // Calculate the position to center the text within the button
    int textX = buttonX + (buttonWidth - buttonTextSurface->w) / 2;
    int textY = buttonY + (buttonHeight - buttonTextSurface->h) / 2;

    SDL_Rect textPosition = {textX, textY, buttonTextSurface->w, buttonTextSurface->h};

    // Event loop
    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                if (isInsideRect(mouseX, mouseY, buttonTextRect)) {
                    // Handle button click action, e.g., navigate back to the home page
                    // Add your logic here.
                }
            }
        }

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render the background image
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Render the text from the file
        SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font, text, buttonColor, 600); // 800 is the line width
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect dstRect = {(800 - textSurface->w) / 2, 30, textSurface->w, textSurface->h};

        SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        // Render the "Back to Home Page" button with a white background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set the background color to white
        SDL_RenderFillRect(renderer, &buttonTextRect);
        SDL_RenderCopy(renderer, buttonTextTexture, NULL, &textPosition);

        // Present the renderer
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(buttonTextTexture); // Cleanup the button texture
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
