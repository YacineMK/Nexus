#include "startpage.h"

int main()
{
    SDL_Window *win = initSDL();
    if (win == NULL)
    {
        return 1;
    }

    SDL_Renderer *renderer = initRenderer(win);
    if (renderer == NULL)
    {
        cleanup(win, NULL, NULL, NULL);
        return 1;
    }

    TTF_Font *font = initFont();
    if (font == NULL)
    {
        cleanup(win, renderer, NULL, NULL);
        return 1;
    }

    SDL_Texture *backgroundTexture = loadImageTexture(renderer, "./assets/bg_purple.png");
    if (backgroundTexture == NULL)
    {
        cleanup(win, renderer, font, NULL);
        return 1;
    }

    cleanup(win, renderer, font, backgroundTexture);

    return 0;
}