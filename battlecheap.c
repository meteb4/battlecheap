#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

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

void bsh();                                                                 // battlecheap shell
void rules();                                                               // les regles
void play();                                                                // le 'jeu'
char *makeGrid();                                                           // créer un tableau
void offset();                                                              // affiche un espace d elongeur off slent
void printl(int n);                                                         // affiche une ligne de longueue 44
void showGrid(char tab[]);                                                  // fonction de debogage
void showTable(char tab_j[], char tab_b[]);                                 // affiche la grille
int ALPHANUM(char c);                                                       // transforme une lettre en entier
char BOOLCHAR(int bool);                                                    // Renvoie 'Y' si bool == 1, 'N' sinon
void showShips(ship list[]);                                                // Affiche les different navires
void initGridPlayer(char tab[], ship list[]);                               // demande les coordonées des navires
void initGridBot(char tab[], ship list[]);                                  // initialise aleatoirement les navires du bot
int gridAuth(char tab[], int p, char x, int y, char dir, ship list[]);      // vereifie si les coordonées int du sens
void gridPlace(char tab[], int p, char x, int y, char dir, ship list[]);    // place le navire selon les coordonées
void torpedo(char grid_ATTACK_1[], char grid_2[], char x, int y, int *hit); // attaque du joueur
void requestTorpedo(char grid_ATTACK_1[], char grid_2[], int *hit);         // pour rendre le code souple
void torpedoCalibration(char grid_ATTACK_1[], char grid_2[], int *hit);     // pour le bot
int fatalTorpedo(char grid_ATTACK_1[], char grid_2[], ship list[]);         // verifie que le joueur 1 a fait coulé un navire du joueur 2
int peaceQuestionMark(ship list_1[], ship list_2[], int *a, int *b);
void processing();

int main()
{
    srand(time(0));
    bsh();
    return 0;
}

void bsh()
{
    char str[] = "\n _           _   _   _           _                      \n| |         | | | | | |         | |                     \n| |__   __ _| |_| |_| | ___  ___| |__   ___  __ _ _ __  \n| '_ \\ / _` | __| __| |/ _ \\/ __| '_ \\ / _ \\/ _` | '_ \\ \n| |_) | (_| | |_| |_| |  __/ (__| | | |  __/ (_| | |_) |\n|_.__/ \\__,_|\\__|\\__|_|\\___|\\___|_| |_|\\___|\\__,_| .__/ \n                                                 | |    \n                                                 |_|\n\n";
    for (int i = 0; i < sizeof(str) / sizeof(char); i += 1)
    {
        printf("%c", str[i]);
    }
    // Saluer le joeur
    printf("\nbsh>Voici les differentes commandes disponibles : \n");
    printf("     - \033[1mrules\033[0m: ennonce les regles\n");
    printf("     - \033[1mships\033[0m: Fait une liste des bateaux\n");
    printf("     - \033[1mhelp\033[0m: Fait une liste des bateaux\n");
    printf("     - \033[1mplay\033[0m: commence la partie\n");
    printf("     - \033[1mexit\033[0m: arrete la partie\n");
    char tmp[10] = {' '};
    int finito = 0;
    while (!finito)
    {
        printf("bsh>");
        if (scanf("%s", tmp) == 1)
        {
            if (strcmp(tmp, "rules") == 0)
            {
                rules();
            }
            else if (strcmp(tmp, "ships") == 0)
            {
                // Faire la liste des bateaux
                // listShips(crewBot);
            }
            else if (strcmp(tmp, "help") == 0)
            {
                printf("Voici les differents controles\n");
                printf("  - Quand c'est votre tour, il suffit, pour attaquer, d'entrer une coordonnée.\n    Format: [A:J],[1:10]. Ne pas oublier la virgule.\n    Exemple: bsh>B,5\n");
            }
            else if (strcmp(tmp, "play") == 0)
            {
                // Renvoyer vers la fonction jouer
                play();
            }
            else if (strcmp(tmp, "exit") == 0)
            {
                // arrete la partie
                finito = 1;
            }
            else
            {
                printf("  Command %s wasn't found", tmp);
                fflush(stdin);
            }
        }
    }
}

void rules()
{
    printf("\033[1mRegles\033[0m\n");
    printf("Vous allez jouer contre une intelligence artificielle à la \033[1mbattaille navale\033[0m. Chacun de vous aura deux grilles de 10x10 indicées de [A,J] et [1,10].\n");
    printf("La première est l'emplacement de vos navires, que vous placerez.\n");
    printf("La première est l'emplacement de vos navires, que vous placerez.\n");
    printf("Chaque joueur à son tour annonce une case: soit \"touché\" pour un navire impacté mais encore en flotte, soit \"coulé\" si vous venez de couler un navier. Sinon \"plouf\"\n");
    printf("--- --- --- --- --- --- --- --- --- ---\n");
}

void play()
{
    // Créer les tableaux
    char *grid_player = makeGrid();
    char *grid_ATTACK_player = makeGrid();
    char *grid_bot = makeGrid();
    char *grid_ATTACK_bot = makeGrid();

    // initialiser les tableaux
    initGridBot(grid_player, crewPlayer);
    initGridBot(grid_bot, crewBot);

    // On continue de jouer tant que tout les navires d'au moins un joueur ne soient pas coulés
    // A chaque debut de tour, on affiche les tableaux
    // Si le joueur touche, il peut rejouer, sinon c'est au tour du bot, et ainsi de suite
    int a, b;
    while (!(peaceQuestionMark(crewPlayer, crewBot, &a, &b)))
    {
        printf("\033[1mC'est au tour du joueur\033[0m\n");
        int combo = 1;
        while (combo)
        {
            showTable(grid_player, grid_ATTACK_player);
            int tmp = 0;
            requestTorpedo(grid_ATTACK_player, grid_bot, &tmp);
            if (fatalTorpedo(grid_ATTACK_player, grid_bot, crewBot))
            {
                printf("Coulé\n");
            }
            if (peaceQuestionMark(crewPlayer, crewBot, &a, &b))
            {
                break;
            }
            if (tmp == 0)
            {
                combo = 0;
            }
        }
        printf("\033[1mC'est au tour du bot\033[0m\n");
        combo = 1;
        while (combo)
        {
            int tmp = 0;
            torpedoCalibration(grid_ATTACK_bot, grid_player, &tmp);
            if (fatalTorpedo(grid_ATTACK_bot, grid_player, crewPlayer))
            {
                printf("Coulé\n");
            }
            if (peaceQuestionMark(crewPlayer, crewBot, &a, &b))
            {
                break;
            }
            {
                if (tmp == 0)
                {
                    combo = 0;
                }
            }
        }
    }
    // La partie est finie
    // On veut connaitre le gagnant
    peaceQuestionMark(crewPlayer, crewBot, &a, &b);
    if (a == 0)
    {
        printf("\033[1mC'est vous, le joueur, le GRAND gagnant de cette bataille navale !\033[0m\n");
    }
    else if (b == 0)
    {
        printf("\033[1mC'est vous, le joueur, le GRAND perdant de cette bataille navale ..\033[0m\n");
    }
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
            printf(" \033[1;34m%c\033[0m |", tab_j[(i - 1) * 10 + j]); // char à l'emplacement de la ligne et colonne resp
        }
        offset();
        printf("%d  |", i);
        for (int j = 0; j < len; j += 1)
        {
            printf(" \033[1;31m%c\033[0m |", tab_b[(i - 1) * 10 + j]); // char à l'emplacement de la ligne et colonne resp
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
        printf(" \033[1;34m%c\033[1;0m |", tab_j[(len - 1) * 10 + i]);
    }
    offset();
    printf("%d |", len);
    for (int i = 0; i < len; i += 1)
    {
        printf(" \033[1;31m%c\033[0m |", tab_b[(len - 1) * 10 + i]);
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

void requestTorpedo(char grid_ATTACK_1[], char grid_2[], int *hit)
{
    printf("Entrez une coordonée :");
    char x;
    int y;
    fflush(stdin);
    if (scanf("%c,%d", &x, &y) != 2)
    {
        fflush(stdin);
        printf("Mauvaise coordonnée chef.\n");
        requestTorpedo(grid_ATTACK_1, grid_2, hit);
    }
    else if (ALPHANUM(x) < 1 || ALPHANUM(x) > 10)
    {
        printf("Mauvaise coordonnée chef...\n");
        requestTorpedo(grid_ATTACK_1, grid_2, hit);
    }
    else if (!(y >= 1 && y <= 10))
    {
        printf("Mauvaise coordonnée chef..\n");
        requestTorpedo(grid_ATTACK_1, grid_2, hit);
    }
    else
    {
        torpedo(grid_ATTACK_1, grid_2, x, y, hit);
    }
}

void torpedo(char grid_ATTACK_1[], char grid_2[], char x, int y, int *hit)
{
    if (grid_2[ALPHANUM(x) - 1 + (y - 1) * 10] == '!' || grid_2[ALPHANUM(x) - 1 + (y - 1) * 10] == '-')
    {
        printf("Touché !\n");
        grid_ATTACK_1[ALPHANUM(x) - 1 + (y - 1) * 10] = 'X'; // pour dire qu'il y a un impact
        grid_2[ALPHANUM(x) - 1 + (y - 1) * 10] = 'X';        // pour dire qu'il y a une impact
        *hit = 1;
    }
    else if (grid_2[ALPHANUM(x) - 1 + (y - 1) * 10] == ' ')
    {
        printf("Plouf, pas touché..\n");
        grid_ATTACK_1[ALPHANUM(x) - 1 + (y - 1) * 10] = '.';
        *hit = 0;
    }
}

void torpedoCalibration(char grid_ATTACK_1[], char grid_2[], int *hit)
{
    int isAvailable = 0;
    int x, y;
    while (!isAvailable)
    {
        x = rand() % 10;
        y = rand() % 10;
        if (grid_ATTACK_1[x + y * 10] != 'X' || grid_ATTACK_1[x + y * 10] != 'o' || grid_ATTACK_1[x + y * 10] != '.')
        {
            torpedo(grid_ATTACK_1, grid_2, x, y, hit);
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

int peaceQuestionMark(ship list_1[], ship list_2[], int *player, int *bot)
{
    int a, b = 1;
    for (int i = 0; i < 5; i += 1)
    {
        if (a && list_1[i].finito == 0)
        {
            a = 0;
        }
        if (b && list_2[i].finito == 0)
        {
            b = 0;
        }
    }
    *player = a;
    *bot = b;
    return a + b;
}

void processing()
{
    int dt = rand() % 3 + 1;
    sleep(dt);
}