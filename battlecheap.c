#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int name_len = 32;

struct Ship
{
    int pos_i, pos_f; // selon le tableau 10x10
    char name[name_len];
    int length;
    int isPlaced;
    int finito;
};
typedef struct Ship ship;

// ship porteAvions = {.name = "Porte-avions", .length = 5};
// ship croiseur = {.name = "Croiseur", .length = 4};
// ship contreTorpilleur = {.name = "Contre-torpilleurs", .length = 3};
// ship torpilleur = {.name = "Torpilleur", .length = 2};

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
const int lines_len = (len + 1) * 4;

char *
makeGrid();                                                              // créer un tableau
void offset();                                                           // affiche un espace d elongeur off slent
void printl(int n);                                                      // affiche une ligne de longueue 44
void showGrid(char tab[]);                                               // fonction de debogage
void showTable(char tab_j[], char tab_b[]);                              // affiche la grille
int ALPHANUM(char c);                                                    // transforme une lettre en entier
char BOOLCHAR(int bool);                                                 // Renvoie 'Y' si bool == 1, 'N' sinon
void showShips(ship list[]);                                             // Affiche les different navires
void initGridPlayer(char tab[], ship list[]);                            // demande les coordonées des navires
void initGridBot(char tab[], ship list[]);                               // initialise aleatoirement les navires du bot
int gridAuth(char tab[], int p, char x, int y, char dir, ship list[]);   // vereifie si les coordonées int du sens
void gridPlace(char tab[], int p, char x, int y, char dir, ship list[]); // place le navire selon les coordonées
void torpedo(char grid_ATTACK_1[], char grid_2[], char x, int y);        // attaque du joueur
void requestTorpedo(char grid_ATTACK_1[], char grid_2[]);                // pour rendre le code souple
void torpedoCalibration(char grid_ATTACK_1[], char grid_2[]);            // pour le bot
int fatalTorpedo(char grid_ATTACK_1[], char grid_2[], ship list[]);      // verifie que le joueur 1 a fait coulé un navire du joueur 2
int peaceQuestionMark(ship list_1[], ship list_2[]);
void processing();

int main()
{
    char *grid_player = makeGrid();
    char *grid_ATTACK_player = makeGrid();
    char *grid_bot = makeGrid();
    char *grid_ATTACK_bot = makeGrid();
    // initGridPlayer(grid_player, crewPlayer);
    initGridBot(grid_player, crewPlayer);
    initGridBot(grid_bot, crewBot);
    int i = 0;
    while (!peaceQuestionMark(crewPlayer, crewBot))
    {
        showTable(grid_player, grid_ATTACK_player);
        printf("Tour du joueur: \n");
        requestTorpedo(grid_ATTACK_player, grid_bot);
        if (
            fatalTorpedo(grid_ATTACK_player, grid_bot, crewBot)) // ajouter quel navire a coulé
            printf("Vous avez coulé un navire !\n");
        printf("Tour du bot: \n");
        processing();
        torpedoCalibration(grid_ATTACK_bot, grid_player);
        if (fatalTorpedo(grid_ATTACK_bot, grid_player, crewPlayer))
            printf("Un de vos navires vient d'être détruit..\n");
    }
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

void printl(int n)
{
    for (int i = 0; i < n; i += 1) //(len + 1) * 4
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
    printl(lines_len);
    printf("\n");
    for (int i = 1; i < len; i += 1)
    {
        printf("%d  |", i);
        for (int j = 0; j < len; j += 1)
        {
            printf(" %c |", tab[(i - 1) * 10 + j]); // char à l'emplacement de la ligne et colonne resp
        }
        printf("\n");
        printl(lines_len);
        printf("\n");
    }
    printf("%d |", len);
    for (int i = 0; i < len; i += 1)
    {
        printf(" %c |", tab[(len - 1) * 10 + i]);
    }
    printf("\n");
    printl(lines_len);
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
    printl(lines_len);
    offset();
    printl(lines_len);
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
        printl(lines_len);
        offset();
        printl(lines_len);
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
    printl(lines_len);
    offset();
    printl(lines_len);
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

char BOOLCHAR(int bool)
{
    if (bool == 1)
    {
        return 'Y';
    }
    else
    {
        return 'N';
    }
}

void showShips(ship list[])
{
    printf("CARACTERISTIQUES DES NAVIRES : \n");
    printl(sizeof("| index | longueur | placé |") / sizeof(char));
    printf("\n| index | longueur | placé   |");
    for (int i = 0; i < 5; i += 1)
    {
        printf("\n");
        printl(sizeof("| index | longueur | placé |") / sizeof(char));
        printf("\n");
        printf("| %d     | %d        | %c       |", i, list[i].length, BOOLCHAR(list[i].isPlaced));
    }
}

void initGridPlayer(char tab[], ship list[])
{
    // on a 5 bateaux. Il faut d'abord les afficher puis llaisser le joeuru les placer.
    printf("\nPLACEMENT DES NAVIRES\n");

    // on commence par demander quel bateau le joueur veut placer
    for (int i = 0; i < 5; i += 1)
    {
        showGrid(tab);
        showShips(list);
        printf("\nVous avez %d navire(s) à placer.\n", 5 - i);
        int p;
        char x;
        int y;
        char dir;
        int isDone = 0;
        while (!isDone)
        {
            printf("Entrez l'indice du navire que vous souhaitez placer : ");
            fflush(stdin);
            if (scanf("%d", &p) == 1)
            {
                printf("Entrez les coordonnées de depart du navire (A,3) : ");
                fflush(stdin);
                if (scanf("(%c,%d)", &x, &y) == 2)
                {
                    printf("voulez vous vous decaler a droite ou a gauche (d, b) : ");

                    fflush(stdin);
                    if (scanf("%c", &dir) == 1)
                    {
                        fflush(stdin);
                        if (gridAuth(tab, p, x, y, dir, list))
                        {
                            gridPlace(tab, p, x, y, dir, list);
                            showGrid(tab);
                            list[p].isPlaced = 1;
                            printf("Navire en route, capitaine !\n");
                            isDone = 1;
                        }
                        else
                        {
                            printf("\nIl est impossible d'envoyer un navire à cet endroit..\nréessayez.\n");
                        }
                    }
                }
            }
        }
    }
}

void initGridBot(char tab[], ship list[])
{
    int y;
    char x, dir;
    char *tmp_dir = (char *)malloc(2);
    tmp_dir[0] = 'd';
    tmp_dir[1] = 'b';
    for (int i = 0; i < 5; i += 1) // 5 etant le nombre de navires
    {
        int isDone = 0;
        while (!isDone)
        {
            x = rand() % 10;
            y = rand() % 10;
            dir = tmp_dir[rand() % 2];
            if (gridAuth(tab, i, alphabet[x], y, dir, list))
            {
                gridPlace(tab, i, alphabet[x], y, dir, list); // TODO: soucis car dans grid place on modifie la liste des navires du joueur et pas celle du bot..
                isDone = 1;
            }
        }
    }
}

int gridAuth(char tab[], int p, char x, int y, char dir, ship list[])
{
    if (list[p].isPlaced == 1)
    {
        return 0;
    }
    else if (dir == 'd')
    {
        if (ALPHANUM(x) - 1 + (y - 1) * 10 + list[p].length > y * 10)
        {
            return 0;
        }
        for (int i = 0; i < list[p].length; i += 1)
        {
            if (tab[(y - 1) * len + ALPHANUM(x) - 1 + i] != ' ')
            {
                return 0;
            }
        }
    }
    else if (dir == 'b')
    {
        if (ALPHANUM(x) - 1 + (y - 1) * 10 + list[p].length * 10 > len * len)
        {
            return 0;
        }
        for (int i = 0; i < list[p].length; i += 1)
        {
            if (tab[(y - 1) * len + ALPHANUM(x) - 1 + i * len] != ' ')
            {
                return 0;
            }
        }
    }
    return 1;
}

void gridPlace(char tab[], int p, char x, int y, char dir, ship list[])
{
    if (dir == 'd')
    {
        list[p].pos_i = ALPHANUM(x) - 1 + (y - 1) * 10;
        list[p].pos_f = list[p].pos_i + list[p].length;
        for (int i = 0; i < list[p].length; i += 1)
        {
            tab[list[p].pos_i + i] = '-';
        } // On change tous les char par des -
    }
    else if (dir == 'b')
    {
        list[p].pos_i = ALPHANUM(x) - 1 + (y - 1) * 10;
        list[p].pos_f = list[p].pos_i + list[p].length * 10;
        for (int i = 0; i < list[p].length; i += 1)
        {
            tab[ALPHANUM(x) - 1 + (y - 1) * 10 + i * 10] = '!';
        }
    }
}

void requestTorpedo(char grid_ATTACK_1[], char grid_2[])
{
    printf("Entrez une coordonée :");
    char x;
    int y;
    fflush(stdin);
    if (scanf("(%c,%d)", &x, &y) != 2)
    {
        fflush(stdin);
        printf("Mauvaise coordonnée chef.\n");
        requestTorpedo(grid_ATTACK_1, grid_2);
    }
    else if (ALPHANUM(x) < 1 || ALPHANUM(x) > 10)
    {
        printf("Mauvaise coordonnée chef...\n");
        requestTorpedo(grid_ATTACK_1, grid_2);
    }
    else if (!(y >= 1 && y <= 10))
    {
        printf("Mauvaise coordonnée chef..\n");
        requestTorpedo(grid_ATTACK_1, grid_2);
    }
    else
    {
        torpedo(grid_ATTACK_1, grid_2, x, y);
    }
}

void torpedo(char grid_ATTACK_1[], char grid_2[], char x, int y)
{
    // printf("Entrez une coordonée :");
    // char x;
    // int y;
    // fflush(stdin);
    // if (scanf("(%c,%d)", &x, &y) != 2)
    // {
    //     printf("Mauvaise coordonnée chef..\n");
    //     torpedo(grid_ATTACK_1, grid_2);
    // }
    // else
    // {

    // x appartient a [ABCDEFGHIJ]
    // 1 ≤ y ≤ 10
    // if (ALPHANUM(x) < 1 || ALPHANUM(x) > 10)
    // {
    //     printf("Mauvaise coordonnée chef...\n");
    //     requestTorpedo(grid_ATTACK_1, grid_2);
    // }
    // else if (!(y >= 1 && y <= 10))
    // {
    //     printf("Mauvaise coordonnée chef..\n");
    //     requestTorpedo(grid_ATTACK_1, grid_2);
    // }
    if (grid_2[ALPHANUM(x) - 1 + (y - 1) * 10] == '!' || grid_2[ALPHANUM(x) - 1 + (y - 1) * 10] == '-')
    {
        printf("Touché !\n");
        grid_ATTACK_1[ALPHANUM(x) - 1 + (y - 1) * 10] = 'X'; // pour dire qu'il y a un impact
        grid_2[ALPHANUM(x) - 1 + (y - 1) * 10] = 'X';        // pour dire qu'il y a une impact
    }
    else if (grid_2[ALPHANUM(x) - 1 + (y - 1) * 10] == ' ')
    {
        printf("Plouf, pas touché..\n");
        grid_ATTACK_1[ALPHANUM(x) - 1 + (y - 1) * 10] = '.';
    }
}

void torpedoCalibration(char grid_ATTACK_1[], char grid_2[])
{
    int isAvailable = 0;
    int x, y;
    while (!isAvailable)
    {
        x = rand() % 10;
        y = rand() % 10;
        if (!(grid_ATTACK_1[x + y * 10] == 'X' || grid_ATTACK_1[x + y * 10] == 'o' || grid_ATTACK_1[x + y * 10] == '.'))
        {
            torpedo(grid_ATTACK_1, grid_2, x, y);
            isAvailable = 1;
        }
    }
}

int fatalTorpedo(char grid_ATTACK_1[], char grid_2[], ship list[]) // TODO:ajouter un parametre pour crewBot, crewPlayer
{
    int isBrokey;
    for (int i = 0; i < 5; i += 1) // nombre de navires = 5
    {
        isBrokey = 0;
        // si deplacement horiz (droite)
        if (list[i].pos_f - list[i].pos_i <= 10)
        {
            for (int j = 0; j < list[i].length; j += 1)
            {
                if (grid_ATTACK_1[list[i].pos_i + j] != 'X')
                {
                    isBrokey = 1;
                }
            }
            if (!isBrokey)
            {
                for (int j = 0; j < list[i].length; j += 1)
                {
                    grid_ATTACK_1[list[i].pos_i + j] = 'o';
                    grid_2[list[i].pos_i + j] = 'o';
                    list[i].finito = 1;
                }
                return 1;
            }
        }
        else if (list[i].pos_f - list[i].pos_i >= 10)
        {
            for (int j = 0; j < list[i].length; j += 1)
            {
                if (grid_ATTACK_1[list[i].pos_i + j * 10] != 'X')
                {
                    isBrokey = 1;
                }
            }
            if (!isBrokey)
            {
                for (int j = 0; j < list[i].length; j += 1)
                {
                    grid_ATTACK_1[list[i].pos_i + j * 10] = 'o';
                    grid_2[list[i].pos_i + j * 10] = 'o';
                    list[i].finito = 1;
                }
                return 1;
            }
        }
    }
    return 0;
}

int peaceQuestionMark(ship list_1[], ship list_2[])
{
    for (int i = 0; i < 5; i += 1)
    {
        if (list_1[i].finito == 0 || list_2[i].finito == 0)
        {
            return 0;
        }
    }
    return 1;
}

void processing()
{
    int dt = rand() % 3 + 1;
    sleep(dt);
}