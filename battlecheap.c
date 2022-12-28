#include <stdio.h>
#include <stdlib.h>

struct Ship
{
    int pos_i, pos_f; // selon le tableau 10x10
    char name[32];
    int length;
    char des;
    int isPlaced;
};
typedef struct Ship ship;

ship porteAvions = {.name = "Porte-avions", .length = 5};
ship croiseur = {.name = "Croiseur", .length = 4};
ship contreTorpilleur = {.name = "Contre-torpilleurs", .length = 3};
ship torpilleur = {.name = "Torpilleur", .length = 2};

ship crewPlayer[5] = {
    {.name = "Porte-avions", .length = 5, .isPlaced = 0},
    {.name = "Croiseur", .length = 4, .isPlaced = 0},
    {.name = "Contre-torpilleurs", .length = 3, .isPlaced = 0},
    {.name = "Contre-torpilleurs", .length = 3, .isPlaced = 0},
    {.name = "Torpilleur", .length = 2, .isPlaced = 0}};

ship crewBot[5] = {
    {.name = "Porte-avions", .length = 5, .isPlaced = 0},
    {.name = "Croiseur", .length = 4, .isPlaced = 0},
    {.name = "Contre-torpilleurs", .length = 3, .isPlaced = 0},
    {.name = "Contre-torpilleurs", .length = 3, .isPlaced = 0},
    {.name = "Torpilleur", .length = 2, .isPlaced = 0}};

const int len = 10;
const int offset_size = 16;
const char alphabet[10] = "ABCDEFGHIJ";

char *makeGrid();                                           // créer un tableau
void offset();                                              // affiche un espace d elongeur off slent
void printl();                                              // affiche une ligne de longueue 44
void showGrid(char tab[]);                                  // fonction de debogage
void showTable(char tab_j[], char tab_b[]);                 // affiche la grille
int ALPHANUM(char c);                                       // transforme une lettre en entier
void initGridPlayer(char tab[]);                            // demande les coordonées des navires
int gridAuth(char tab[], int p, char x, int y, char dir);   // vereifie si les coordonées int du sens
void gridPlace(char tab[], int p, char x, int y, char dir); // place le navire selon les coordonées

int main()
{
    char *grid_player = makeGrid();
    char *grid_ATTACK_player = makeGrid();
    showGrid(grid_player);
    // initGridPlayer(grid_player);
    return 0;
}

char *makeGrid()
{
    char *tmp = (char *)malloc(len * len); // à préciser
    for (int i = 0; i < len * len; i += 1)
        tmp[i] = ' ';
    return tmp;
}

void offset()
{
    for (int i = 0; i < offset_size; i += 1) // afficher l'écart entre les deux tableaux
    {
        printf(" ");
    }
}

void printl()
{
    for (int i = 0; i < (len + 1) * 4; i += 1)
    {
        printf("-");
    }
}

void showGrid(char tab[])
{
    printf("///|");
    for (int i = 0; i < len; i += 1) // afficher les coordonées horizontales du tableau du joueur
    {
        printf(" %c |", alphabet[i]);
    }
    printf("\n");
    printl();
    printf("\n");
    for (int i = 1; i < len; i += 1)
    {
        printf("%d  |", i);
        for (int j = 0; j < len; j += 1)
        {
            printf(" %c |", tab[(i - 1) * 10 + j]); // char à l'emplacement de la ligne et colonne resp
        }
        printf("\n");
        printl();
        printf("\n");
    }
    printf("%d |", len);
    for (int i = 0; i < len; i += 1)
    {
        printf(" %c |", tab[(len - 1) * 10 + i]);
    }
    printf("\n");
    printl();
    printf("\n");
}

void showTable(char tab_j[], char tab_b[])
{

    printf("///|");
    for (int i = 0; i < len; i += 1) // afficher les coordonées horizontales du tableau du joueur
    {
        printf(" %c |", alphabet[i]);
    }
    offset();
    printf("///|");
    for (int i = 0; i < len; i += 1) // afficher les coordonées horizontales du tableau du bot
    {
        printf(" %c |", alphabet[i]);
    }
    printf("\n");
    printl();
    offset();
    printl();
    printf("\n");
    for (int i = 1; i < len; i += 1)
    {
        printf("%d  |", i);
        for (int j = 0; j < len; j += 1)
        {
            printf(" %c |", tab_j[(i - 1) * 10 + j]); // char à l'emplacement de la ligne et colonne resp
        }
        offset();
        printf("%d  |", i);
        for (int j = 0; j < len; j += 1)
        {
            printf(" %c |", tab_b[(i - 1) * 10 + j]); // char à l'emplacement de la ligne et colonne resp
        }
        printf("\n");
        printl();
        offset();
        printl();
        printf("\n");
    }
    printf("%d |", len);
    for (int i = 0; i < len; i += 1)
    {
        printf(" %c |", tab_j[(len - 1) * 10 + i]);
    }
    offset();
    printf("%d |", len);
    for (int i = 0; i < len; i += 1)
    {
        printf(" %c |", tab_b[(len - 1) * 10 + i]);
    }
    printf("\n");
    printl();
    offset();
    printl();
    printf("\n");
}

int ALPHANUM(char c)
{
    for (int i = 0; i < len; i += 1)
    {
        if (c == alphabet[i])
        {
            return i + 1;
        }
    }
    return -1;
}

void initGridPlayer(char tab[])
{
    // on a 5 bateaux. Il faut d'abord les afficher puis llaisser le joeuru les placer.
    printf("| ind | len |\n");
    for (int i = 0; i < 5; i += 1)
    {
        printf("|  %d  |  %d  |\n", i, crewPlayer[i].length);
    }
    // on commence par demander quel bateau le joueur veut placer
    for (int i = 0; i < 5; i += 1)
    {
        int p;
        char x;
        int y;
        char dir;
        int isDone = 0;
        while (!isDone)
        {
            printf("Entrez l'indice du navire que vous souhaitez placer : ");
            if (scanf("%d", &p) == 1)
            {
                fflush(stdin);
                printf("Entrez les coordonnées de depart du navire (A,3) : ");
                if (scanf("(%c,%d)", &x, &y) == 2)
                {
                    fflush(stdin);
                    printf("voulez vous vous decaler a droite ou a gauche (d, g) : ");
                    if (scanf("%c", &dir) == 1)
                    {
                        fflush(stdin);
                        if (gridAuth(tab, p, x, y, dir))
                        {
                            isDone = 1;
                            char *tmp = makeGrid();
                            showGrid(tab);
                            free(tmp);
                        }
                    }
                }
            }
        }
    }
}

int gridAuth(char tab[], int p, char x, int y, char dir)
{
    if (dir == 'd')
    {
        for (int i = 0; i < crewPlayer[p].length; i += 1)
        {
            if (tab[(y - 1) * len + ALPHANUM(x) - 1 + i] != ' ')
            {
                return 0;
            }
        }
    }
    else if (dir == 'g')
    {
        for (int i = 0; i < crewPlayer[p].length; i += 1)
        {
            if (tab[(y - 1) * len + ALPHANUM(x) - 1 + i * len] != ' ')
            {
                return 0;
            }
        }
    }
    return 1;
}

void gridPlace(char tab[], int p, char x, int y, char dir);