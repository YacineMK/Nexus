#ifndef STARTERPAGE_H
#define STARTERPAGE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Window *initSDL();
SDL_Renderer *initRenderer(SDL_Window *win);
TTF_Font *initFont();
SDL_Texture *loadImageTexture(SDL_Renderer *renderer, const char *filePath);
SDL_Texture *renderTextToTexture(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color textColor);
void cleanup(SDL_Window *win, SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *backgroundTexture);

#endif
