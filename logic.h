#ifndef LOGIC_H_INCLUDED
#define LOGIC_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <windows.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

//---------effect------------//
extern Mix_Chunk *bonk;
extern Mix_Chunk *click;
extern Mix_Chunk *game_over;
extern Mix_Chunk *level_win;
extern Mix_Chunk *success;

typedef struct {
    int x;
    int y;
    int direction;
} Ball;

typedef struct Date {
    int jour;
    int mois;
    int annee;
} Date;

typedef struct Player {
    char username[20];
    int Niveau;
    int Partie;
    int score;
    int echec_consecutive;
    int win_consecutive;
    Date date;
} Player;

#define DIAGONAL_LEFT_TO_RIGHT 1
#define DIAGONAL_RIGHT_TO_LEFT 2

Ball generationPointDepart(int N);
void transposeMatrix(int N,int grid[N][N]);
void generationDiagonal(int N, int grid[N][N], int Niveau);
void calculeTrajectoire(int N, int grid[N][N], Ball* ball, int* Diagonal_touche);
void affichageGrid(int N, int grid[N][N]);
int Direction_depart(Ball ball, int N);
int calculeDiagonal(int N,int grid[N][N]);
int calculeScorePartie(int N,int Diagonal_touche , int Diagonal_number,bool Game_win);
Date DateAujourdhui();
void affichageDate(Player player);
int joueurExiste(char *username, Player *player);
void chargerJoueur(const char *username, Player *player);
void enregistrerJoueur(const Player *player);
void mettreAJourJoueur(const char *username, Player *newPlayer);
void gestionUtilisateur(Player *joueur, bool isNewUser);
void initPlayer(Player *player);
void MatrixToGrid(int N, int grid[N][N], int matrix[22][22]);
void GridToMatrix(int N, int grid[N][N], int matrix[22][22]);
void etatPartie(Player *player, bool gameWin, int score);
void remplirTableauDepuisFichierBinaire(Player T[100], int *nombreDeJoueurs);
void trierJoueursParScore(Player T[100], int tabSize);
void initMatrix(int N, int grid[N][N]);


#endif // LOGIC_H_INCLUDED
