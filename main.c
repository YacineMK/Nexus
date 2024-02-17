#include "logic.h"

// Window dimensions
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 650;

#define MAX_USERNAMES 20
char *usernames[20];
int usernamesCount = 0;
int MAX_VOLUME = 20;

#define ANIMATION_DURATION 100
#define CHARACTER_WIDTH 300
#define CHARACTER_HEIGHT 300
#define CHARACTER_IMAGE_COUNT 12
#define FRAME_DELAY 100

#define CHARACTER_PATH_PREFIX "photo/bounadem"

/*--------------------------------------------------------*/
SDL_Window *win = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *backgroundTexture = NULL;
SDL_Surface *backgroundSurface = NULL;
//------------effect------------
Mix_Chunk *bonk;
Mix_Chunk *click;
Mix_Chunk *game_over;
Mix_Chunk *level_win;
Mix_Chunk *success;
//------------------------------

typedef struct color
{
    int r;
    int g;
    int b;
    int a;
} Color;

void cleanup(SDL_Window *win, SDL_Renderer *renderer, SDL_Texture *backgroundTexture, SDL_Surface *backgroundSurface);

void DelayTa3na(int a){
    Uint32 startTime = SDL_GetTicks();
    while(SDL_GetTicks() - startTime <= a){
        SDL_Event exit;
        while(SDL_PollEvent(&exit)){
            if(exit.type == SDL_QUIT){
                cleanup(win,renderer,backgroundTexture,backgroundSurface);
            }
        }
    }
}

void cleanup(SDL_Window *win, SDL_Renderer *renderer, SDL_Texture *backgroundTexture, SDL_Surface *backgroundSurface)
{
    if (backgroundTexture)
    {
        SDL_DestroyTexture(backgroundTexture);
    }

    if (backgroundSurface)
    {
        SDL_FreeSurface(backgroundSurface);
    }

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }

    if (win)
    {
        SDL_DestroyWindow(win);
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


void initRender(){
    SDL_DestroyTexture(backgroundTexture);
    SDL_FreeSurface(backgroundSurface);
    SDL_DestroyRenderer(renderer);

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    backgroundTexture = NULL;
    backgroundSurface = NULL;

}

void drawTransparentRectangle(int x, int y, int width, int height, Color border, Color fill)
{
    SDL_Rect rect = {x, y, width, height};

    // Dessiner la bordure
    SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
    SDL_RenderDrawRect(renderer, &rect);

    // Rendre l'intérieur transparent
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, fill.a);
    SDL_RenderFillRect(renderer, &rect);

    // Réinitialiser le mode de mélange
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void drawCellule(Ball ball, int N)
{
    int CELL_SIZE = 420 / N;
    int BALL_WIDTH = CELL_SIZE ;
    int BALL_HEIGHT = CELL_SIZE ;
    int offsetX = 390;
    int offsetY = 115;

    int rectX = offsetX + ball.x * CELL_SIZE;
    int rectY = offsetY + ball.y * CELL_SIZE;

    drawTransparentRectangle(rectX, rectY, BALL_WIDTH, BALL_HEIGHT, (Color){255, 0, 0, 255}, (Color){0, 0, 0, 0});
}

void drawFilledCircle(int centerX, int centerY, int radius, Color c)
{
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    for (int y = -radius; y <= radius; ++y)
    {
        int maxX = (int)sqrt(radius * radius - y * y);
        for (int x = -maxX; x <= maxX; ++x)
        {
            SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        }
    }
}

void drawBall(Ball ball, int N)
{
    int CELL_SIZE = 420 / N;
    int BALL_RADIUS = CELL_SIZE / 4;
    int offsetX = (SCREEN_WIDTH - N * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - N * CELL_SIZE) / 2;

    int circleCenterX = offsetX + ball.x * CELL_SIZE + CELL_SIZE / 2;
    int circleCenterY = offsetY + ball.y * CELL_SIZE + CELL_SIZE / 2;

    drawFilledCircle(circleCenterX, circleCenterY, BALL_RADIUS, (Color){255, 0, 0, 255});
}

void drawGreenBall(Ball ball, int N)
{
    int CELL_SIZE = 420 / N;
    int BALL_RADIUS = CELL_SIZE / 4;
    int offsetX = (SCREEN_WIDTH - N * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - N * CELL_SIZE) / 2;

    int circleCenterX = offsetX + ball.x * CELL_SIZE + CELL_SIZE / 2;
    int circleCenterY = offsetY + ball.y * CELL_SIZE + CELL_SIZE / 2;

    drawFilledCircle(circleCenterX, circleCenterY, BALL_RADIUS, (Color){0, 255, 0, 255});
}

void drawOrangeBall(Ball ball, int N)
{
    int CELL_SIZE = 420 / N;
    int BALL_RADIUS = CELL_SIZE / 4;
    int offsetX = (SCREEN_WIDTH - N * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - N * CELL_SIZE) / 2;

    int circleCenterX = offsetX + ball.x * CELL_SIZE + CELL_SIZE / 2;
    int circleCenterY = offsetY + ball.y * CELL_SIZE + CELL_SIZE / 2;

    drawFilledCircle(circleCenterX, circleCenterY, BALL_RADIUS, (Color){255, 128, 0, 255});
}

void renderMatrix(int N, int grid[N][N] )
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Load background image
    SDL_Surface *backgroundSurface = IMG_Load("photo/background/backgroundMatrix.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    int CELL_SIZE = 420 / N;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int offsetX = (SCREEN_WIDTH - N * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - N * CELL_SIZE) / 2;


    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            SDL_Rect cellRect = {offsetX + j * CELL_SIZE, offsetY + i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_RenderDrawRect(renderer, &cellRect);

            if ((i == 0 && j == 0) || (i == N - 1 && j == 0) || (i == 0 && j == N - 1) || (i == N - 1 && j == N - 1))
            {
            }
            else
            {

                if ((i == 0 || i == N - 1) && (j == 0 || j == N - 1))
                {
                    // Colorier les cases des bords diff�remment pour les distinguer
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &cellRect);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                }

                if (i == 0)
                {
                    int circleCenterX = offsetX + j * CELL_SIZE + CELL_SIZE / 2;
                    int circleCenterY = offsetY + i * CELL_SIZE + CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
                if (i == N - 1)
                {
                    int circleCenterX = offsetX + j * CELL_SIZE + CELL_SIZE / 2;
                    int circleCenterY = offsetY + (i + 1) * CELL_SIZE - CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
                if (j == 0)
                {
                    int circleCenterX = offsetX + j * CELL_SIZE + CELL_SIZE / 2;
                    int circleCenterY = offsetY + i * CELL_SIZE + CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
                if (j == N - 1)
                {
                    int circleCenterX = offsetX + (j + 1) * CELL_SIZE - CELL_SIZE / 2;
                    int circleCenterY = offsetY + i * CELL_SIZE + CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void renderAlexMatrix(int N, int grid[N][N],TTF_Font *font,int score,int level)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Load background image
    SDL_Surface *backgroundSurface = IMG_Load("photo/background/backgroundAlex.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    int CELL_SIZE = 420 / N;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int offsetX = (SCREEN_WIDTH - N * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - N * CELL_SIZE) / 2;

    // Render home icon
    SDL_Surface *homeSurface = IMG_Load("photo/bouton_home.png");

    if (homeSurface)
    {
        SDL_Texture *homeTexture = SDL_CreateTextureFromSurface(renderer, homeSurface);
        SDL_Rect homeRect = {1070, 18, 60, 60};
        SDL_RenderCopy(renderer, homeTexture, NULL, &homeRect);

        SDL_DestroyTexture(homeTexture);
        SDL_FreeSurface(homeSurface);
    }
/*
    // Render pause icon
    SDL_Surface *pauseSurface = IMG_Load("photo/bouton_pause.png");

    if (pauseSurface)
    {
        SDL_Texture *pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
        SDL_Rect pauseRect = {60, 18, 60, 60};
        SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseRect);

        SDL_DestroyTexture(pauseTexture);
        SDL_FreeSurface(pauseSurface);
    }*/

    SDL_Color textColor = {255, 255, 255, 255};

    int xi = 550, yi = 25;
    int xj = 370, yj = 595;
    int textSize = 25;
    char ScoreText[15];
    char NiveauText[30];

    snprintf(ScoreText, sizeof(ScoreText), "Score : %d ", score);
    snprintf(NiveauText, sizeof(NiveauText), "Niveau : %d ", level);

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, ScoreText, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Surface *niveauSurface = TTF_RenderText_Solid(font, NiveauText, textColor);
    SDL_Texture *niveauTexture = SDL_CreateTextureFromSurface(renderer, niveauSurface);

    SDL_Rect textRect = {xi, yi, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_Rect niveauRect = {xj, yj, niveauSurface->w, niveauSurface->h};
    SDL_RenderCopy(renderer, niveauTexture, NULL, &niveauRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(niveauTexture);
    SDL_FreeSurface(niveauSurface);

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            SDL_Rect cellRect = {offsetX + j * CELL_SIZE, offsetY + i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_RenderDrawRect(renderer, &cellRect);

            if ((i == 0 && j == 0) || (i == N - 1 && j == 0) || (i == 0 && j == N - 1) || (i == N - 1 && j == N - 1))
            {
            }
            else
            {

                if ((i == 0 || i == N - 1) && (j == 0 || j == N - 1))
                {
                    // Colorier les cases des bords diff�remment pour les distinguer
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &cellRect);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                }

                if (i == 0)
                {
                    int circleCenterX = offsetX + j * CELL_SIZE + CELL_SIZE / 2;
                    int circleCenterY = offsetY + i * CELL_SIZE + CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
                if (i == N - 1)
                {
                    int circleCenterX = offsetX + j * CELL_SIZE + CELL_SIZE / 2;
                    int circleCenterY = offsetY + (i + 1) * CELL_SIZE - CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
                if (j == 0)
                {
                    int circleCenterX = offsetX + j * CELL_SIZE + CELL_SIZE / 2;
                    int circleCenterY = offsetY + i * CELL_SIZE + CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
                if (j == N - 1)
                {
                    int circleCenterX = offsetX + (j + 1) * CELL_SIZE - CELL_SIZE / 2;
                    int circleCenterY = offsetY + i * CELL_SIZE + CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}

// Perfecto
void renderPlayerMatrix(TTF_Font *font, int N, int grid[N][N], Player player)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Load background image
    SDL_Surface *backgroundSurface = IMG_Load("photo/background/backgroundPlayer.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    int CELL_SIZE = 420 / N;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int offsetX = (SCREEN_WIDTH - N * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - N * CELL_SIZE) / 2;

    // Render home icon
    SDL_Surface *homeSurface = IMG_Load("photo/bouton_home.png");

    if (homeSurface)
    {
        SDL_Texture *homeTexture = SDL_CreateTextureFromSurface(renderer, homeSurface);
        SDL_Rect homeRect = {1070, 18, 60, 60};
        SDL_RenderCopy(renderer, homeTexture, NULL, &homeRect);

        SDL_DestroyTexture(homeTexture);
        SDL_FreeSurface(homeSurface);
    }

    // Render pause icon
    SDL_Surface *pauseSurface = IMG_Load("photo/bouton_pause.png");

    if (pauseSurface)
    {
        SDL_Texture *pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
        SDL_Rect pauseRect = {60, 18, 60, 60};
        SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseRect);

        SDL_DestroyTexture(pauseTexture);
        SDL_FreeSurface(pauseSurface);
    }

    SDL_Color textColor = {255, 255, 255, 255};

    int xi = 550, yi = 25;
    int xj = 370, yj = 595;
    int textSize = 25;
    char ScoreText[15];
    char NiveauText[30];
    snprintf(ScoreText, sizeof(ScoreText), "Score : %d ", player.score);
    snprintf(NiveauText, sizeof(NiveauText), "Niveau : %d , Partie : %d", player.Niveau, player.Partie);

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, ScoreText, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Surface *niveauSurface = TTF_RenderText_Solid(font, NiveauText, textColor);
    SDL_Texture *niveauTexture = SDL_CreateTextureFromSurface(renderer, niveauSurface);

    SDL_Rect textRect = {xi, yi, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_Rect niveauRect = {xj, yj, niveauSurface->w, niveauSurface->h};
    SDL_RenderCopy(renderer, niveauTexture, NULL, &niveauRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(niveauTexture);
    SDL_FreeSurface(niveauSurface);

    SDL_Surface *live1Surface = IMG_Load("photo/oneleft.png");
    SDL_Surface *live2Surface = IMG_Load("photo/twoleft.png");
    SDL_Surface *live3Surface = IMG_Load("photo/threeleft.png");

    switch (player.echec_consecutive)
    {
    case 0:

        if (live1Surface)
        {
            SDL_Texture *live1Texture = SDL_CreateTextureFromSurface(renderer, live1Surface);
            SDL_Rect live1Rect = {800, 570, CELL_SIZE, CELL_SIZE};
            SDL_RenderCopy(renderer, live1Texture, NULL, &live1Rect);
            SDL_DestroyTexture(live1Texture);
            SDL_FreeSurface(live1Surface);
        }
        if (live2Surface)
        {
            SDL_Texture *live2Texture = SDL_CreateTextureFromSurface(renderer, live2Surface);
            SDL_Rect live2Rect = {750, 570, CELL_SIZE, CELL_SIZE};
            SDL_RenderCopy(renderer, live2Texture, NULL, &live2Rect);
            SDL_DestroyTexture(live2Texture);
            SDL_FreeSurface(live2Surface);
        }
        if (live3Surface)
        {
            SDL_Texture *live3Texture = SDL_CreateTextureFromSurface(renderer, live3Surface);
            SDL_Rect live3Rect = {700, 570, CELL_SIZE, CELL_SIZE};
            SDL_RenderCopy(renderer, live3Texture, NULL, &live3Rect);
            SDL_DestroyTexture(live3Texture);
            SDL_FreeSurface(live3Surface);
        }
        break;
    case 1:
        if (live1Surface)
        {
            SDL_Texture *live1Texture = SDL_CreateTextureFromSurface(renderer, live1Surface);
            SDL_Rect live1Rect = {775, 570, CELL_SIZE, CELL_SIZE};
            SDL_RenderCopy(renderer, live1Texture, NULL, &live1Rect);
            SDL_DestroyTexture(live1Texture);
            SDL_FreeSurface(live1Surface);
        }
        if (live2Surface)
        {
            SDL_Texture *live2Texture = SDL_CreateTextureFromSurface(renderer, live2Surface);
            SDL_Rect live2Rect = {725, 570, CELL_SIZE, CELL_SIZE};
            SDL_RenderCopy(renderer, live2Texture, NULL, &live2Rect);
            SDL_DestroyTexture(live2Texture);
            SDL_FreeSurface(live2Surface);
        }
        break;
    case 2:
        if (live1Surface)
        {
            SDL_Texture *live1Texture = SDL_CreateTextureFromSurface(renderer, live1Surface);
            SDL_Rect live1Rect = {750, 570, CELL_SIZE, CELL_SIZE};
            SDL_RenderCopy(renderer, live1Texture, NULL, &live1Rect);
            SDL_DestroyTexture(live1Texture);
            SDL_FreeSurface(live1Surface);
        }
        break;
    }

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            SDL_Rect cellRect = {offsetX + j * CELL_SIZE, offsetY + i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_RenderDrawRect(renderer, &cellRect);

            if ((i == 0 && j == 0) || (i == N - 1 && j == 0) || (i == 0 && j == N - 1) || (i == N - 1 && j == N - 1))
            {
            }
            else
            {

                if ((i == 0 || i == N - 1) && (j == 0 || j == N - 1))
                {
                    // Colorier les cases des bords diff�remment pour les distinguer
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &cellRect);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                }

                if (i == 0)
                {
                    int circleCenterX = offsetX + j * CELL_SIZE + CELL_SIZE / 2;
                    int circleCenterY = offsetY + i * CELL_SIZE + CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
                if (i == N - 1)
                {
                    int circleCenterX = offsetX + j * CELL_SIZE + CELL_SIZE / 2;
                    int circleCenterY = offsetY + (i + 1) * CELL_SIZE - CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
                if (j == 0)
                {
                    int circleCenterX = offsetX + j * CELL_SIZE + CELL_SIZE / 2;
                    int circleCenterY = offsetY + i * CELL_SIZE + CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
                if (j == N - 1)
                {
                    int circleCenterX = offsetX + (j + 1) * CELL_SIZE - CELL_SIZE / 2;
                    int circleCenterY = offsetY + i * CELL_SIZE + CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void renderNexusMatrix(int N, int grid[N][N])
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Load background image
    SDL_Surface *backgroundSurface = IMG_Load("photo/background/backgroundNexus.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    int CELL_SIZE = 420 / N;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int offsetX = (SCREEN_WIDTH - N * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - N * CELL_SIZE) / 2;

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            SDL_Rect cellRect = {offsetX + j * CELL_SIZE, offsetY + i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_RenderDrawRect(renderer, &cellRect);

            if ((i == 0 && j == 0) || (i == N - 1 && j == 0) || (i == 0 && j == N - 1) || (i == N - 1 && j == N - 1))
            {
            }
            else
            {

                if ((i == 0 || i == N - 1) && (j == 0 || j == N - 1))
                {
                    // Colorier les cases des bords diff�remment pour les distinguer
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &cellRect);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                }

                if (i == 0)
                {
                    int circleCenterX = offsetX + j * CELL_SIZE + CELL_SIZE / 2;
                    int circleCenterY = offsetY + i * CELL_SIZE + CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
                if (i == N - 1)
                {
                    int circleCenterX = offsetX + j * CELL_SIZE + CELL_SIZE / 2;
                    int circleCenterY = offsetY + (i + 1) * CELL_SIZE - CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
                if (j == 0)
                {
                    int circleCenterX = offsetX + j * CELL_SIZE + CELL_SIZE / 2;
                    int circleCenterY = offsetY + i * CELL_SIZE + CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
                if (j == N - 1)
                {
                    int circleCenterX = offsetX + (j + 1) * CELL_SIZE - CELL_SIZE / 2;
                    int circleCenterY = offsetY + i * CELL_SIZE + CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 255, 255});
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void thickLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int thickness)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        for (int i = 0; i < thickness; ++i)
        {
            SDL_RenderDrawPoint(renderer, x1, y1 + i);
        }

        if ((x1 == x2) && (y1 == y2))
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void renderDiagonals(int N, int grid[N][N])
{
    int CELL_SIZE = 420 / N;
    int offsetX = (SCREEN_WIDTH - N * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - N * CELL_SIZE) / 2;
    int LINE_THICKNESS = 5; // Adjust this value to change line thickness

    for (int i = 1; i < N - 1; ++i)
    {
        for (int j = 1; j < N - 1; ++j)
        {
            if (grid[i][j] == 2)
            {
                thickLine(renderer, offsetX + j * CELL_SIZE+10, offsetY + i * CELL_SIZE+10,
                          offsetX + (j + 1) * CELL_SIZE-10, offsetY + (i + 1) * CELL_SIZE-10, LINE_THICKNESS);
            }
            else if (grid[i][j] == 1)
            {
                thickLine(renderer, offsetX + j * CELL_SIZE+10, offsetY + (i + 1) * CELL_SIZE-10,
                          offsetX + (j + 1) * CELL_SIZE-10, offsetY + i * CELL_SIZE+10, LINE_THICKNESS);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

Ball handleButtonClick2(int mouseX, int mouseY, int N)
{
    Ball cordonneEstime;

    int CELL_SIZE = 420 / N;
    int offsetX = (SCREEN_WIDTH - N * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - N * CELL_SIZE) / 2;

    cordonneEstime.x = (mouseX - offsetX) / CELL_SIZE;
    cordonneEstime.y = (mouseY - offsetY) / CELL_SIZE;

    return cordonneEstime;
}

void animationTrajectoireV2(int N, int grid[N][N], Ball ball) {
    int orientation;

    SDL_Delay(200);
    renderDiagonals(N, grid);
    SDL_Delay(500);
    do {
        orientation = grid[ball.y][ball.x];

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderNexusMatrix(N, grid);

        if (orientation == DIAGONAL_LEFT_TO_RIGHT) {
            if (ball.direction == 0) {
                ball.direction = 1;
            } else if (ball.direction == 1) {
                ball.direction = 0;
            } else if (ball.direction == 2) {
                ball.direction = 3;
            } else if (ball.direction == 3) {
                ball.direction = 2;
            }
            //Mix_PlayChannel(-1, bonk, 0);
        } else if (orientation == DIAGONAL_RIGHT_TO_LEFT) {
            if (ball.direction == 0) {
                ball.direction = 3;
            } else if (ball.direction == 1) {
                ball.direction = 2;
            } else if (ball.direction == 2) {
                ball.direction = 1;
            } else if (ball.direction == 3) {
                ball.direction = 0;
            }
            //Mix_PlayChannel(-1, bonk, 0);
        }

        if (ball.direction == 0) {
            ball.y--;
        } else if (ball.direction == 1) {
            ball.x++;
        } else if (ball.direction == 2) {
            ball.y++;
        } else if (ball.direction == 3) {
            ball.x--;
        }

        SDL_Event exit;
        while(SDL_PollEvent(&exit)){
            if(exit.type == SDL_QUIT){
                cleanup(win,renderer,backgroundTexture,backgroundSurface);
            }
        }

        drawOrangeBall(ball, N);
        renderDiagonals(N, grid);

        DelayTa3na(200);

        //initRender();
    } while (ball.x > 0 && ball.x < N-1 && ball.y > 0 && ball.y < N-1);
    SDL_Delay(500);
}



void animationTrajectoire(int N, int grid[N][N], Ball ball,bool * homeButton) {
    int orientation;
    bool goHome = (*homeButton) = false;

    SDL_Delay(200);
    renderDiagonals(N, grid);
    SDL_Delay(500);
    do {
        orientation = grid[ball.y][ball.x];
        //initRender();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderMatrix(N, grid);

        if (orientation == DIAGONAL_LEFT_TO_RIGHT) {
            if (ball.direction == 0) {
                ball.direction = 1;
            } else if (ball.direction == 1) {
                ball.direction = 0;
            } else if (ball.direction == 2) {
                ball.direction = 3;
            } else if (ball.direction == 3) {
                ball.direction = 2;
            }
            //Mix_PlayChannel(-1, bonk, 0);
        } else if (orientation == DIAGONAL_RIGHT_TO_LEFT) {
            if (ball.direction == 0) {
                ball.direction = 3;
            } else if (ball.direction == 1) {
                ball.direction = 2;
            } else if (ball.direction == 2) {
                ball.direction = 1;
            } else if (ball.direction == 3) {
                ball.direction = 0;
            }
            //Mix_PlayChannel(-1, bonk, 0);
        }

        if (ball.direction == 0) {
            ball.y--;
        } else if (ball.direction == 1) {
            ball.x++;
        } else if (ball.direction == 2) {
            ball.y++;
        } else if (ball.direction == 3) {
            ball.x--;
        }

        SDL_Event exit;
        while(SDL_PollEvent(&exit)){
            if(exit.type == SDL_QUIT){
                cleanup(win,renderer,backgroundTexture,backgroundSurface);
            }else if(exit.type == SDL_MOUSEBUTTONDOWN){
                int mouseX = exit.button.x;
                int mouseY = exit.button.y;
                if(isInsideRect(mouseX, mouseY, (SDL_Rect){1000, 25, 200, 50})){
                    goHome = true;
                }
            }
        }

        drawOrangeBall(ball, N);
        renderDiagonals(N, grid);

        DelayTa3na(200);

    } while (!goHome && (ball.x > 0 && ball.x < N-1 && ball.y > 0 && ball.y < N-1 ));
    (*homeButton) = goHome;
    SDL_Delay(200);
}

void animationPlayerTrajectoire(int N, int grid[N][N], Ball ball,bool * homeButton,TTF_Font *font,Player player) {
    int orientation;
    bool goHome = (*homeButton) = false;

    SDL_Delay(200);
    renderDiagonals(N, grid);
    SDL_Delay(500);
    do {
        orientation = grid[ball.y][ball.x];
        //initRender();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderPlayerMatrix(font,N, grid,player);

        if (orientation == DIAGONAL_LEFT_TO_RIGHT) {
            if (ball.direction == 0) {
                ball.direction = 1;
            } else if (ball.direction == 1) {
                ball.direction = 0;
            } else if (ball.direction == 2) {
                ball.direction = 3;
            } else if (ball.direction == 3) {
                ball.direction = 2;
            }
            //Mix_PlayChannel(-1, bonk, 0);
        } else if (orientation == DIAGONAL_RIGHT_TO_LEFT) {
            if (ball.direction == 0) {
                ball.direction = 3;
            } else if (ball.direction == 1) {
                ball.direction = 2;
            } else if (ball.direction == 2) {
                ball.direction = 1;
            } else if (ball.direction == 3) {
                ball.direction = 0;
            }
            //Mix_PlayChannel(-1, bonk, 0);
        }

        if (ball.direction == 0) {
            ball.y--;
        } else if (ball.direction == 1) {
            ball.x++;
        } else if (ball.direction == 2) {
            ball.y++;
        } else if (ball.direction == 3) {
            ball.x--;
        }

        SDL_Event exit;
        while(SDL_PollEvent(&exit)){
            if(exit.type == SDL_QUIT){
                cleanup(win,renderer,backgroundTexture,backgroundSurface);
            }else if(exit.type == SDL_MOUSEBUTTONDOWN){
                int mouseX = exit.button.x;
                int mouseY = exit.button.y;
                if(isInsideRect(mouseX, mouseY, (SDL_Rect){1000, 25, 200, 50})){
                    goHome = true;
                }
            }
        }

        drawOrangeBall(ball, N);
        renderDiagonals(N, grid);

        DelayTa3na(200);

    } while (!goHome && (ball.x > 0 && ball.x < N-1 && ball.y > 0 && ball.y < N-1 ));
    (*homeButton) = goHome;
    SDL_Delay(200);
}

void animationAlexTrajectoire(int N, int grid[N][N], Ball ball,bool * homeButton,TTF_Font *font,int score,int level) {
    int orientation;
    bool goHome = (*homeButton) = false;

    SDL_Delay(200);
    renderDiagonals(N, grid);
    SDL_Delay(500);
    do {
        orientation = grid[ball.y][ball.x];
        //initRender();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderAlexMatrix(N, grid,font,score,level);

        if (orientation == DIAGONAL_LEFT_TO_RIGHT) {
            if (ball.direction == 0) {
                ball.direction = 1;
            } else if (ball.direction == 1) {
                ball.direction = 0;
            } else if (ball.direction == 2) {
                ball.direction = 3;
            } else if (ball.direction == 3) {
                ball.direction = 2;
            }
            //Mix_PlayChannel(-1, bonk, 0);
        } else if (orientation == DIAGONAL_RIGHT_TO_LEFT) {
            if (ball.direction == 0) {
                ball.direction = 3;
            } else if (ball.direction == 1) {
                ball.direction = 2;
            } else if (ball.direction == 2) {
                ball.direction = 1;
            } else if (ball.direction == 3) {
                ball.direction = 0;
            }
            //Mix_PlayChannel(-1, bonk, 0);
        }

        if (ball.direction == 0) {
            ball.y--;
        } else if (ball.direction == 1) {
            ball.x++;
        } else if (ball.direction == 2) {
            ball.y++;
        } else if (ball.direction == 3) {
            ball.x--;
        }

        SDL_Event exit;
        while(SDL_PollEvent(&exit)){
            if(exit.type == SDL_QUIT){
                cleanup(win,renderer,backgroundTexture,backgroundSurface);
            }else if(exit.type == SDL_MOUSEBUTTONDOWN){
                int mouseX = exit.button.x;
                int mouseY = exit.button.y;
                if(isInsideRect(mouseX, mouseY, (SDL_Rect){1000, 25, 200, 50})){
                    goHome = true;
                }
            }
        }

        drawOrangeBall(ball, N);
        renderDiagonals(N, grid);

        DelayTa3na(200);

    } while (!goHome && (ball.x > 0 && ball.x < N-1 && ball.y > 0 && ball.y < N-1 ));
    (*homeButton) = goHome;
    SDL_Delay(200);
}

// a modifier
void animationAvantDebut(int N, int grid[N][N], Ball copyBall)
{
    Ball ball = copyBall;
    int i, j;
    SDL_Delay(200);

    copyBall.x = copyBall.y = 0;
    ball.x = ball.y = N - 1;
    for (i = 0; i < N; i++)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        renderMatrix(N, grid);
        drawBall(copyBall, N);
        drawBall(ball, N);
        SDL_RenderPresent(renderer);
        DelayTa3na(40);
        copyBall.x++;
        ball.x--;
        SDL_RenderClear(renderer);
    }
    ball.x = 0;
    ball.y = N - 1;
    copyBall.x = N - 1;
    copyBall.y = 0;
    for (i = 0; i < N; i++)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        renderMatrix(N, grid);
        drawBall(copyBall, N);
        drawBall(ball, N);
        SDL_RenderPresent(renderer);
        DelayTa3na(40);
        copyBall.x--;
        ball.x++;
        SDL_RenderClear(renderer);
    }
    copyBall.x = copyBall.y = 0;
    ball.x = ball.y = N - 1;
    for (i = 0; i < N; i++)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        renderMatrix(N, grid);
        drawBall(copyBall, N);
        drawBall(ball, N);
        SDL_RenderPresent(renderer);
        DelayTa3na(40);
        copyBall.y++;
        ball.y--;
        SDL_RenderClear(renderer);
    }
    ball.y = 0;
    ball.x = N - 1;
    copyBall.y = N - 1;
    copyBall.x = 0;
    for (i = 0; i < N; i++)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        renderMatrix(N, grid);
        drawBall(copyBall, N);
        drawBall(ball, N);
        SDL_RenderPresent(renderer);
        DelayTa3na(40);
        copyBall.y--;
        ball.y++;
        SDL_RenderClear(renderer);
    }

    DelayTa3na(40);
}

SDL_Texture *characterTextures[CHARACTER_IMAGE_COUNT];

void initializeAnimation()
{
    // Chargement des textures du personnage
    for (int i = 0; i < CHARACTER_IMAGE_COUNT; ++i)
    {
        char imagePath[100];
        snprintf(imagePath, sizeof(imagePath), "%s/walking/%d.png", CHARACTER_PATH_PREFIX, i + 1);
        characterTextures[i] = IMG_LoadTexture(renderer, imagePath);
    }
}

// Animation avant debut du jeu
void animationStartGame(_Bool *quitTest, TTF_Font *StartFont,TTF_Font *font, bool isHovered)
{

    static int frame = 0;               // Variable statique pour conserver l'état de la frame actuelle
    static int xPos = -CHARACTER_WIDTH; // Position initiale en X en dehors de l'écran

    SDL_RenderClear(renderer);

    SDL_Surface *backgroundSurface = IMG_Load("photo/background/start.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    SDL_Rect destinationRect = {xPos, SCREEN_HEIGHT - CHARACTER_HEIGHT - 70, CHARACTER_WIDTH, CHARACTER_HEIGHT};

    // Affichage de la texture correspondant à la frame actuelle
    SDL_RenderCopy(renderer, characterTextures[frame], NULL, &destinationRect);

    SDL_Surface *backgroundSurface2 = IMG_Load("photo/background/start2.png");

    if (backgroundSurface2)
    {
        SDL_Texture *backgroundTexture2 = SDL_CreateTextureFromSurface(renderer, backgroundSurface2);
        SDL_Rect backgroundRect2 = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture2, NULL, &backgroundRect2);

        SDL_DestroyTexture(backgroundTexture2);
        SDL_FreeSurface(backgroundSurface2);
    }

    SDL_Color textColor,textColor2;
    textColor2.r = textColor2.g = textColor2.b = textColor2.a= 255;
    if (isHovered)
    {
        textColor.r = 236;
        textColor.g = 90;
        textColor.b = 90;
    }
    else
    {
        textColor.r = 255;
        textColor.g = 237;
        textColor.b = 130;
    }

    SDL_Surface *messageSurface = TTF_RenderText_Solid(StartFont, "The MindMaze", textColor);
    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
    SDL_Rect messageRect;
    messageRect.x = (1150 - messageSurface->w) / 2;
    messageRect.y = 225;
    messageRect.w = messageSurface->w;
    messageRect.h = messageSurface->h;
    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);

    SDL_Surface *messageSurface2 = TTF_RenderText_Solid(font, "Alex vs Nexus", textColor2);
    SDL_Texture *messageTexture2 = SDL_CreateTextureFromSurface(renderer, messageSurface2);
    SDL_Rect messageRect2;
    messageRect2.x = (1150 - messageSurface2->w) / 2;
    messageRect2.y = 320;
    messageRect2.w = messageSurface2->w;
    messageRect2.h = messageSurface2->h;
    SDL_RenderCopy(renderer, messageTexture2, NULL, &messageRect2);
    SDL_FreeSurface(messageSurface2);
    SDL_DestroyTexture(messageTexture2);

    SDL_RenderPresent(renderer);

    // Changement de frame pour l'animation de marche
    frame = (frame + 1) % CHARACTER_IMAGE_COUNT;

    // Déplacement du personnage vers la droite
    xPos += 20; // Ajuste cette valeur pour contrôler la vitesse du déplacement

    // Réinitialisation de la position une fois qu'il atteint le côté droit
    if (xPos > SCREEN_WIDTH)
    {
        xPos = -CHARACTER_WIDTH;
        //*quitTest = 1;
    }
}

SDL_Texture *character[9];

void initializeBounadem()
{
    // Chargement des textures du personnage
    for (int i = 0; i < 9; ++i)
    {
        char imagePath[100];
        snprintf(imagePath, sizeof(imagePath), "%s/stand/%d.png", CHARACTER_PATH_PREFIX, i + 1);
        characterTextures[i] = IMG_LoadTexture(renderer, imagePath);
    }
}

void renderFixedCharacter(int i)
{
    // SDL_RenderClear(renderer);

    static int frame = 0; // Variable statique pour conserver l'état de la frame actuelle

    SDL_Rect destinationRect = {SCREEN_WIDTH / 2 - CHARACTER_WIDTH / 2, SCREEN_HEIGHT / 2 - CHARACTER_HEIGHT / 2, CHARACTER_WIDTH, CHARACTER_HEIGHT};

    // Affichage de la texture correspondant à la frame actuelle
    SDL_RenderCopy(renderer, character[frame], NULL, &destinationRect);

    SDL_RenderPresent(renderer);

    // Changement de frame pour l'animation
    frame = (frame + 1) % 9;

    SDL_Delay(FRAME_DELAY);
}

/*--------------------------------------------------------*/
// input w hadok swala7
int isPointInsideRect(int x, int y, SDL_Rect rect)
{
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
}

void saveUsernameToArray(const char *username, int *count)
{
    for (int i = 0; i < *count; ++i)
    {
        if (strcmp(usernames[i], username) == 0)
        {
            return;
        }
    }

    if (*count < MAX_USERNAMES - 1)
    { // Ensure there's space in the array
        usernames[*count] = strdup(username);
        (*count)++;
    }
}

void freeUsernamesArray(int count)
{
    for (int i = 0; i < count; ++i)
    {
        free(usernames[i]);
    }
}

void renderUI(SDL_Renderer *renderer, SDL_Texture *bgTexture, TTF_Font *font, TTF_Font *confirmFont,
              SDL_Rect usernameRect, SDL_Rect confirmButtonRect, const char *username,
              int isConfirmClicked, int isButtonHovered)
{
    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);
    SDL_Surface *backgroundSurface = IMG_Load("photo/background/inputbackground.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }
    // Set the background
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

    SDL_Color textColor = {255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Enter Player Username:", textColor);
    if (textSurface != NULL)
    {
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        SDL_Rect textRect;
        textRect.x = (1200 - textSurface->w) / 2;
        textRect.y = 180;
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
    }

    SDL_Color confirmColor = {255, 255, 255};
    if (isConfirmClicked)
    {
        confirmColor.r = 188;
        confirmColor.g = 163;
        confirmColor.b = 222;
    }
    else if (isButtonHovered)
    {
        confirmColor.r = 147;
        confirmColor.g = 55;
        confirmColor.b = 252;
    }

    SDL_Surface *confirmSurface = TTF_RenderText_Solid(confirmFont, "Confirm", confirmColor);
    if (confirmSurface != NULL)
    {
        SDL_Texture *confirmTexture = SDL_CreateTextureFromSurface(renderer, confirmSurface);
        SDL_FreeSurface(confirmSurface);
        SDL_Rect confirmRect;
        confirmRect.x = confirmButtonRect.x + 10;
        confirmRect.y = confirmButtonRect.y + 5;
        confirmRect.w = confirmSurface->w;
        confirmRect.h = confirmSurface->h;
        SDL_RenderCopy(renderer, confirmTexture, NULL, &confirmRect);
        SDL_DestroyTexture(confirmTexture);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &usernameRect);
    SDL_RenderDrawRect(renderer, &confirmButtonRect);
    SDL_Rect cursor = {usernameRect.x + 10 + (int)(strlen(username) * 15.1), 220, 2, 20};
    SDL_RenderFillRect(renderer, &cursor);

    SDL_Color usernameColor = {250, 250, 250};
    textSurface = TTF_RenderText_Solid(font, username, usernameColor);
    if (textSurface != NULL)
    {
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        SDL_Rect textRect;
        textRect.x = usernameRect.x + 10;
        textRect.y = usernameRect.y + 5;
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
    }

    SDL_RenderPresent(renderer);
}

void handleUsernameInput(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *confirmFont,SDL_Rect usernameRect, SDL_Rect confirmButtonRect, SDL_Texture *bgTexture, char *username, bool *quit, bool *isEscape)
{
    int done = 0;
    int isConfirmClicked = 0;
    int isButtonHovered = 0;

    SDL_StartTextInput();
    renderUI(renderer, bgTexture, font, confirmFont, usernameRect, confirmButtonRect, username, isConfirmClicked, isButtonHovered);
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                *quit = true;
                done = 1;
            }
            else if (event.type == SDL_TEXTINPUT && strlen(username) < 18)
            {
                strcat(username, event.text.text);
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    done = 1;
                    saveUsernameToArray(username, &usernamesCount);
                }
                else if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(username) > 0)
                {
                    username[strlen(username) - 1] = '\0';
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    *isEscape = true;
                    done = 1;
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                if (isPointInsideRect(mouseX, mouseY, confirmButtonRect))
                {
                    done = 1;
                    isConfirmClicked = 1;
                    saveUsernameToArray(username, &usernamesCount);
                }
            }

            if (event.type == SDL_MOUSEMOTION)
            {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;
                if (isPointInsideRect(mouseX, mouseY, confirmButtonRect))
                {
                    isButtonHovered = 1;
                }
                else
                {
                    isButtonHovered = 0;
                }
            }
            renderUI(renderer, bgTexture, font, confirmFont, usernameRect, confirmButtonRect, username, isConfirmClicked, isButtonHovered);
        }
        // renderUI(renderer, bgTexture, font, confirmFont, usernameRect, confirmButtonRect, username, isConfirmClicked, isButtonHovered);
        // SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();
}

/*--------------------------------------------------------*/
// cleaning function


// Function to draw an arc
void SDL_RenderDrawArc(SDL_Renderer *renderer, int x, int y, int radius, int startAngle, int endAngle)
{
    for (int i = startAngle; i <= endAngle; i++)
    {
        double angle = i * M_PI / 180.0;
        int xPos = x + radius * cos(angle);
        int yPos = y + radius * sin(angle);
        SDL_RenderDrawPoint(renderer, xPos, yPos);
    }
}

// Function to draw rounded rectangle
void SDL_RenderDrawRoundRect(SDL_Renderer *renderer, SDL_Rect *rect, int borderRadius)
{
    int x = rect->x;
    int y = rect->y;
    int w = rect->w;
    int h = rect->h;

    // Draw top line
    SDL_RenderDrawLine(renderer, x + borderRadius, y, x + w - borderRadius, y);

    // Draw top right corner
    SDL_RenderDrawArc(renderer, x + w - borderRadius, y + borderRadius, borderRadius, -90, 0);

    // Draw right line
    SDL_RenderDrawLine(renderer, x + w, y + borderRadius, x + w, y + h - borderRadius);

    // Draw bottom right corner
    SDL_RenderDrawArc(renderer, x + w - borderRadius, y + h - borderRadius, borderRadius, 0, 90);

    // Draw bottom line
    SDL_RenderDrawLine(renderer, x + borderRadius, y + h, x + w - borderRadius, y + h);

    // Draw bottom left corner
    SDL_RenderDrawArc(renderer, x + borderRadius, y + h - borderRadius, borderRadius, 90, 180);

    // Draw left line
    SDL_RenderDrawLine(renderer, x, y + borderRadius, x, y + h - borderRadius);

    // Draw top left corner
    SDL_RenderDrawArc(renderer, x + borderRadius, y + borderRadius, borderRadius, 180, 270);
}

// Modified DrawButton function
void DrawButton(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, int width, int height, SDL_Color textColor, SDL_Color borderColor)
{

    SDL_Rect buttonTextRect = {x, y, width, height};
    SDL_Surface *buttonTextSurface = TTF_RenderText_Solid(font, text, textColor);

    int textWidth, textHeight;
    TTF_SizeText(font, text, &textWidth, &textHeight);
    SDL_Texture *buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
    SDL_Rect textPosition = {x + (width - textWidth) / 2, y + (height - textHeight) / 2, textWidth, textHeight};

    // Draw rounded rectangle
    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRoundRect(renderer, &buttonTextRect, 15);

    // Reset the render draw color to the default color (if needed)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderCopy(renderer, buttonTextTexture, NULL, &textPosition);

    SDL_FreeSurface(buttonTextSurface);
    SDL_DestroyTexture(buttonTextTexture);
}

// function to check if its inside
int isInsideRect(int x, int y, SDL_Rect rect) {
     //return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
     if(x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h){
        return 1;
     }
    return 0;
}


// function to handle the button click
void handleButtonClick(const char *buttonName)
{
    printf("%s Button Clicked!\n", buttonName);
}

// HOME
void DrawHomeOn(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *fonthead, bool StartHovered, bool StoryHovered, bool HighScoreHovered)
{

    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);
    SDL_Surface *backgroundSurface = IMG_Load("photo/background/homeon.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    SDL_Color textColor1, textColor2, textColor3;
    SDL_Color ButtonColor1, ButtonColor2, ButtonColor3;

    if (StartHovered)
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 191;
        ButtonColor1.b = 0;
        textColor1.r = 255;
        textColor1.g = 191;
        textColor1.b = 0;
    }
    else
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 255;
        ButtonColor1.b = 255;
        textColor1.r = 255;
        textColor1.g = 255;
        textColor1.b = 255;
    }

    if (StoryHovered)
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 191;
        ButtonColor2.b = 0;
        textColor2.r = 255;
        textColor2.g = 191;
        textColor2.b = 0;
    }
    else
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 255;
        ButtonColor2.b = 255;
        ButtonColor2.a = 255;
        textColor2.r = 255;
        textColor2.g = 255;
        textColor2.b = 255;
        textColor2.a = 255;
    }

    if (HighScoreHovered)
    {
        ButtonColor3.r = 255;
        ButtonColor3.g = 191;
        ButtonColor3.b = 0;
        textColor3.r = 255;
        textColor3.g = 191;
        textColor3.b = 0;
    }
    else
    {
        ButtonColor3.r = 255;
        ButtonColor3.g = 255;
        ButtonColor3.b = 255;
        textColor3.r = 255;
        textColor3.g = 255;
        textColor3.b = 255;
    }
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface *messageSurface = TTF_RenderText_Solid(fonthead, "Welcome to our game", textColor);
    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
    SDL_Rect messageRect;
    messageRect.x = (1150 - messageSurface->w) / 2; // Centered horizontally
    messageRect.y = 225;                            // Above the buttons
    messageRect.w = messageSurface->w;
    messageRect.h = messageSurface->h;
    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);

    // BUTTONS
    DrawButton(renderer, font, "Start", ((1200 - 0.4 * 950) / 2), (380 - 45), (0.4 * 800), 45, textColor1, ButtonColor1);
    DrawButton(renderer, font, "Story", ((1200 - 0.4 * 950) / 2), (460 - 45), (0.4 * 800), 45, textColor2, ButtonColor2);
    DrawButton(renderer, font, "High Score", ((1200 - 0.4 * 950) / 2), (540 - 45), (0.4 * 800), 45, textColor3, ButtonColor3);
}

void DrawHomeOff(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *fonthead, bool StartHovered, bool StoryHovered, bool HighScoreHovered)
{

    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);
    SDL_Surface *backgroundSurface = IMG_Load("photo/background/homeoff.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    SDL_Color textColor1, textColor2, textColor3;
    SDL_Color ButtonColor1, ButtonColor2, ButtonColor3;

    if (StartHovered)
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 191;
        ButtonColor1.b = 0;
        textColor1.r = 255;
        textColor1.g = 191;
        textColor1.b = 0;
    }
    else
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 255;
        ButtonColor1.b = 255;
        textColor1.r = 255;
        textColor1.g = 255;
        textColor1.b = 255;
    }

    if (StoryHovered)
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 191;
        ButtonColor2.b = 0;
        textColor2.r = 255;
        textColor2.g = 191;
        textColor2.b = 0;
    }
    else
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 255;
        ButtonColor2.b = 255;
        ButtonColor2.a = 255;
        textColor2.a = 255;
        textColor2.r = 255;
        textColor2.g = 255;
        textColor2.b = 255;
    }

    if (HighScoreHovered)
    {
        ButtonColor3.r = 255;
        ButtonColor3.g = 191;
        ButtonColor3.b = 0;
        textColor3.r = 255;
        textColor3.g = 191;
        textColor3.b = 0;
    }
    else
    {
        ButtonColor3.r = 255;
        ButtonColor3.g = 255;
        ButtonColor3.b = 255;
        textColor3.r = 255;
        textColor3.g = 255;
        textColor3.b = 255;
    }
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface *messageSurface = TTF_RenderText_Solid(fonthead, "Welcome to our game", textColor);
    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
    SDL_Rect messageRect;
    messageRect.x = (1150 - messageSurface->w) / 2; // Centered horizontally
    messageRect.y = 225;                            // Above the buttons
    messageRect.w = messageSurface->w;
    messageRect.h = messageSurface->h;
    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);

    // BUTTONS
    DrawButton(renderer, font, "Start", ((1200 - 0.4 * 950) / 2), (380 - 45), (0.4 * 800), 45, textColor1, ButtonColor1);
    DrawButton(renderer, font, "Story", ((1200 - 0.4 * 950) / 2), (460 - 45), (0.4 * 800), 45, textColor2, ButtonColor2);
    DrawButton(renderer, font, "High Score", ((1200 - 0.4 * 950) / 2), (540 - 45), (0.4 * 800), 45, textColor3, ButtonColor3);
}

// HELP PAGE
void DrawHelpOneofOne(SDL_Renderer *renderer, TTF_Font *font)
{

    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);
    SDL_Surface *backgroundSurface = IMG_Load("photo/help/one/1.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    const char *filename = "text/help1.txt";
    FILE *file = fopen(filename, "r");
    SDL_Color textColor = {250, 250, 250};
    SDL_Color textColor1 = {2, 143, 118};
    if (file)
    {
        char text[8192] = {0};
        char line[1024];

        while (fgets(line, sizeof(line), file))
        {
            strcat(text, line);
        }
        fclose(file);

        SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font, text, textColor, 600);

        if (textSurface)
        {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {(1200 - textSurface->w) / 2, 100, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}

void DrawHelpTwoofOne(SDL_Renderer *renderer, TTF_Font *font)
{

    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);
    SDL_Surface *backgroundSurface = IMG_Load("photo/help/one/2.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    const char *filename = "text/help1.txt";
    FILE *file = fopen(filename, "r");
    SDL_Color textColor = {250, 250, 250};
    SDL_Color textColor1 = {2, 143, 118};
    if (file)
    {
        char text[8192] = {0};
        char line[1024];

        while (fgets(line, sizeof(line), file))
        {
            strcat(text, line);
        }
        fclose(file);

        SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font, text, textColor, 600);

        if (textSurface)
        {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {(1200 - textSurface->w) / 2, 100, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}

void DrawHelpThreeofOne(SDL_Renderer *renderer, TTF_Font *font)
{

    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);
    SDL_Surface *backgroundSurface = IMG_Load("photo/help/one/3.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    const char *filename = "text/help1.txt";
    FILE *file = fopen(filename, "r");
    SDL_Color textColor = {250, 250, 250};
    SDL_Color textColor1 = {2, 143, 118};
    if (file)
    {
        char text[8192] = {0};
        char line[1024];

        while (fgets(line, sizeof(line), file))
        {
            strcat(text, line);
        }
        fclose(file);

        SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font, text, textColor, 600);

        if (textSurface)
        {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {(1200 - textSurface->w) / 2, 100, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}

void DrawHelpFourofOne(SDL_Renderer *renderer, TTF_Font *font)
{

    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);
    SDL_Surface *backgroundSurface = IMG_Load("photo/help/one/4.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    const char *filename = "text/help1.txt";
    FILE *file = fopen(filename, "r");
    SDL_Color textColor = {250, 250, 250};
    SDL_Color textColor1 = {2, 143, 118};
    if (file)
    {
        char text[8192] = {0};
        char line[1024];

        while (fgets(line, sizeof(line), file))
        {
            strcat(text, line);
        }
        fclose(file);

        SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font, text, textColor, 600);

        if (textSurface)
        {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {(1200 - textSurface->w) / 2, 100, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}

// HELP PAGE
void DrawHelpOneofTwo(SDL_Renderer *renderer, TTF_Font *font)
{

    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);
    SDL_Surface *backgroundSurface = IMG_Load("photo/help/two/1.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    const char *filename = "text/help2.txt";
    FILE *file = fopen(filename, "r");
    SDL_Color textColor = {250, 250, 250};
    SDL_Color textColor1 = {2, 143, 118};
    if (file)
    {
        char text[8192] = {0};
        char line[1024];

        while (fgets(line, sizeof(line), file))
        {
            strcat(text, line);
        }
        fclose(file);

        SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font, text, textColor, 600);

        if (textSurface)
        {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {(1200 - textSurface->w) / 2, 100, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}

// HELP PAGE
void DrawHelpOneofThree(SDL_Renderer *renderer, TTF_Font *font)
{

    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);
    SDL_Surface *backgroundSurface = IMG_Load("photo/help/three/1.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    const char *filename = "text/help3.txt";
    FILE *file = fopen(filename, "r");
    SDL_Color textColor = {250, 250, 250};
    SDL_Color textColor1 = {2, 143, 118};
    if (file)
    {
        char text[8192] = {0};
        char line[1024];

        while (fgets(line, sizeof(line), file))
        {
            strcat(text, line);
        }
        fclose(file);

        SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font, text, textColor, 600);

        if (textSurface)
        {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {150, 80, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}

void DrawHelpTwoofThree(SDL_Renderer *renderer, TTF_Font *font)
{

    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);
    SDL_Surface *backgroundSurface = IMG_Load("photo/help/three/2.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }
    const char *filename = "text/help3.txt";
    FILE *file = fopen(filename, "r");
    SDL_Color textColor = {250, 250, 250};
    SDL_Color textColor1 = {2, 143, 118};
    if (file)
    {
        char text[8192] = {0};
        char line[1024];

        while (fgets(line, sizeof(line), file))
        {
            strcat(text, line);
        }
        fclose(file);

        SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font, text, textColor, 600);

        if (textSurface)
        {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_Rect textRect = {150, 80, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}

// HIGHSCORE PAGE
void DrawHighScoreOff(SDL_Renderer *renderer, TTF_Font *font, Player joueurs[], int nombreDeJoueurs, bool EchapHovered)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Charger l'image de fond
    SDL_Surface *backgroundSurface = IMG_Load("photo/background/highscoreoff.png");
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    // Dessiner l'image de fond
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    SDL_Color textColor1, ButtonColor1; // Couleur du texte

    textColor1.r = 255;
    textColor1.g = 255;
    textColor1.b = 255;

    int x = 250;
    int y = 230;
    int textSize = 25;

    //username
    for (int i = 0; i < nombreDeJoueurs && i < 5; ++i)
    {
        char joueurText[100];
        snprintf(joueurText, sizeof(joueurText), "%d. %s ", i+1,joueurs[i].username);

        SDL_Surface *textSurface = TTF_RenderText_Solid(font, joueurText, textColor1);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        y += textSize + 15;
    }

    //niveau
    x = 460;
    y = 230;
    for (int i = 0; i < nombreDeJoueurs && i < 5; ++i)
    {
        char joueurText[100];
        snprintf(joueurText, sizeof(joueurText), "Niveau %d ", joueurs[i].Niveau);

        SDL_Surface *textSurface = TTF_RenderText_Solid(font, joueurText, textColor1);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        y += textSize + 15;
    }

    //score
    x = 600;
    y = 230;
    for (int i = 0; i < nombreDeJoueurs && i < 5; ++i)
    {
        char joueurText[100];
        snprintf(joueurText, sizeof(joueurText), "Score : %d",joueurs[i].score);

        SDL_Surface *textSurface = TTF_RenderText_Solid(font, joueurText, textColor1);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        y += textSize + 15;
    }

    //date
    x = 770;
    y = 230;
    for (int i = 0; i < nombreDeJoueurs && i < 5; ++i)
    {
        char joueurText[100];
        snprintf(joueurText, sizeof(joueurText), "Date : %02d/%02d/%d",joueurs[i].date.jour, joueurs[i].date.mois, joueurs[i].date.annee);

        SDL_Surface *textSurface = TTF_RenderText_Solid(font, joueurText, textColor1);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        y += textSize + 15;
    }

    if (EchapHovered)
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 191;
        ButtonColor1.b = 0;
        textColor1.r = 255;
        textColor1.g = 191;
        textColor1.b = 0;
    }
    else
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 255;
        ButtonColor1.b = 255;
        textColor1.r = 255;
        textColor1.g = 255;
        textColor1.b = 255;
    }

    DrawButton(renderer, font, "Home", 525, 520, 120, 40, textColor1, ButtonColor1);

    SDL_DestroyTexture(backgroundTexture); // Libérer la texture de l'image de fond
    SDL_RenderPresent(renderer);
}

// HIGHSCORE PAGE
void DrawHighScoreOn(SDL_Renderer *renderer, TTF_Font *font, Player joueurs[], int nombreDeJoueurs, bool EchapHovered)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Charger l'image de fond
    SDL_Surface *backgroundSurface = IMG_Load("photo/background/highscoreon.png");
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    // Dessiner l'image de fond
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_Color textColor1, ButtonColor1; // Couleur du texte

    textColor1.r = 255;
    textColor1.g = 255;
    textColor1.b = 255;

    int x = 250;
    int y = 230;
    int textSize = 25;

    //username
    for (int i = 0; i < nombreDeJoueurs && i < 5; ++i)
    {
        char joueurText[100];
        snprintf(joueurText, sizeof(joueurText), "%d. %s ", i+1,joueurs[i].username);

        SDL_Surface *textSurface = TTF_RenderText_Solid(font, joueurText, textColor1);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        y += textSize + 15;
    }

    //niveau
    x = 460;
    y = 230;
    for (int i = 0; i < nombreDeJoueurs && i < 5; ++i)
    {
        char joueurText[100];
        snprintf(joueurText, sizeof(joueurText), "Niveau %d ", joueurs[i].Niveau);

        SDL_Surface *textSurface = TTF_RenderText_Solid(font, joueurText, textColor1);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        y += textSize + 15;
    }

    //score
    x = 600;
    y = 230;
    for (int i = 0; i < nombreDeJoueurs && i < 5; ++i)
    {
        char joueurText[100];
        snprintf(joueurText, sizeof(joueurText), "Score : %d",joueurs[i].score);

        SDL_Surface *textSurface = TTF_RenderText_Solid(font, joueurText, textColor1);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        y += textSize + 15;
    }

    //date
    x = 770;
    y = 230;
    for (int i = 0; i < nombreDeJoueurs && i < 5; ++i)
    {
        char joueurText[100];
        snprintf(joueurText, sizeof(joueurText), "Date : %02d/%02d/%d",joueurs[i].date.jour, joueurs[i].date.mois, joueurs[i].date.annee);

        SDL_Surface *textSurface = TTF_RenderText_Solid(font, joueurText, textColor1);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        y += textSize + 15;
    }


    if (EchapHovered)
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 191;
        ButtonColor1.b = 0;
        textColor1.r = 255;
        textColor1.g = 191;
        textColor1.b = 0;
    }
    else
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 255;
        ButtonColor1.b = 255;
        textColor1.r = 255;
        textColor1.g = 255;
        textColor1.b = 255;
    }

    DrawButton(renderer, font, "Home", 525, 520, 120, 40, textColor1, ButtonColor1);

    SDL_DestroyTexture(backgroundTexture); // Libérer la texture de l'image de fond
    SDL_RenderPresent(renderer);
}

// MODE PAGE
void drawmodewin(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *fonthead, bool machinehovered, bool playerhovered)
{
    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);

    SDL_Surface *backgroundSurface = IMG_Load("photo/background.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    SDL_Color textColor = {255, 255, 255};

    SDL_Color textColor1, textColor2;
    SDL_Color ButtonColor1, ButtonColor2;

    if (machinehovered)
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 191;
        ButtonColor1.b = 0;
        textColor1.r = 255;
        textColor1.g = 191;
        textColor1.b = 0;
    }
    else
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 255;
        ButtonColor1.b = 255;
        textColor1.r = 255;
        textColor1.g = 255;
        textColor1.b = 255;
    }

    if (playerhovered)
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 191;
        ButtonColor2.b = 0;
        textColor2.r = 255;
        textColor2.g = 191;
        textColor2.b = 0;
    }
    else
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 255;
        ButtonColor2.b = 255;
        textColor2.r = 255;
        textColor2.g = 255;
        textColor2.b = 255;
    }
    // the 2 buttons
    DrawButton(renderer, font, "Machine", 350, 350, 200, 50, textColor1, ButtonColor1);
    DrawButton(renderer, font, "Player", 600, 350, 200, 50, textColor2, ButtonColor2);

    SDL_Surface *messageSurface = TTF_RenderText_Solid(fonthead, "Choose the game mode", textColor);
    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
    SDL_Rect messageRect;
    messageRect.x = (1150 - messageSurface->w) / 2; // Centered horizontally
    messageRect.y = 250;                            // Above the buttons
    messageRect.w = messageSurface->w;
    messageRect.h = messageSurface->h;
    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);
}

void drawMachineTypeWin(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *fonthead, bool machinemachinehovered, bool humainhovered)
{
    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);

    SDL_Surface *backgroundSurface = IMG_Load("photo/background/ma.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    SDL_Color textColor = {0, 0, 0};

    SDL_Color textColor1, textColor2;
    SDL_Color ButtonColor1, ButtonColor2;

    if (machinemachinehovered)
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 191;
        ButtonColor1.b = 0;
        textColor1.r = 255;
        textColor1.g = 191;
        textColor1.b = 0;
    }
    else
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 255;
        ButtonColor1.b = 255;
        textColor1.r = 255;
        textColor1.g = 255;
        textColor1.b = 255;
    }

    if (humainhovered)
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 191;
        ButtonColor2.b = 0;
        textColor2.r = 255;
        textColor2.g = 191;
        textColor2.b = 0;
    }
    else
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 255;
        ButtonColor2.b = 255;
        textColor2.r = 255;
        textColor2.g = 255;
        textColor2.b = 255;
    }
    // the 2 buttons
    DrawButton(renderer, font, "Nexus", 350, 350, 200, 50, textColor1, ButtonColor1);
    DrawButton(renderer, font, "Alex", 600, 350, 200, 50, textColor2, ButtonColor2);

    SDL_Surface *messageSurface = TTF_RenderText_Solid(fonthead, "Nexus VS ..", textColor);
    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
    SDL_Rect messageRect;
    messageRect.x = (1150 - messageSurface->w) / 2; // Centered horizontally
    messageRect.y = 250;                            // Above the buttons
    messageRect.w = messageSurface->w;
    messageRect.h = messageSurface->h;
    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);
}

// CONTINUE PAGE
void drawcontinewin(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *fonthead, bool continuehovered, bool newgamehovered)
{
    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);

    SDL_Surface *backgroundSurface = IMG_Load("photo/background/background.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }
    SDL_Color textColor = {255, 255, 255};
    SDL_Color textColor1, textColor2;
    SDL_Color ButtonColor1, ButtonColor2;
    if (continuehovered)
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 191;
        ButtonColor1.b = 0;
        textColor1.r = 255;
        textColor1.g = 191;
        textColor1.b = 0;
    }
    else
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 255;
        ButtonColor1.b = 255;
        textColor1.r = 255;
        textColor1.g = 255;
        textColor1.b = 255;
    }

    if (newgamehovered)
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 191;
        ButtonColor2.b = 0;
        textColor2.r = 255;
        textColor2.g = 191;
        textColor2.b = 0;
    }
    else
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 255;
        ButtonColor2.b = 255;
        textColor2.r = 255;
        textColor2.g = 255;
        textColor2.b = 255;
    }

    // the 2 buttons
    DrawButton(renderer, font, "Continue", 600, 350, 200, 50, textColor1, ButtonColor1);
    DrawButton(renderer, font, "New Game", 350, 350, 200, 50, textColor2, ButtonColor2);

    SDL_Surface *messageSurface = TTF_RenderText_Solid(fonthead, "Would you like to?", textColor);
    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
    SDL_Rect messageRect;
    messageRect.x = (1150 - messageSurface->w) / 2; // Centered horizontally
    messageRect.y = 250;                            // Above the buttons
    messageRect.w = messageSurface->w;
    messageRect.h = messageSurface->h;
    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);
}

// machine continue or no
void drawContineMachinewin(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *fonthead, bool continhovered, bool homehovered)
{
    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);

    SDL_Surface *backgroundSurface = IMG_Load("photo/background/inputbackground.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }
    SDL_Color textColor = {255, 255, 255};
    SDL_Color textColor1, textColor2;
    SDL_Color ButtonColor1, ButtonColor2;
    if (continhovered)
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 191;
        ButtonColor1.b = 0;
        textColor1.r = 255;
        textColor1.g = 191;
        textColor1.b = 0;
    }
    else
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 255;
        ButtonColor1.b = 255;
        textColor1.r = 255;
        textColor1.g = 255;
        textColor1.b = 255;
    }

    if (homehovered)
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 191;
        ButtonColor2.b = 0;
        textColor2.r = 255;
        textColor2.g = 191;
        textColor2.b = 0;
    }
    else
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 255;
        ButtonColor2.b = 255;
        textColor2.r = 255;
        textColor2.g = 255;
        textColor2.b = 255;
    }

    // the 2 buttons
    DrawButton(renderer, font, "Go Home", 350, 350, 200, 50, textColor1, ButtonColor1);
    DrawButton(renderer, font, "Continue", 600, 350, 200, 50, textColor2, ButtonColor2);

    SDL_Surface *messageSurface = TTF_RenderText_Solid(fonthead, "Would you like to?", textColor);
    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
    SDL_Rect messageRect;
    messageRect.x = (1150 - messageSurface->w) / 2; // Centered horizontally
    messageRect.y = 250;                            // Above the buttons
    messageRect.w = messageSurface->w;
    messageRect.h = messageSurface->h;
    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);
}

// oldornewWin PAGE
void oldornewWin(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *fonthead, bool oldhovered, bool newhovered)
{
    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);

    SDL_Surface *backgroundSurface = IMG_Load("photo/background/oldornew.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }
    SDL_Color textColor = {255, 255, 255};
    SDL_Color textColor1, textColor2;
    SDL_Color ButtonColor1, ButtonColor2;
    if (oldhovered)
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 191;
        ButtonColor1.b = 0;
        textColor1.r = 255;
        textColor1.g = 191;
        textColor1.b = 0;
    }
    else
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 255;
        ButtonColor1.b = 255;
        textColor1.r = 255;
        textColor1.g = 255;
        textColor1.b = 255;
    }

    if (newhovered)
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 191;
        ButtonColor2.b = 0;
        textColor2.r = 255;
        textColor2.g = 191;
        textColor2.b = 0;
    }
    else
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 255;
        ButtonColor2.b = 255;
        textColor2.r = 255;
        textColor2.g = 255;
        textColor2.b = 255;
    }
    // the 2 buttons
    DrawButton(renderer, font, "Old Player", 600, 350, 200, 50, textColor1, ButtonColor1);
    DrawButton(renderer, font, "New Player", 350, 350, 200, 50, textColor2, ButtonColor2);

    SDL_Surface *messageSurface = TTF_RenderText_Solid(fonthead, "Are you ..", textColor);
    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
    SDL_Rect messageRect;
    messageRect.x = (1150 - messageSurface->w) / 2; // Centered horizontally
    messageRect.y = 250;                            // Above the buttons
    messageRect.w = messageSurface->w;
    messageRect.h = messageSurface->h;
    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);
}

// matrix page
void drawmatrix(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *fonthead)
{
    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);

    SDL_Surface *backgroundSurface = IMG_Load("photo/background.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }

    SDL_Color textColor = {255, 255, 255};
    SDL_Color textColor1 = {2, 143, 118};

    // Draw a filled rectangle
    SDL_Rect rect = {500, 100, 450, 450};               // Example coordinates and size
    SDL_SetRenderDrawColor(renderer, 225, 84, 69, 255); // Set color to white
    SDL_RenderFillRect(renderer, &rect);
}

// pause page
void drawpause(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *fonthead, bool continhovered, bool homehovered)
{
    SDL_SetRenderDrawColor(renderer, 37, 37, 37, 255);
    SDL_RenderClear(renderer);

    SDL_Surface *backgroundSurface = IMG_Load("photo/background/inputbackground.png");

    if (backgroundSurface)
    {
        SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, 1200, 650};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        SDL_DestroyTexture(backgroundTexture);
        SDL_FreeSurface(backgroundSurface);
    }
    SDL_Color textColor = {255, 255, 255};
    SDL_Color textColor1, textColor2;
    SDL_Color ButtonColor1, ButtonColor2;
    if (continhovered)
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 191;
        ButtonColor1.b = 0;
        textColor1.r = 255;
        textColor1.g = 191;
        textColor1.b = 0;
    }
    else
    {
        ButtonColor1.r = 255;
        ButtonColor1.g = 255;
        ButtonColor1.b = 255;
        textColor1.r = 255;
        textColor1.g = 255;
        textColor1.b = 255;
    }

    if (homehovered)
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 191;
        ButtonColor2.b = 0;
        textColor2.r = 255;
        textColor2.g = 191;
        textColor2.b = 0;
    }
    else
    {
        ButtonColor2.r = 255;
        ButtonColor2.g = 255;
        ButtonColor2.b = 255;
        textColor2.r = 255;
        textColor2.g = 255;
        textColor2.b = 255;
    }
    // the 2 buttons
    DrawButton(renderer, font, "continue", 350, 350, 200, 50, textColor1,  ButtonColor1);
    DrawButton(renderer, font, "home", 600, 350, 200, 50, textColor2,  ButtonColor2);

    SDL_Surface *messageSurface = TTF_RenderText_Solid(fonthead, "Would you like to?", textColor);
    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(renderer, messageSurface);
    SDL_Rect messageRect;
    messageRect.x = (1150 - messageSurface->w) / 2; // Centered horizontally
    messageRect.y = 250;                            // Above the buttons
    messageRect.w = messageSurface->w;
    messageRect.h = messageSurface->h;
    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);
    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);
}

SDL_Color get_pixel_color_texture(SDL_Renderer *renderer, int x, int y)
{
    SDL_Color color = {0, 0, 0, 0}; // Initialize with default values

    // Create a 1x1 surface
    SDL_Surface *surface = SDL_CreateRGBSurface(0, 1, 1, 32, 0, 0, 0, 0);

    // Read the pixels directly from the renderer at the specified position
    SDL_RenderReadPixels(renderer, &(SDL_Rect){x, y, 1, 1}, surface->format->format, surface->pixels, surface->pitch);

    // Get the color of the pixel from the surface
    SDL_GetRGB(*(Uint32 *)surface->pixels, surface->format, &color.r, &color.g, &color.b);

    // Free the temporary surface
    SDL_FreeSurface(surface);

    return color;
}

void createMatrixWithPixel(int N, SDL_Renderer *renderer, int gridcopy[N][N])
{
    int size = 420;
    int offsetX = (SCREEN_WIDTH - size) / 2;
    int offsetY = (SCREEN_HEIGHT - size) / 2;
    int CELL_SIZE = size / N;
    int correctionX = (size % N) / 2;
    int correctionY = (size % N) / 2;
    int X,Y,X1,Y1;

    for (int i = 1; i < N - 1; i++)
    {
        for (int j = 1; j < N - 1; j++)
        {
            if(N<11)
            {
                X = offsetX + j * CELL_SIZE + CELL_SIZE / 2 + correctionX;
                Y = offsetY + i * CELL_SIZE + CELL_SIZE / 2 + correctionY;
                X1 = offsetX + j * CELL_SIZE + CELL_SIZE / 4 + correctionX;
                Y1 = offsetY + i * CELL_SIZE + CELL_SIZE / 4 + correctionY;
            }
            else if(N==11)
            {
                X = offsetX + j * CELL_SIZE + CELL_SIZE / 2 + correctionX;
                Y = offsetY + i * CELL_SIZE + CELL_SIZE / 2 + correctionY;
                X1 = offsetX + j * CELL_SIZE + CELL_SIZE / 4 + correctionX+1;
                Y1 = offsetY + i * CELL_SIZE + CELL_SIZE / 4 + correctionY+1;
            }
            else if(N==12 || N==13)
            {
                X = offsetX + j * CELL_SIZE + CELL_SIZE / 2 + correctionX+1;
                Y = offsetY + i * CELL_SIZE + CELL_SIZE / 2 + correctionY+1;
                X1 = offsetX + j * CELL_SIZE + CELL_SIZE / 4 + correctionX+2;
                Y1 = offsetY + i * CELL_SIZE + CELL_SIZE / 4 + correctionY+2;
            }
            else if(N==14||N==15)
            {
                X = offsetX + j * CELL_SIZE + CELL_SIZE / 2 + correctionX+1;
                Y = offsetY + i * CELL_SIZE + CELL_SIZE / 2 + correctionY+1;
                X1 = offsetX + j * CELL_SIZE + CELL_SIZE / 4 + correctionX+3;
                Y1 = offsetY + i * CELL_SIZE + CELL_SIZE / 4 + correctionY+3;
            }else if(N==16||N==17)
            {
                X = offsetX + j * CELL_SIZE + CELL_SIZE / 2 + correctionX+1;
                Y = offsetY + i * CELL_SIZE + CELL_SIZE / 2 + correctionY+1;
                X1 = offsetX + j * CELL_SIZE + CELL_SIZE / 4 + correctionX+4;
                Y1 = offsetY + i * CELL_SIZE + CELL_SIZE / 4 + correctionY+4;
            }else if(N==18||N==19||N==20)
            {
                X = offsetX + j * CELL_SIZE + CELL_SIZE / 2 + correctionX+1;
                Y = offsetY + i * CELL_SIZE + CELL_SIZE / 2 + correctionY+1;
                X1 = offsetX + j * CELL_SIZE + CELL_SIZE / 4 + correctionX+5;
                Y1 = offsetY + i * CELL_SIZE + CELL_SIZE / 4 + correctionY+5;
            }

            SDL_Color pixelColor = get_pixel_color_texture(renderer, X, Y);
            SDL_Color pixelColor1 = get_pixel_color_texture(renderer, X1, Y1);

            if (pixelColor.r == 255 && pixelColor.g == 255 && pixelColor.b == 255)
            {
                if (pixelColor1.r == 255 && pixelColor1.g == 255 && pixelColor1.b == 255)
                {
                    gridcopy[i][j] = 2;
                }
                else
                {
                    gridcopy[i][j] = 1;
                }
            }
        }
    }
}


int main(int argc, char *argv[])
{

    // SDL Variable
    bool homeWindow = true;
    bool helpWindow = false;
    bool helpWindowOne = false;
    bool helpWindowTwo = false;
    bool helpWindowThree = false;
    bool highscoreWin = false;
    bool modewindow = false;
    bool oldornewwin = false;
    bool continwin = false;
    bool matrixwin = false;
    bool pausewin = false;
    bool userinputwin = false;
    bool machineWin = false;
    bool machinePlayerwin = false;
    bool playMachine;

    // Gestion Hover
    bool StartHovered = false;
    bool StoryHovered = false;
    bool HighScoreHovered = false;
    bool EchapHovered = false;
    bool playerhovered = false;
    bool machinehovered = false;
    bool machinemachinehovered = false;
    bool humainhovered = false;
    bool newhovered = false;
    bool oldhovered = false;
    bool continuehovered = false;
    bool newgamehovered = false;
    bool continhovered = false;
    bool homehovered = false;

    // Logique Variable
    bool newParty = true;
    bool startGame = false;
    bool diagonalsDisplayed;
    bool coordonneEstimeTrue = false;
    bool isBallSelectedWithKey = false;
    bool isMachineVsMachine;
    char username[25] = "";
    int matrix[22][22];
    Player T[100];
    int tabSize;

    Uint32 startTime = SDL_GetTicks(), startPause, endPause, tempsPause;
    bool playGame;
    bool gameWin;
    bool isNewPlayer;
    bool continueGame;
    Player player, copyPlayer;
    int N;
    //---------------------------------------------------------------
    int diagonalTouched, diagonalNumber;
    Ball coordonneFinEstime, ball, copyBall;
    Ball ballAnimation, keyboardBall;

    // init SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    // init img
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        printf("IMG_Init failed: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // window creation
    win = SDL_CreateWindow("SDL_Projet ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL)
    {
        printf("Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // renderer
    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // ttf
    if (TTF_Init() < 0)
    {
        printf("TTF_Init() failed: %s\n", TTF_GetError());
        cleanup(win, renderer, backgroundTexture, backgroundSurface);
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("font/Hoecake.ttf", 24);
    if (font == NULL)
    {
        printf("TTF_OpenFont() failed: %s\n", TTF_GetError());
        cleanup(win, renderer, backgroundTexture, backgroundSurface);
        return 1;
    }

    TTF_Font *fonthead = TTF_OpenFont("font/Hoecake.ttf", 32);
    if (font == NULL)
    {
        printf("TTF_OpenFont() failed: %s\n", TTF_GetError());
        cleanup(win, renderer, backgroundTexture, backgroundSurface);
        return 1;
    }

    TTF_Font *helpFont = TTF_OpenFont("font/Hoecake.ttf", 28);

    // init mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL_mixer : %s\n", Mix_GetError());
        SDL_DestroyRenderer(renderer);
        // SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //-----effect--------
    bonk = Mix_LoadWAV("effect/bonk.wav");
    click = Mix_LoadWAV("effect/click.wav");
    game_over = Mix_LoadWAV("effect/game-over.wav");
    level_win = Mix_LoadWAV("effect/level-win.wav");
    success = Mix_LoadWAV("effect/success.wav");

    Mix_Music *music = Mix_LoadMUS("effect/music.ogg");
    Mix_Music *speech = Mix_LoadMUS("effect/Speech.ogg");


    bool quit = false;
    SDL_Event exit;

    backgroundSurface = IMG_Load("photo/background.png");
    if (backgroundSurface == NULL)
    {
        printf("Failed to load background image: %s\n", IMG_GetError());
        cleanup(win, renderer, backgroundTexture, backgroundSurface);
        return 1;
    }

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    if (backgroundTexture == NULL)
    {
        printf("Failed to create background texture: %s\n", SDL_GetError());
        cleanup(win, renderer, backgroundTexture, backgroundSurface);
        return 1;
    }

    SDL_Rect usernameRect = {(1200 - 290) / 2, 220, 290, 30};
    SDL_Rect confirmButtonRect = {(1200 - 90) / 2, 265, 90, 30};

    TTF_Font *StartFont = TTF_OpenFont("font/StartFont.ttf", 40);

    TTF_Font *confirmFont = TTF_OpenFont("font/Hoecake.ttf", 18);
    TTF_Font *startFont2 = TTF_OpenFont("font/Hoecake.ttf", 24);
    if (confirmFont == NULL)
    {
        printf("Font could not be loaded! TTF_Error: %s\n", TTF_GetError());
        return 1;
    }

    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MAX_VOLUME);
    // Animation avant lancement du jeu
    initializeAnimation();
    bool quittest = false;
    bool isStartHovered = false;
    while (!quittest)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                cleanup(win,renderer,backgroundTexture,backgroundSurface);
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;
                if (isInsideRect(mouseX, mouseY, (SDL_Rect){320, 225, 495, 65}))
                {
                    isStartHovered = true;
                }
                else
                {
                    isStartHovered = false;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                if (isInsideRect(mouseX, mouseY, (SDL_Rect){320, 225, 495, 65}))
                {
                    quittest = true;
                }
            }else if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_p){
                    MAX_VOLUME = MAX_VOLUME + 10;
                    Mix_VolumeMusic(MAX_VOLUME);
                }else if(event.key.keysym.sym == SDLK_m){
                    MAX_VOLUME = MAX_VOLUME - 10;
                    Mix_VolumeMusic(MAX_VOLUME);
                }
            }
        }
        animationStartGame(&quittest, StartFont,startFont2, isStartHovered);
        SDL_Delay(FRAME_DELAY);
    }
    Uint32 startTransisition = SDL_GetTicks();
    int transitionTime = 10;
    //DelayTa3na(transitionTime);

    SDL_RenderClear(renderer);

    bool isClicked = false;
    int index = 0;
    int index2 = 0;

    initRender();

    // MAIN LOOP
    while (!quit)
    {
        //initRender();

        while (SDL_PollEvent(&exit))
        {
            if (exit.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (exit.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX = exit.button.x;
                int mouseY = exit.button.y;
                if (!helpWindow && !highscoreWin && !modewindow && !continwin && !matrixwin && !pausewin && !oldornewwin && !machineWin && !machinePlayerwin)
                { // Home page

                    if (isInsideRect(mouseX, mouseY, (SDL_Rect){410, 325, 315, 50}))
                    {
                        Mix_PlayChannel(-1, click, 0);
                        modewindow = true; // Display the mode page
                    }
                    else if (isInsideRect(mouseX, mouseY, (SDL_Rect){410, 410, 315, 50}))
                    {
                        Mix_PlayChannel(-1, click, 0);
                        helpWindow = true; // Display the help page
                        highscoreWin = false;
                    }
                    else if (isInsideRect(mouseX, mouseY, (SDL_Rect){410, 490, 315, 50}))
                    {
                        Mix_PlayChannel(-1, click, 0);
                        highscoreWin = true;
                        helpWindow = false;
                        remplirTableauDepuisFichierBinaire(T, &tabSize);
                        trierJoueursParScore(T, tabSize);
                    }
                    newParty = true;
                }
                else if (modewindow)
                { // Mode page
                    if (isInsideRect(mouseX, mouseY, (SDL_Rect){600, 350, 200, 50}))
                    {
                        Mix_PlayChannel(-1, click, 0);

                        helpWindow = false;
                        modewindow = false;
                        machineWin = false;
                        machinePlayerwin = false;
                        oldornewwin = true;
                        continwin = false;
                        matrixwin = false;
                        pausewin = false; // Display the oldornew page
                        userinputwin = false;
                        highscoreWin = false;
                    }
                    else if (isInsideRect(mouseX, mouseY, (SDL_Rect){350, 350, 200, 50}))
                    {
                        Mix_PlayChannel(-1, click, 0);
                        helpWindow = false;
                        modewindow = false;
                        machineWin = false;
                        machinePlayerwin = true;
                        oldornewwin = false;
                        continwin = false;
                        matrixwin = false;
                        pausewin = false;
                        userinputwin = false;
                        highscoreWin = false;
                    }
                }
                else if (machinePlayerwin)
                {
                    if (isInsideRect(mouseX, mouseY, (SDL_Rect){600, 350, 200, 50}))
                    {
                        Mix_PlayChannel(-1, click, 0);
                        helpWindow = false;
                        modewindow = false;
                        machineWin = true;
                        machinePlayerwin = false;
                        isMachineVsMachine = false;
                        oldornewwin = false;
                        continwin = false;
                        matrixwin = false;
                        pausewin = false; // Display the oldornew page
                        userinputwin = false;
                        highscoreWin = false;
                    }
                    else if (isInsideRect(mouseX, mouseY, (SDL_Rect){350, 350, 200, 50}))
                    {
                        Mix_PlayChannel(-1, click, 0);
                        helpWindow = false;
                        modewindow = false;
                        machineWin = true;
                        machinePlayerwin = false;
                        isMachineVsMachine = true;
                        oldornewwin = false;
                        continwin = false;
                        matrixwin = false;
                        pausewin = false;
                        userinputwin = false;
                        highscoreWin = false;

                        playMachine = true;
                    }
                }
                else if (highscoreWin)
                { // HighScore page
                    if (isInsideRect(mouseX, mouseY, (SDL_Rect){525, 520, 120, 50}))
                    {
                        Mix_PlayChannel(-1, click, 0);
                        helpWindow = false;
                        modewindow = false;
                        machineWin = false;
                        machinePlayerwin = false;
                        oldornewwin = false;
                        continwin = false;
                        matrixwin = false;
                        pausewin = false;
                        userinputwin = false;
                        highscoreWin = false;
                        // Display the home page
                    }
                }
                else if (oldornewwin)
                { // oldornew page
                    if (isInsideRect(mouseX, mouseY, (SDL_Rect){350, 350, 200, 50}))
                    { // newplayer
                        // display continue page
                        Mix_PlayChannel(-1, click, 0);
                        helpWindow = false;
                        modewindow = false;
                        machineWin = false;
                        machinePlayerwin = false;
                        oldornewwin = false;
                        continwin = false;
                        matrixwin = false;
                        pausewin = false;
                        userinputwin = true;
                        highscoreWin = false;
                        // + variable logique
                        isNewPlayer = true;
                    }
                    else if (isInsideRect(mouseX, mouseY, (SDL_Rect){625, 350, 200, 50}))
                    { // old player
                        Mix_PlayChannel(-1, click, 0);
                        helpWindow = false;
                        modewindow = false;
                        machineWin = false;
                        machinePlayerwin = false;
                        oldornewwin = false;
                        continwin = false;
                        matrixwin = false;
                        pausewin = false;
                        userinputwin = true;
                        highscoreWin = false;
                        // + variable logique
                        isNewPlayer = false;
                    }
                }
                else if (continwin)
                { // continue page
                    if (isInsideRect(mouseX, mouseY, (SDL_Rect){600, 350, 200, 50}))
                    { // continue
                        Mix_PlayChannel(-1, click, 0);
                        helpWindow = false;
                        modewindow = false;
                        machineWin = false;
                        machinePlayerwin = false;
                        oldornewwin = false;
                        continwin = false;
                        matrixwin = true;
                        pausewin = false;
                        userinputwin = false;
                        highscoreWin = false;
                        // variable de la logique
                        continueGame = true;
                    }
                    else if (isInsideRect(mouseX, mouseY, (SDL_Rect){350, 350, 200, 50}))
                    { // new game
                        Mix_PlayChannel(-1, click, 0);
                        helpWindow = false;
                        modewindow = false;
                        machineWin = false;
                        machinePlayerwin = false;
                        oldornewwin = false;
                        continwin = false;
                        matrixwin = true;
                        pausewin = false;
                        userinputwin = false;
                        highscoreWin = false;
                        // variable de la logique
                        continueGame = false;
                    }
                }
                else if (matrixwin)
                {
                    if (isInsideRect(mouseX, mouseY, (SDL_Rect){200, 25, 200, 50}))
                    {
                        Mix_PlayChannel(-1, click, 0);
                        helpWindow = false;
                        modewindow = false;
                        machineWin = false;
                        machinePlayerwin = false;
                        oldornewwin = false;
                        continwin = false;
                        matrixwin = false;
                        pausewin = true;
                        highscoreWin = false;
                        userinputwin = false;
                    }
                    else if (isInsideRect(mouseX, mouseY, (SDL_Rect){1000, 25, 200, 50}))
                    {
                        Mix_PlayChannel(-1, click, 0);
                        helpWindow = false;
                        modewindow = false;
                        machineWin = false;
                        machinePlayerwin = false;
                        oldornewwin = false;
                        continwin = false;
                        matrixwin = false;
                        pausewin = false;
                        highscoreWin = false;
                        userinputwin = false;
                    }
                }
                else if (pausewin)
                {
                    if (isInsideRect(mouseX, mouseY, (SDL_Rect){350, 350, 200, 50}))
                    {
                        Mix_PlayChannel(-1, click, 0);
                        helpWindow = false;
                        modewindow = false;
                        machineWin = false;
                        machinePlayerwin = false;
                        oldornewwin = false;
                        continwin = false;
                        matrixwin = true;
                        pausewin = false;
                        highscoreWin = false;
                        userinputwin = false;
                        newParty = false;
                        endPause = SDL_GetTicks();
                    }
                    else if (isInsideRect(mouseX, mouseY, (SDL_Rect){550, 350, 200, 50}))
                    {
                        Mix_PlayChannel(-1, click, 0);
                        helpWindow = false;
                        modewindow = false;
                        machineWin = false;
                        machinePlayerwin = false;
                        oldornewwin = false;
                        continwin = false;
                        matrixwin = false;
                        pausewin = false;
                        highscoreWin = false;
                        userinputwin = false;
                        newParty = true;
                    }
                }
            }
            else if (exit.type == SDL_KEYDOWN)
            {
                if (exit.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    helpWindow = false;
                    modewindow = false;
                    machineWin = false;
                    machinePlayerwin = false;
                    oldornewwin = false;
                    continwin = false;
                    matrixwin = false;
                    pausewin = false;
                    highscoreWin = false;
                    userinputwin = false;
                    newParty = true;
                }
                else if(exit.key.keysym.sym == SDLK_p){
                    if(MAX_VOLUME < 100){
                        MAX_VOLUME = MAX_VOLUME + 10;
                    }
                    Mix_VolumeMusic(MAX_VOLUME);
                }else if(exit.key.keysym.sym == SDLK_m){
                    if(MAX_VOLUME > 0){
                        MAX_VOLUME = MAX_VOLUME - 10;
                    }
                    Mix_VolumeMusic(MAX_VOLUME);
                }
            }
            else if (exit.type == SDL_MOUSEMOTION)
            {
                int mouseX = exit.motion.x;
                int mouseY = exit.motion.y;

                if (!helpWindow && !highscoreWin && !modewindow && !continwin && !matrixwin && !pausewin && !oldornewwin && !machineWin && !machinePlayerwin)
                { // Home page

                    if (isInsideRect(mouseX, mouseY, (SDL_Rect){410, 325, 315, 50}))
                    {
                        StartHovered = true;
                        StoryHovered = false;
                        HighScoreHovered = false;
                    }
                    else if (isInsideRect(mouseX, mouseY, (SDL_Rect){410, 410, 315, 50}))
                    {
                        StartHovered = false;
                        StoryHovered = true;
                        HighScoreHovered = false;
                    }
                    else if (isInsideRect(mouseX, mouseY, (SDL_Rect){410, 490, 315, 50}))
                    {
                        StartHovered = false;
                        StoryHovered = false;
                        HighScoreHovered = true;
                    }
                    else
                    {
                        StartHovered = false;
                        StoryHovered = false;
                        HighScoreHovered = false;
                    }
                }
                else if (modewindow)
                {
                    if (isInsideRect(mouseX, mouseY, (SDL_Rect){600, 350, 200, 50}))
                    {

                        playerhovered = true;
                        machinehovered = false;
                    }
                    else if (isInsideRect(mouseX, mouseY, (SDL_Rect){350, 350, 200, 50}))
                    {

                        machinehovered = true;
                        playerhovered = false;
                    }else{
                        machinehovered = false;
                        playerhovered = false;
                    }
                }
                else if (machinePlayerwin)
                {
                    if (isInsideRect(mouseX, mouseY, (SDL_Rect){600, 350, 200, 50}))
                    {

                        humainhovered = true;
                        machinemachinehovered = false;
                    }
                    else if (isInsideRect(mouseX, mouseY, (SDL_Rect){350, 350, 200, 50}))
                    {

                        machinemachinehovered = true;
                        humainhovered = false;
                    }else{
                        humainhovered = false;
                        machinemachinehovered = false;
                    }
                }
                else if (highscoreWin)
                {
                    if (isInsideRect(mouseX, mouseY, (SDL_Rect){525, 520, 120, 50}))
                    {

                        EchapHovered = true;
                    }
                    else
                    {
                        EchapHovered = false;
                    }
                }
                else if (oldornewwin)
                { // oldornew page
                    if (isInsideRect(mouseX, mouseY, (SDL_Rect){350, 350, 200, 50}))
                    { // newplayer

                        newhovered = true;
                        oldhovered = false;
                    }
                    else if (isInsideRect(mouseX, mouseY, (SDL_Rect){600, 350, 200, 50}))
                    { // old player

                        oldhovered = true;
                        newhovered = false;
                    }else{
                        oldhovered = false;
                        newhovered = false;
                    }
                }
                else if (continwin)
                { // continue page
                    if (isInsideRect(mouseX, mouseY, (SDL_Rect){600, 350, 200, 50}))
                    { // continue

                        newgamehovered = false;
                        continuehovered = true;
                    }
                    else if (isInsideRect(mouseX, mouseY, (SDL_Rect){350, 350, 200, 50}))
                    { // new game

                        newgamehovered = true;
                        continuehovered = false;
                    }else{
                         newgamehovered = false;
                        continuehovered = false;
                    }
                }
                else if (pausewin)
                {
                    if (isInsideRect(mouseX, mouseY, (SDL_Rect){350, 350, 200, 50}))
                    {

                        continhovered = true;
                        homehovered = false;
                    }
                    else if (isInsideRect(mouseX, mouseY, (SDL_Rect){550, 350, 200, 50}))
                    {
                        continhovered = false;
                        homehovered = true;
                    }else{
                        continhovered = false;
                        homehovered = false;
                    }
                }
            }
        }
                SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

                if (!helpWindow && !modewindow && !continwin && !matrixwin && !pausewin && !oldornewwin && !userinputwin && !highscoreWin && !machineWin && !machinePlayerwin)
                {
                    Mix_VolumeMusic(MAX_VOLUME);
                    homeWindow = false;

                    if (index < 20)
                    {
                        DrawHomeOn(renderer, font, fonthead, StartHovered, StoryHovered, HighScoreHovered);
                    }
                    if (index >= 20 && index < 25)
                    {
                        DrawHomeOff(renderer, font, fonthead, StartHovered, StoryHovered, HighScoreHovered);
                    }
                    if (index >= 25 && index < 30)
                    {
                        DrawHomeOn(renderer, font, fonthead, StartHovered, StoryHovered, HighScoreHovered);
                    }
                    if (index >= 30 && index <= 40)
                    {
                        DrawHomeOff(renderer, font, fonthead, StartHovered, StoryHovered, HighScoreHovered);
                    }
                    if (index > 40 && index <= 50)
                    {
                        DrawHomeOn(renderer, font, fonthead, StartHovered, StoryHovered, HighScoreHovered);
                    }
                    if (index >= 50 && index <= 60)
                    {
                        DrawHomeOff(renderer, font, fonthead, StartHovered, StoryHovered, HighScoreHovered);
                    }
                    if (index >= 60 && index <= 80)
                    {
                        DrawHomeOn(renderer, font, fonthead, StartHovered, StoryHovered, HighScoreHovered);
                    }
                    index++;
                    if (index > 80)
                    {
                        index = 0;
                    }
                }
                else if (helpWindow)
                {
                    homeWindow = false;

                    helpWindowOne = true;
                    SDL_Event exitHelp;
                    int temp, i = 0;

                    Uint32 startSpeechTime = SDL_GetTicks();
                    int SpeechTime1 = 19500; // ms
                    int SpeechTime2 = SpeechTime1 + 21000;
                    int SpeechTime3 = SpeechTime2 + 2000;
                    DelayTa3na(500);
                    Mix_HaltMusic();
                    Mix_PlayMusic(speech, 1);

                    while (helpWindow)
                    {

                        while (SDL_PollEvent(&exitHelp))
                        {
                            if (exitHelp.type == SDL_QUIT)
                            {
                                cleanup(win,renderer,backgroundTexture,backgroundSurface);
                            }else if(exitHelp.type == SDL_KEYDOWN){
                                if(exitHelp.key.keysym.sym == SDLK_p){
                                    if(MAX_VOLUME < 100){
                                        MAX_VOLUME = MAX_VOLUME + 10;
                                    }
                                    Mix_VolumeMusic(MAX_VOLUME);
                                }else if(exitHelp.key.keysym.sym == SDLK_m){
                                    if(MAX_VOLUME > 0){
                                        MAX_VOLUME = MAX_VOLUME - 10;
                                    }
                                    Mix_VolumeMusic(MAX_VOLUME);
                                }
                            }else if(exitHelp.type == SDL_MOUSEBUTTONDOWN){
                                int mousex,mousey;
                                if(isInsideRect(mousex, mousex, (SDL_Rect){0, 0, 1200, 1200})){
                                    if(helpWindowThree &&  SDL_GetTicks() - startSpeechTime > SpeechTime3){
                                        helpWindowOne = false;
                                        helpWindowTwo = false;
                                        helpWindowThree = false;
                                        helpWindow = false;
                                        Mix_PlayMusic(music, -1);
                                    }
                                }
                            }
                        }


                        if (helpWindow)
                        {
                            if (helpWindowOne && SDL_GetTicks() - startSpeechTime > SpeechTime1)
                            {
                                helpWindowOne = false;
                                helpWindowTwo = true;
                                helpWindowThree = false;
                            }
                            else if (helpWindowTwo && SDL_GetTicks() - startSpeechTime > SpeechTime2)
                            {
                                helpWindowOne = false;
                                helpWindowTwo = false;
                                helpWindowThree = true;
                            }
                            /*else if (helpWindowThree && SDL_GetTicks() - startSpeechTime > SpeechTime3)
                            {
                                helpWindowOne = false;
                                helpWindowTwo = false;
                                helpWindowThree = false;
                                helpWindow = false;
                                Mix_PlayMusic(music, -1);
                            }*/
                        }

                        if (helpWindowOne)
                        {
                            temp++;

                            if (temp < 6)
                            {
                                DrawHelpTwoofOne(renderer, helpFont);
                                SDL_RenderPresent(renderer);
                            }
                            if (temp >= 6 && temp < 8)
                            {
                                DrawHelpThreeofOne(renderer, helpFont);
                                SDL_RenderPresent(renderer);
                            }
                            if (temp >= 8 && temp < 10)
                            {
                                DrawHelpTwoofOne(renderer, helpFont);
                                SDL_RenderPresent(renderer);
                            }
                            if (temp >= 10 && temp < 13)
                            {
                                DrawHelpThreeofOne(renderer, helpFont);
                                SDL_RenderPresent(renderer);
                            }

                            if (temp > 13)
                            {
                                temp = 0;
                            }
                            //-------------------------------------Synchroni..............
                        }
                        if (helpWindowThree)
                        {
                            temp++;

                            if (temp < 3)
                            {
                                DrawHelpOneofThree(renderer, font);
                                SDL_RenderPresent(renderer);
                            }
                            if (temp >= 3 && temp < 4)
                            {
                                DrawHelpTwoofThree(renderer, font);
                                SDL_RenderPresent(renderer);
                            }
                            if (temp >= 4 && temp < 5)
                            {
                                DrawHelpOneofThree(renderer, font);
                                SDL_RenderPresent(renderer);
                            }
                            if (temp >= 5 && temp < 7)
                            {
                                DrawHelpTwoofThree(renderer, font);
                                SDL_RenderPresent(renderer);
                            }

                            if (temp > 7)
                            {
                                temp = 0;
                            }
                        }
                        if (helpWindowTwo)
                        {
                            DrawHelpOneofTwo(renderer, helpFont);
                            SDL_RenderPresent(renderer);
                        }
                    }
                }
                else if (highscoreWin)
                {

                    if (index2 < 17)
                    {
                        DrawHighScoreOn(renderer, font, T, tabSize, EchapHovered);
                    }
                    if (index2 >= 17 && index2 < 34)
                    {
                        DrawHighScoreOff(renderer, font, T, tabSize, EchapHovered);
                    }
                    index2++;
                    if (index2 >= 34)
                    {
                        index2 = 0;
                    }
                }
                else if (modewindow)
                {
                    homeWindow = false;
                    drawmodewin(renderer, font, fonthead, machinehovered, playerhovered);
                }
                else if (machinePlayerwin)
                {
                    homeWindow = false;
                    drawMachineTypeWin(renderer, font, fonthead, machinemachinehovered, humainhovered);
                }
                else if (oldornewwin)
                {
                    homeWindow = false;
                    oldornewWin(renderer, font, fonthead, oldhovered, newhovered);
                }
                else if (userinputwin)
                {
                    bool isEscape = false;
                    homeWindow = false;
                    usernamesCount = 0;
                    handleUsernameInput(renderer, font, confirmFont, usernameRect, confirmButtonRect, backgroundTexture, username, &quit, &isEscape);

                    if (quit)
                    {
                        printf("Fin De Jeu ...");
                    }
                    if (isEscape)
                    {
                        helpWindow = false;
                        modewindow = false;
                        machineWin = false;
                        machinePlayerwin = false;
                        oldornewwin = false;
                        continwin = false;
                        matrixwin = false;
                        pausewin = false;
                        highscoreWin = false;
                        userinputwin = false;
                        newParty = true;
                    }

                    if (!quit && !isEscape)
                    {
                        printf("Noms d'utilisateur saisis : %s\n", username);
                        if (isNewPlayer)
                        {
                            if (!joueurExiste(username, &player))
                            {
                                strcpy(player.username, username);
                                initPlayer(&player);
                                enregistrerJoueur(&player);
                                helpWindow = false;
                                modewindow = false;
                                machineWin = false;
                                machinePlayerwin = false;
                                oldornewwin = false;
                                continwin = false;
                                matrixwin = true;
                                pausewin = false;
                                userinputwin = false;
                            }
                            else
                            {
                                printf("donner un nom valide \n");
                            }
                        }
                        else
                        {
                            if (joueurExiste(username, &player))
                            {
                                chargerJoueur(username, &player);
                                continwin = true;
                                userinputwin = false;
                            }
                            else
                            {
                                printf("donner un nom valide \n");
                            }
                        }
                    }
                }
                else if (continwin)
                {
                    homeWindow = false;
                    drawcontinewin(renderer, font, fonthead, continuehovered, newgamehovered);
                }
                else if (pausewin)
                {
                    homeWindow = false;
                    drawpause(renderer, font, fonthead, continhovered, homehovered);
                }
                else if (machineWin)
                {
                    homeWindow = false;
                    if (isMachineVsMachine)
                    {
                        N = 6;
                        int winCount = 0;

                        while (playMachine)
                        {

                            int grid[N][N];
                            int gridcopy[N][N];

                            initMatrix(N, grid);
                            initMatrix(N, gridcopy);
                            animationAvantDebut(N, grid, copyBall);
                            generationDiagonal(N, grid, 5);
                            ball = generationPointDepart(N);
                            renderNexusMatrix(N, grid);
                            renderDiagonals(N, grid);
                            SDL_RenderPresent(renderer);
                            DelayTa3na(3000);

                            createMatrixWithPixel(N,renderer, gridcopy);

                            affichageGrid(N, gridcopy);

                            copyBall = ball;
                            calculeTrajectoire(N, gridcopy, &copyBall, &diagonalTouched);
                            SDL_RenderClear(renderer);

                            renderNexusMatrix(N, gridcopy);
                            drawBall(ball, N);
                            SDL_RenderPresent(renderer);

                            DelayTa3na(1000);

                            drawGreenBall(copyBall, N);
                            SDL_RenderPresent(renderer);

                            animationTrajectoireV2(N, gridcopy, ball);
                            SDL_RenderPresent(renderer);
                            SDL_RenderClear(renderer);

                            bool quitMachine = false;

                            SDL_RenderPresent(renderer);
                            drawContineMachinewin(renderer, font, fonthead, continhovered, homehovered);
                            SDL_RenderPresent(renderer);

                            while (!quitMachine)
                            {
                                while (SDL_PollEvent(&exit))
                                {
                                    if (exit.type == SDL_QUIT)
                                    {
                                        quitMachine = true;
                                        playMachine = false;
                                        quit = true;
                                    }
                                    else if (exit.type == SDL_MOUSEBUTTONDOWN)
                                    {
                                        int mouseX = exit.button.x;
                                        int mouseY = exit.button.y;

                                        if (isInsideRect(mouseX, mouseY, (SDL_Rect){625, 350, 200, 50}))
                                        {
                                            quitMachine = true;
                                            playMachine = true;
                                            winCount++;
                                        }
                                        else if (isInsideRect(mouseX, mouseY, (SDL_Rect){350, 350, 200, 50}))
                                        {
                                            quitMachine = true;

                                            playMachine = false;
                                            machineWin = false;
                                            machinePlayerwin = false;
                                        }
                                    }else if(exit.type == SDL_KEYDOWN){
                                        if(exit.key.keysym.sym == SDLK_p){
                                            if(MAX_VOLUME < 100){
                                                MAX_VOLUME = MAX_VOLUME + 10;
                                            }
                                            Mix_VolumeMusic(MAX_VOLUME);
                                        }else if(exit.key.keysym.sym == SDLK_m){
                                            if(MAX_VOLUME > 0){
                                                MAX_VOLUME = MAX_VOLUME - 10;
                                            }
                                            Mix_VolumeMusic(MAX_VOLUME);
                                        }
                                    }
                                }
                            }
                            if (winCount >= 4 && N < 20)
                            {
                                N += 1;
                                winCount = 0;
                            }
                            //initRender();
                        }
                    }else{
                        N = 6;
                        int winCount = 0;
                        bool startMachine = false;
                        bool machinePlay = true;
                        bool play = true;
                        int score = 0;
                        int level = 1;


                        while (machinePlay && !homeWindow){
                            //initRender();
                            int grid[N][N];
                            int gridcopy[N][N];
                            int DiagoTouched = 0;
                            Ball cellule,copyCell;
                            cellule.x = cellule.y = 1;
                            initMatrix(N, gridcopy);
                            initMatrix(N, grid);
                            renderAlexMatrix(N, grid,font,score,level);
                            renderDiagonals(N, grid);
                            SDL_RenderPresent(renderer);

                            bool quittemp = false;
                            while (!quittemp)
                            {
                                //initRender();
                                while (SDL_PollEvent(&exit))
                                {
                                    if (exit.type == SDL_QUIT)
                                    {
                                        quit = true;
                                        quittemp = true;
                                        machinePlay = false;
                                    }else if(exit.type == SDL_MOUSEMOTION){
                                        int mouseX = exit.motion.x;
                                        int mouseY = exit.motion.y;
                                        copyCell = handleButtonClick2(mouseX, mouseY, N);
                                        if(copyCell.x >= 0 && copyCell.x <= N - 1 && copyCell.y >= 0 && copyCell.y <= N - 1){
                                            cellule = copyCell;
                                        }
                                    }
                                    else if (exit.type == SDL_MOUSEBUTTONDOWN)
                                    {
                                        int mouseX = exit.button.x;
                                        int mouseY = exit.button.y;

                                        ball = handleButtonClick2(mouseX, mouseY, N);

                                        if(isInsideRect(mouseX, mouseY, (SDL_Rect){1000, 25, 200, 50})){
                                                Mix_PlayChannel(-1, click, 0);
                                            helpWindow = false;
                                            modewindow = false;
                                            machineWin = false;
                                            machinePlayerwin = false;
                                            oldornewwin = false;
                                            continwin =  false;
                                            matrixwin = false;
                                            pausewin = false;
                                            highscoreWin = false;
                                            userinputwin = false;
                                            machinePlay = false;
                                            quittemp = true;

                                        }else if (ball.x > 0 && ball.x < N - 1 && ball.y > 0 && ball.y < N - 1){

                                            switch (grid[ball.y][ball.x])
                                            {
                                            case 0:
                                                grid[ball.y][ball.x] = 1;
                                                break;
                                            case 1:
                                                grid[ball.y][ball.x] = 2;
                                                break;
                                            case 2:
                                                grid[ball.y][ball.x] = 0;
                                                break;
                                            }
                                            startMachine = false;
                                        }
                                        else if (((ball.x == 0 || ball.x == N - 1) && ball.y > 0 && ball.y < N - 1) || (ball.y == 0 || ball.y == N - 1) && ball.x > 0 && ball.x < N - 1)
                                        {
                                            Mix_PlayChannel(-1, click, 0);
                                            startMachine = true;
                                        }
                                    }else if(exit.type == SDL_KEYDOWN){
                                        if (exit.key.keysym.sym == SDLK_DOWN) {
                                            cellule.y ++ ;
                                            if(cellule.y>N-1){
                                                cellule.y = 0;
                                            }
                                        }else if(exit.key.keysym.sym == SDLK_UP){
                                            cellule.y --;
                                            if(cellule.y<0){
                                                cellule.y = N-1;
                                            }
                                        }else if(exit.key.keysym.sym == SDLK_RIGHT){
                                            cellule.x ++;
                                            if(cellule.x>N-1){
                                                cellule.x = 0;
                                            }
                                        }else if(exit.key.keysym.sym == SDLK_LEFT){
                                            cellule.x --;
                                            if(cellule.x<0){
                                                cellule.x = N-1;
                                            }
                                        }

                                        if(exit.key.keysym.sym == SDLK_RETURN){
                                            if(((cellule.x == 0 || cellule.x == N - 1) && cellule.y > 0 && cellule.y < N - 1) || (cellule.y == 0 || cellule.y == N - 1) && cellule.x > 0 && cellule.x < N - 1){
                                                startMachine = true;
                                                ball = cellule;
                                                drawGreenBall(ball,N);
                                            }else if(cellule.x > 0 && cellule.x < N - 1 && cellule.y > 0 && cellule.y < N - 1){
                                                switch (grid[cellule.y][cellule.x])
                                                {
                                                case 0:
                                                    grid[cellule.y][cellule.x] = 1;
                                                    break;
                                                case 1:
                                                    grid[cellule.y][cellule.x] = 2;
                                                    break;
                                                case 2:
                                                    grid[cellule.y][cellule.x] = 0;
                                                    break;
                                                }
                                                startMachine = false;

                                            }


                                        }
                                        if(exit.key.keysym.sym == SDLK_p){
                                            if(MAX_VOLUME < 100){
                                                MAX_VOLUME = MAX_VOLUME + 10;
                                            }
                                            Mix_VolumeMusic(MAX_VOLUME);
                                        }else if(exit.key.keysym.sym == SDLK_m){
                                            if(MAX_VOLUME > 0){
                                                MAX_VOLUME = MAX_VOLUME - 10;
                                            }
                                            Mix_VolumeMusic(MAX_VOLUME);
                                        }
                                    }
                                    SDL_RenderClear(renderer);
                                    renderAlexMatrix(N, gridcopy,font,score,level);
                                    renderDiagonals(N, grid);
                                    drawCellule(cellule,N);
                                    SDL_RenderPresent(renderer);
                                }
                                if (startMachine)
                                {
                                    initMatrix(N,gridcopy);
                                    createMatrixWithPixel(N,renderer,gridcopy);

                                    SDL_RenderClear(renderer);
                                    renderAlexMatrix(N, grid,font,score,level);

                                    DelayTa3na(3000);
                                    SDL_RenderClear(renderer);
                                    renderAlexMatrix(N, gridcopy,font,score,level);
                                    Ball copyBall = ball;
                                    copyBall.direction = Direction_depart(copyBall, N);
                                    drawGreenBall(copyBall,N);

                                    Ball copyBall2 = copyBall;
                                    calculeTrajectoire(N,grid,&copyBall2,&DiagoTouched);
                                    score = score + calculeScorePartie(N, DiagoTouched, calculeDiagonal(N,grid), true);
                                    animationAlexTrajectoire(N, gridcopy, copyBall,&homeWindow,font,score,level);

                                    if(homeWindow){
                                        machinePlay = false;
                                        quittemp = true;
                                    }else{
                                        winCount ++;
                                        if(winCount >= 4 && N < 20){
                                            N += 1;
                                            level ++;
                                            winCount = 0;
                                        }
                                        quittemp = true;
                                        renderMatrix(N, grid);
                                    }

                                    startMachine = false;

                                    SDL_RenderClear(renderer);
                                    initMatrix(N,grid);
                                }
                            }

                        }
                    }
                }
                else if (matrixwin)
                {
                    homeWindow = false;
                    if (!continueGame)
                    {
                        if (isNewPlayer)
                        {
                            continueGame = true;
                            isNewPlayer = false;
                        }
                        initPlayer(&player);
                        mettreAJourJoueur(player.username, &player);
                    }

                    N = player.Niveau + 5;

                    int grid[N][N];
                    MatrixToGrid(N, grid, matrix);

                    diagonalTouched = 0;
                    if (newParty)
                    {
                        diagonalsDisplayed = true;
                        // animation des ball (circle)
                        animationAvantDebut(N, grid, copyBall); //--------------------------------------------------------------------------------------------
                        SDL_Delay(500);
                        startTime = SDL_GetTicks();
                        tempsPause = 0;
                        generationDiagonal(N, grid, 1);
                        ball = generationPointDepart(N);
                        copyBall = ball;
                        ballAnimation = ball;
                        ball.direction = Direction_depart(ball, N);
                        calculeTrajectoire(N, grid, &ball, &diagonalTouched);
                        // copie de la matrice
                        GridToMatrix(N, grid, matrix);
                        keyboardBall.x = 0;
                        keyboardBall.y = 1;
                    }
                    else
                    {
                        tempsPause = tempsPause + endPause - startPause;
                    }

                    if (SDL_GetTicks() - startTime < N * 500 + tempsPause)
                    {
                        renderPlayerMatrix(font, N, grid, player);
                        renderDiagonals(N, grid);
                    }
                    else
                    {
                        SDL_RenderClear(renderer);
                        renderPlayerMatrix(font, N, grid, player);
                        drawBall(copyBall, N);
                    }
                    SDL_RenderPresent(renderer);

                    playGame = true;

                    SDL_Event event2;
                    while (playGame)
                    {
                        if (SDL_GetTicks() - startTime > N * 500 + tempsPause && diagonalsDisplayed)
                        {
                            SDL_RenderClear(renderer);
                            renderPlayerMatrix(font, N, grid, player);
                            drawBall(copyBall, N);
                            SDL_RenderPresent(renderer);
                            diagonalsDisplayed = false;
                            startGame = true;
                        }
                        while (SDL_PollEvent(&event2))
                        {
                            int mouseX2 = event2.button.x;
                            int mouseY2 = event2.button.y;
                            if (event2.type == SDL_QUIT)
                            {
                                // cleanup();
                                return 0;
                            }
                            else if (event2.type == SDL_MOUSEBUTTONDOWN)
                            {

                                if (isInsideRect(mouseX2, mouseY2, (SDL_Rect){25, 25, 150, 150}))
                                {
                                    Mix_PlayChannel(-1, click, 0);
                                    helpWindow = false;
                                    modewindow = false;
                                    machineWin = false;
                                    machinePlayerwin = false;
                                    oldornewwin = false;
                                    continwin = false;
                                    matrixwin = false;
                                    pausewin = true;
                                    userinputwin = false;
                                    playGame = false;
                                    newParty = false;
                                    startPause = SDL_GetTicks();
                                }
                                else if (isInsideRect(mouseX2, mouseY2, (SDL_Rect){1000, 25, 200, 50}))
                                {
                                    Mix_PlayChannel(-1, click, 0);
                                    helpWindow = false;
                                    modewindow = false;
                                    machineWin = false;
                                    machinePlayerwin = false;
                                    oldornewwin = false;
                                    continwin = false;
                                    matrixwin = false;
                                    pausewin = false;
                                    userinputwin = false;
                                    playGame = false;
                                    newParty = true;
                                    startGame = false;
                                }
                                if (startGame)
                                {
                                    Ball coorEst = handleButtonClick2(mouseX2, mouseY2, N);
                                    if (((coorEst.x == 0 || coorEst.x == N - 1) && coorEst.y > 0 && coorEst.y < N - 1) || (coorEst.y == 0 || coorEst.y == N - 1) && coorEst.x > 0 && coorEst.x < N - 1)
                                    {
                                        coordonneEstimeTrue = true;
                                        coordonneFinEstime = coorEst;
                                    }
                                }
                            }
                            else if (event2.type == SDL_KEYDOWN)
                            {
                                if(event2.key.keysym.sym == SDLK_p){
                                    if(MAX_VOLUME < 100){
                                        MAX_VOLUME = MAX_VOLUME + 10;
                                    }
                                    Mix_VolumeMusic(MAX_VOLUME);
                                }else if(event2.key.keysym.sym == SDLK_m){
                                    if(MAX_VOLUME > 0){
                                        MAX_VOLUME = MAX_VOLUME - 10;
                                    }
                                    Mix_VolumeMusic(MAX_VOLUME);
                                }

                                if (event2.key.keysym.scancode == SDL_SCANCODE_SPACE && startGame)
                                {

                                    if (keyboardBall.y == 0 && keyboardBall.x >= 0 && keyboardBall.x < N)
                                    {
                                        keyboardBall.x += 1;
                                    }
                                    if (keyboardBall.x == N - 1 && keyboardBall.y >= 0 && keyboardBall.y < N)
                                    {
                                        keyboardBall.y += 1;
                                    }
                                    if (keyboardBall.y == N - 1 && keyboardBall.x >= 0 && keyboardBall.x < N)
                                    {
                                        keyboardBall.x -= 1;
                                    }
                                    if (keyboardBall.x == 0 && keyboardBall.y >= 0 && keyboardBall.y < N)
                                    {
                                        keyboardBall.y -= 1;
                                    }
                                    if (keyboardBall.x == 0 && keyboardBall.y == 0)
                                    {
                                        keyboardBall.x += 1;
                                    }
                                    isBallSelectedWithKey = true;
                                    SDL_RenderClear(renderer);
                                    renderPlayerMatrix(font, N, grid, player);
                                    drawBall(copyBall, N);
                                    drawGreenBall(keyboardBall, N);
                                    SDL_RenderPresent(renderer);
                                }
                                else if (event2.key.keysym.scancode == SDL_SCANCODE_RETURN && isBallSelectedWithKey)
                                {
                                    coordonneEstimeTrue = true;
                                    coordonneFinEstime.x = keyboardBall.x;
                                    coordonneFinEstime.y = keyboardBall.y;
                                }
                            }
                        }

                        if (coordonneEstimeTrue && startGame && playGame && !pausewin)
                        {
                            // party win
                            if (ball.x == coordonneFinEstime.x && ball.y == coordonneFinEstime.y)
                            {
                                // affichage de la balle en vert
                                drawGreenBall(coordonneFinEstime, N);
                                gameWin = true;
                            }
                            else
                            {
                                // affichage la ball en rouge
                                drawBall(coordonneFinEstime, N);
                                gameWin = false;
                            }

                            diagonalNumber = calculeDiagonal(N, grid);
                            int score = calculeScorePartie(N,diagonalTouched, diagonalNumber, gameWin);
                            etatPartie(&player, gameWin, score);
                            mettreAJourJoueur(player.username, &player);
                            printf("Niveau %d , Partir %d , Score %d , echec %d ,Win %d \n", player.Niveau, player.Partie, player.score, player.echec_consecutive, player.win_consecutive);
                            animationPlayerTrajectoire(N, grid, ballAnimation,&homeWindow,font,player);
                            DelayTa3na(1000);

                            //initRender();

                            coordonneEstimeTrue = false;
                            newParty = true;
                            startGame = false;
                            playGame = false;
                        }

                        if (!helpWindow && !modewindow && !continwin && !matrixwin && !pausewin && !oldornewwin)
                        {
                            DrawHomeOn(renderer, font, fonthead, false, false, false);
                        }
                        else if (pausewin)
                        {
                            drawpause(renderer, font, fonthead, continhovered, homehovered);
                        }
                    }
                }

            if(homeWindow){
                machineWin = helpWindow = modewindow = continwin = matrixwin = pausewin = oldornewwin = false;
            }

            SDL_RenderPresent(renderer);

        }
        TTF_CloseFont(fonthead);
        TTF_CloseFont(font);
        TTF_CloseFont(helpFont);
        cleanup(win, renderer, backgroundTexture, backgroundSurface);
        return 0;
    }
