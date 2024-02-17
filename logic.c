#include "logic.h"

Ball generationPointDepart(int N) {
    Ball pointDepart;
    int cote;
    do
    {
        srand(time(NULL));
        cote = rand() % 4;
    } while (cote < 0);

    switch (cote) {
        case 0: // Premi�re ligne
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
        case 1: // Derni�re
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
        case 2: // Premi�re colonne
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
        case 3: // Derni�re
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

void transposeGrid(int N,int grid[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            int temp = grid[i][j];
            grid[i][j] = grid[j][i];
            grid[j][i] = temp;
        }
    }

    for(int i = N/2; i<N;i++){
        for(int j = 0 ; j <N/2 ; j++){
            int temp = grid[i][j];
            grid[i][j] = grid[i][N-j-1];
            grid[i][N-j-1] = temp;
        }
    }
}

void generationDiagonal(int N, int grid[N][N], int Niveau) {
    int maxDiagonals = (N-2)*(N-2) / 2 ; // Le nombre maximal de diagonales
    int i, j, temp, d = 0;

    // Initialiser le g�n�rateur de nombres al�atoires avec une graine
    srand(time(NULL));

    // Remplir la matrice avec des z�ros
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
    transposeGrid(N,grid);
}

// Fonction pour calculer la trajectoire de la bille
void calculeTrajectoire(int N,int grid[N][N],Ball* ball,int* Diagonal_touche) {
    int orientation;
    *Diagonal_touche = 0;
    do {
        orientation = grid[ball->y][ball->x];
        if (orientation == DIAGONAL_LEFT_TO_RIGHT) {
            // Diagonale de gauche � droite, ajustez la trajectoire
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
            // Diagonale de droite � gauche, ajustez la trajectoire
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

// Fonction pour calculer le nombre de diagonals dans la matrice
int calculeDiagonal(int N,int grid[N][N]) {
    int i,j;
    int Diagonal_number = 0;

    for (i=1;i<N-1;i++){
        for(j=1;j<N-1;j++){
            if (grid[i][j] == 1 || grid[i][j] == 2){
                Diagonal_number ++;
            }
        }
    }
    return Diagonal_number;
}

// Fonction calcule score de la Partie
int calculeScorePartie(int N,int Diagonal_touche , int Diagonal_number,bool Game_win){
    int game_score;
    if (Game_win == true)
    {
        game_score = (N-5)*Diagonal_touche*10 + (N-5)*(Diagonal_number-Diagonal_touche)*5 ;
    }else
    {
        game_score = 0;
    }

    return game_score;
}

// Fonction Date Aujourd hui
Date DateAujourdhui() {
    Date date;
    time_t timestamp;
    struct tm *tm_info;

    // Obtenir la date actuelle
    time(&timestamp);
    tm_info = localtime(&timestamp);

    date.jour = tm_info->tm_mday;
    date.mois = tm_info->tm_mon + 1;
    date.annee = tm_info->tm_year + 1900;

    return date;
}

//Affichage de la date
void affichageDate(Player player){
    player.date = DateAujourdhui();
    printf("Date : %02d/%02d/%d\n", player.date.jour, player.date.mois, player.date.annee);
}

// fonction de verification si le joueur exest 1 si oui 0 sinon
int joueurExiste(char *username, Player *player) {
    FILE *file;
    file = fopen("players.bin", "rb");

    if (file == NULL) {
        perror("Erreur d'ouverture du fichier.");
        return 0; // Le joueur n'existe pas (fichier inexistant)
    }

    while (fread(player, sizeof(Player), 1, file)) {
        if (strcmp(player->username, username) == 0) {
            fclose(file);
            return 1; // Le joueur existe
        }
    }

    fclose(file);
    return 0; // Le joueur n'existe pas
}

//fonction qui charge les information du joueur
void chargerJoueur(const char *username, Player *player) {
    FILE *file;
    file = fopen("players.bin", "rb");

    if (file == NULL) {
        perror("Erreur d'ouverture du fichier.");
        return;
    }

    while (fread(player, sizeof(Player), 1, file)) {
        if (strcmp(player->username, username) == 0) {
            fclose(file);
            return;
        }
    }

    fclose(file);
}

//cree un nouveau joueur
void enregistrerJoueur(const Player *player) {
    FILE *file;
    file = fopen("players.bin", "ab");

    if (file == NULL) {
        perror("Erreur d'ouverture du fichier.");
        return;
    }

    fwrite(player, sizeof(Player), 1, file);
    fclose(file);
}

//mettre a jours les info
void mettreAJourJoueur(const char *username, Player *newPlayer) {
    FILE *file;
    file = fopen("players.bin", "r+b");

    if (file == NULL) {
        perror("Erreur d'ouverture du fichier.");
        return;
    }

    Player player;

    while (fread(&player, sizeof(Player), 1, file)) {
        if (strcmp(player.username, username) == 0) {
            fseek(file, -sizeof(Player), SEEK_CUR);
            fwrite(newPlayer, sizeof(Player), 1, file);
            fclose(file);
            return;
        }
    }

    fclose(file);
}

void gestionUtilisateur(Player *joueur, bool isNewUser) {
    Player player;
    char username[20];
    printf("Êtes-vous un nouvel utilisateur ? (1 pour Oui, 0 pour Non) : ");
    scanf("%d", &isNewUser);

    if (isNewUser) {
        do {
            printf("Entrez un nom d'utilisateur : ");
            scanf("%s", username);

            if (joueurExiste(username, &player)) {
                printf("Ce nom d'utilisateur est déjà utilisé. Veuillez en choisir un autre.\n");
            }
        } while (joueurExiste(username, &player));

        //strncpy(player.username, username, sizeof(player.username) - 1);
        strcpy(player.username, username);

        player.username[sizeof(player.username) - 1] = '\0';

        player.Niveau = 1;
        player.Partie = 1;
        player.score = 0;
        player.echec_consecutive = 0;
        player.win_consecutive = 0;
        player.date = DateAujourdhui();

        enregistrerJoueur(&player);
        printf("Nouvel utilisateur enregistré.\n");
    } else {
        do{
            printf("Entrez votre nom d'utilisateur : ");
            scanf("%s", username);

            if (joueurExiste(username, &player)) {
                int choix;
                chargerJoueur(username, &player);

                printf("Utilisateur existant. Que voulez-vous faire ?\n");
                printf("1. Continuer\n");
                printf("2. Recommencer a zero\n");
                scanf("%d", &choix);

                if (choix == 2) {
                    player.Niveau = 1;
                    player.Partie = 1;
                    player.score = 0;
                    player.echec_consecutive = 0;
                    player.win_consecutive = 0;
                    mettreAJourJoueur(username , &player);
                }

            } else {
                printf("Utilisateur non trouvé.\n");
            }
        }while (!joueurExiste(username, &player));
    }
    (*joueur) = player;
}

void initPlayer(Player *player){
    player->Niveau = 1;
    player->Partie = 1;
    player->score = 0;
    player->win_consecutive = 0;
    player->echec_consecutive = 0;
    player->date = DateAujourdhui();
}

void MatrixToGrid(int N, int grid[N][N], int matrix[22][22])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            grid[i][j] = matrix[i][j];
        }
    }
}

void GridToMatrix(int N, int grid[N][N], int matrix[22][22])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            matrix[i][j] = grid[i][j];
        }
    }
}

void etatPartie(Player *player, bool gameWin, int score)
{
    if (gameWin)
    {
        player->Partie++;
        player->score = player->score + score;
        player->win_consecutive++;
        player->echec_consecutive = 0;
        player->date = DateAujourdhui();
        Mix_PlayChannel(-1, success, 0);

        if (player->Partie >= 10 || player->win_consecutive >= 4)
        {
            if(player->Niveau < 20){
                player->Niveau++;
                player->Partie = 1;
            }
            player->win_consecutive = 1;
            Mix_PlayChannel(-1, level_win, 0);
        }
    }
    else
    {
        Mix_PlayChannel(-1, game_over, 0);
        player->echec_consecutive++;
        player->win_consecutive = 0;
        if (player->echec_consecutive >= 3)
        {
            if (player->Niveau > 1)
            {
                player->Niveau--;
                player->Partie = 1;
                player->echec_consecutive = 0;
            }
            else
            {
                player->Niveau = 1;
                player->Partie = 1;
                player->echec_consecutive = 0;
            }
        }
    }
}

void remplirTableauDepuisFichierBinaire(Player T[100], int *nombreDeJoueurs)
{
    FILE *file = fopen("players.bin", "rb");

    if (!file)
    {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    *nombreDeJoueurs = 0;

    while (fread(&T[*nombreDeJoueurs], sizeof(Player), 1, file) == 1)
    {
        (*nombreDeJoueurs)++;
    }

    fclose(file);
}

void trierJoueursParScore(Player T[100], int tabSize)
{
    int i, j;
    Player temp;

    for (i = 1; i < tabSize; ++i)
    {
        temp = T[i];
        j = i - 1;

        // Déplacer les éléments du tableau [0..i-1] qui sont plus grands que temp vers une position à l'avant de leur position actuelle
        while (j >= 0 && T[j].score < temp.score)
        {
            T[j + 1] = T[j];
            --j;
        }

        // Insérer temp à sa position correcte dans le tableau trié
        T[j + 1] = temp;
    }
}

void initMatrix(int N, int grid[N][N]){
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            grid[i][j] = 0;
        }
    }
}



