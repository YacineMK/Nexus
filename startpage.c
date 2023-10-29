#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

// Function to initialize SDL, SDL_image, and create a window
SDL_Window *initSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return NULL;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        printf("IMG_Init failed: %s\n", IMG_GetError());
        SDL_Quit();
        return NULL;
    }

    SDL_Window *win = SDL_CreateWindow(
        "Memo with Smaili",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        800,
        SDL_WINDOW_SHOWN);

    if (win == NULL)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    return win;
}

// Function to create a renderer
SDL_Renderer *initRenderer(SDL_Window *win)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return NULL;
    }

    return renderer;
}

// Function to initialize TTF and load the font
TTF_Font *initFont()
{
    if (TTF_Init() < 0)
    {
        printf("TTF_Init() failed: %s\n", TTF_GetError());
        return NULL;
    }

    TTF_Font *font = TTF_OpenFont("./assets/Terminal.ttf", 40);
    if (font == NULL)
    {
        printf("TTF_OpenFont() failed: %s\n", TTF_GetError());
        return NULL;
    }

    return font;
}

// Function to load and create a texture from an image file
SDL_Texture *loadImageTexture(SDL_Renderer *renderer, const char *filePath)
{
    SDL_Surface *surface = IMG_Load(filePath);
    if (surface == NULL)
    {
        printf("Failed to load image: %s\n", IMG_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
    {
        printf("Failed to create texture: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    return texture;
}

// Function to render text to the renderer
SDL_Texture *renderTextToTexture(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color textColor)
{
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (textSurface == NULL)
    {
        printf("TTF_RenderText_Solid failed: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    return textTexture;
}

// Function to clean up resources
void cleanup(SDL_Window *win, SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *backgroundTexture)
{
    if (backgroundTexture != NULL)
    {
        SDL_DestroyTexture(backgroundTexture);
    }
    if (font != NULL)
    {
        TTF_CloseFont(font);
    }
    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (win != NULL)
    {
        SDL_DestroyWindow(win);
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}