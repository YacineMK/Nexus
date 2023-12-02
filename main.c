#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define CELL_SIZE 50
#define BALL_RADIUS 10
#define DIAGONAL_LEFT_TO_RIGHT 1
#define DIAGONAL_RIGHT_TO_LEFT 2

typedef struct {
    int x;
    int y;
    int direction;
} Ball;

typedef struct {
    int r;
    int g;
    int b;
    int a;
} Color;

SDL_Window* window ;
SDL_Renderer* renderer ;

void initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow("Ball Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer) {
        printf("Erreur lors de la création de la fenêtre/renderer SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void drawFilledCircle(int centerX, int centerY, int radius, Color c) {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    for (int y = -radius; y <= radius; ++y) {
        int maxX = (int)sqrt(radius * radius - y * y);
        for (int x = -maxX; x <= maxX; ++x) {
            SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        }
    }
}

//Perfecto
void renderMatrix(int N, int grid[N][N],Uint32 startTime) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int offsetX = (SCREEN_WIDTH - N * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - N * CELL_SIZE) / 2;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            SDL_Rect cellRect = {offsetX + j * CELL_SIZE, offsetY + i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_RenderDrawRect(renderer, &cellRect);

            if((i==0 && j==0)||(i==N-1 && j==0)||(i==0 && j==N-1)||(i==N-1 && j==N-1)){

            }else{

                if ((i == 0 || i == N-1) && (j == 0 || j == N-1)) {
                    // Colorier les cases des bords différemment pour les distinguer
                    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                    SDL_RenderFillRect(renderer, &cellRect);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                }

                if (i == 0) {
                    int circleCenterX = offsetX + j * CELL_SIZE + CELL_SIZE / 2;
                    int circleCenterY = offsetY + i * CELL_SIZE + CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 0, 0, 255});
                }
                if (i == N-1) {
                    int circleCenterX = offsetX + j * CELL_SIZE + CELL_SIZE / 2;
                    int circleCenterY = offsetY + (i + 1) * CELL_SIZE - CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){0, 255, 0, 255});
                }
                if (j == 0) {
                    int circleCenterX = offsetX + j * CELL_SIZE + CELL_SIZE / 2;
                    int circleCenterY = offsetY + i * CELL_SIZE + CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){0, 0, 255, 255});
                }
                if (j == N-1) {
                    int circleCenterX = offsetX + (j + 1) * CELL_SIZE - CELL_SIZE / 2;
                    int circleCenterY = offsetY + i * CELL_SIZE + CELL_SIZE / 2;
                    drawFilledCircle(circleCenterX, circleCenterY, CELL_SIZE / 4, (Color){255, 255, 0, 255});
                }
            }
        }
    }

    Uint32 elapsedTime = SDL_GetTicks() - startTime;

        /*---------------------edit this for diago or use timer -------------------- */
      // Display lines for 10 seconds
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        offsetX = (SCREEN_WIDTH - N * CELL_SIZE) / 2;
        offsetY = (SCREEN_HEIGHT - N * CELL_SIZE) / 2;

        for (int i = 1; i < N - 1; ++i) {
            for (int j = 1; j < N - 1; ++j) {
                if (grid[i][j] == 2) {
                    SDL_RenderDrawLine(renderer, offsetX + j * CELL_SIZE, offsetY + i * CELL_SIZE, offsetX + (j + 1) * CELL_SIZE, offsetY + (i + 1) * CELL_SIZE);
                } else if (grid[i][j] == 1) {
                    SDL_RenderDrawLine(renderer, offsetX + j * CELL_SIZE, offsetY + (i + 1) * CELL_SIZE, offsetX + (j + 1) * CELL_SIZE, offsetY + i * CELL_SIZE);
                }
            }
        }


    SDL_RenderPresent(renderer);
}

void drawBall(Ball ball,int N) {
    int offsetX = (SCREEN_WIDTH - N * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - N * CELL_SIZE) / 2;

    int circleCenterX = offsetX + ball.x * CELL_SIZE + CELL_SIZE / 2;
    int circleCenterY = offsetY + ball.y * CELL_SIZE + CELL_SIZE / 2;

    drawFilledCircle(circleCenterX, circleCenterY, BALL_RADIUS, (Color){255, 255, 255, 255});
}

Ball handleButtonClick(int mouseX, int mouseY, int N) {
    Ball cordonneEstime;

    int offsetX = (SCREEN_WIDTH - N * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - N * CELL_SIZE) / 2;

    cordonneEstime.x = (mouseX - offsetX) / CELL_SIZE;
    cordonneEstime.y = (mouseY - offsetY) / CELL_SIZE;

    return cordonneEstime;
}

Ball generationPointDepart(int N) {
    Ball pointDepart;
    int cote;
    do
    {
        srand(time(NULL));
        cote = rand() % 4;
    } while (cote < 0);

    switch (cote) {
        case 0: // Première ligne
            pointDepart.x = 0;
            do{
                srand(time(NULL));
                pointDepart.y = rand() % (N-1);
            }while (pointDepart.y < 0);

            if (pointDepart.y==0){
                pointDepart.y = pointDepart.y + 1 ;
            }
            pointDepart.direction = 1;
            break;
        case 1: // Dernière
            pointDepart.x = N - 1;
            do{
                srand(time(NULL));
                pointDepart.y = rand() % (N-1);
            }while (pointDepart.y < 0);

            if (pointDepart.y==0){
                pointDepart.y = pointDepart.y + 1 ;
            }
            pointDepart.direction = 3;
            break;
        case 2: // Première colonne
            do{
                srand(time(NULL));
                pointDepart.x = rand() % (N-1);
            }while (pointDepart.x < 0);
            pointDepart.y = 0;

            if (pointDepart.x==0){
                pointDepart.x = pointDepart.x + 1 ;
            }
            pointDepart.direction = 2;
            break;
        case 3: // Dernière
            do{
                srand(time(NULL));
                pointDepart.x = rand() % (N-1);
            }while (pointDepart.x < 0);
            pointDepart.y = N - 1;

            if (pointDepart.x==0){
                pointDepart.x = pointDepart.x + 1 ;
            }
            pointDepart.direction = 0;
            break;
    }
    return pointDepart;
}

void animationTrajectoire(int N, int grid[N][N], Ball ball) {
    int orientation;


    do {
        orientation = grid[ball.y][ball.x];
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

        SDL_Delay(75);
        drawBall(ball,N);
        SDL_RenderPresent(renderer);

    } while (ball.x > 0 && ball.x < N-1 && ball.y > 0 && ball.y < N-1);

}

void generationDiagonal(int N, int grid[N][N], int Niveau) {
    int maxDiagonals = (N-2)*(N-2) / 2 ; // Le nombre maximal de diagonales
    int i, j, temp, d = 0;

    // Initialiser le générateur de nombres aléatoires avec une graine
    srand(time(NULL));

    // Remplir la matrice avec des zéros
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            grid[i][j] = 0;
        }
    }

    for (i = 1; i < N - 1; i++) {
        for (j = 1; j < N - 1; j++) {
            temp = rand() % 3;
            if (temp == 1 || temp == 2) {
                d++;
            }
            if (d <= maxDiagonals) {
                grid[i][j] = temp;
            } else {
                grid[i][j] = 0;
            }
        }
    }
}

// Fonction pour calculer la trajectoire de la bille
void calculeTrajectoire(int N,int grid[N][N],Ball* ball,int* Diagonal_touche) {
    int orientation;
    *Diagonal_touche = 0;
    do {
        orientation = grid[ball->y][ball->x];
        if (orientation == DIAGONAL_LEFT_TO_RIGHT) {
            // Diagonale de gauche à droite, ajustez la trajectoire
            if (ball->direction == 0) {
                ball->direction = 1; // Bille va vers la droite
            } else if (ball->direction == 1) {
                ball->direction = 0; // Bille va vers le haut
            } else if (ball->direction == 2) {
                ball->direction = 3; // Bille va vers la gauche
            } else if (ball->direction == 3) {
                ball->direction = 2; // Bille va vers le bas
            }
            (*Diagonal_touche) ++;
        } else if (orientation == DIAGONAL_RIGHT_TO_LEFT) {
            // Diagonale de droite à gauche, ajustez la trajectoire
            if (ball->direction == 0) {
                ball->direction = 3; // Bille va vers la gauche
            } else if (ball->direction == 1) {
                ball->direction = 2; // Bille va vers le bas
            } else if (ball->direction == 2) {
                ball->direction = 1; // Bille va vers la droite
            } else if (ball->direction == 3) {
                ball->direction = 0; // Bille va vers le haut
            }
            (*Diagonal_touche) ++;
        }

        // Avancez la bille dans la direction actuelle
        if (ball->direction == 0) {
            ball->y--;
        } else if (ball->direction == 1) {
            ball->x++;
        } else if (ball->direction == 2) {
            ball->y++;
        } else if (ball->direction == 3) {
            ball->x--;
        }

        // printf("Position Actuell : (%d, %d)\n", ball->x, ball->y);

    }while (ball->x > 0 && ball->x < N-1 && ball->y > 0 && ball->y < N-1);
}

void affichageGrid(int N,int grid[N][N]) {
    int i, j;
    printf("Grille actuelle :\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("+---");
        }
        printf("\n");
        for (j = 0; j < N; j++) {
            printf("| %d ", grid[i][j]);
        }
        printf("|");
        printf("\n");
    }
    for (j = 0; j < N; j++) {
            printf("+---");
        }
    printf("\n");
}

int Direction_depart(Ball ball,int N){
    if(ball.y == 0)
    {
        ball.direction = 2; //direction vers le bas
    }else if (ball.y == N-1)
    {
        ball.direction = 0; //direction vers le haut
    }
    else if (ball.x == 0)
    {
        ball.direction = 1;
    }else if (ball.x == N-1)
    {
        ball.direction = 3;
    }
    return ball.direction;
}

int main(int argc, char *argv[]) {

    int N;
    int niveau;
    Ball ball,coordonneFin;
    int diagonalTouched;
    bool playGame = true,playPartie = true;
    Uint32 startTime = SDL_GetTicks();

    initialize();

    printf("Donner N : ");
    //scanf("%d", &N);
    N = 6;
    int grid[N][N];

    niveau = N - 5;  //for test

    while(playGame){

        generationDiagonal(N, grid, niveau);
        affichageGrid(N,grid);
        ball = generationPointDepart(N);                                    printf("Point de depart est ( %d , %d ) \n",ball.x,ball.y);
        ball.direction = Direction_depart(ball,N);
        coordonneFin = ball;
        calculeTrajectoire(N,grid,&coordonneFin,&diagonalTouched);          printf("Point d'arrivé  est ( %d , %d ) \n",coordonneFin.x,coordonneFin.y);

        renderMatrix(N, grid, startTime);
        drawBall(ball,N);
        SDL_RenderPresent(renderer);

        playPartie = true;

        while (playPartie) {
            SDL_Event event;

            while (SDL_PollEvent(&event) && playPartie==true){
                if (event.type == SDL_QUIT) {
                    cleanup();
                    return 0;
                } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (event.button.button == SDL_BUTTON_LEFT) {

                        int mouseX, mouseY;
                        SDL_GetMouseState(&mouseX, &mouseY);

                        // Obtenir les coordonnées de la case cliquée
                        Ball clickedCell = handleButtonClick(mouseX, mouseY, N);
                        printf("cordonné estime ( %d , %d )\n",clickedCell.x,clickedCell.y);

                        // Vérifier si les coordonnées sont égales à (2, 0)
                        if (clickedCell.x == coordonneFin.x && clickedCell.y == coordonneFin.y) {
                            printf("Raba7\n");
                            usleep(500000);
                            animationTrajectoire(N, grid, ball);
                            N++;
                        }else{
                            printf("Khserrrt\n");
                            usleep(500000);
                            animationTrajectoire(N, grid, ball);
                        }
                        usleep(5000000);
                        playPartie = false;
                    }
                }
            }
        }
    }
 return 0;
}
