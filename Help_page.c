#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

// Function to check if a point is inside a rectangle
bool isInsideRect(int x, int y, SDL_Rect rect) {
    return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
}

// Function to handle button clicks and print messages
void handleButtonClick(const char *buttonName) {
    printf("%s Button Clicked!\n", buttonName);
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
    SDL_Surface *backgroundSurface = IMG_Load("bg_purple.png");
    if (!backgroundSurface) {
        printf("Failed to load the background image.\n");
        return 1;
    }
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    // Load the text file
    const char *filename = "our_text.txt";
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

    // Create a "Home" button
    TTF_Font *font = TTF_OpenFont("Help_font.ttf", 26); // Adjust font size for the main text
    TTF_Font *buttonFont = TTF_OpenFont("Terminal.ttf", 30); // Adjust font size for the button text
    SDL_Color textColor = {255, 255, 255}; // White text color

    // Define the button's position and dimensions with margins
    int buttonX = 340; // X-coordinate of the button
    int buttonY = 530; // Y-coordinate of the button
    int buttonWidth = 120; // Width of the button
    int buttonHeight = 40; // Height of the button

    SDL_Rect buttonTextRect = {buttonX, buttonY, buttonWidth, buttonHeight};
    SDL_Surface *buttonTextSurface = TTF_RenderText_Solid(buttonFont, "Home", textColor);
    
    int textWidth, textHeight;
    TTF_SizeText(buttonFont, "Home", &textWidth, &textHeight);

    SDL_Texture *buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
    SDL_Rect textPosition = {buttonX + (buttonWidth - textWidth) / 2, buttonY + (buttonHeight - textHeight) / 2, textWidth, textHeight};

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
                    // Handle button click action and print a message
                    handleButtonClick("Home");
                }
                // Add more conditions for other buttons if needed
            }
        }

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render the background image
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Render the text from the file
        TTF_SetFontStyle(font, TTF_STYLE_NORMAL); // Reset font style
        SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font, text, textColor, 600); // 800 is the line width
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect dstRect = {(800 - textSurface->w) / 2, 30, textSurface->w, textSurface->h};

        SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        // Render the "Home" button with a rectangle around it
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set the color for the rectangle (white in this case)
        SDL_RenderDrawRect(renderer, &buttonTextRect); // Draw a rectangle around the button
        SDL_RenderCopy(renderer, buttonTextTexture, NULL, &textPosition);

        // Present the renderer
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    TTF_CloseFont(font);
    TTF_CloseFont(buttonFont);
    TTF_Quit();
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(buttonTextTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
