#ifndef lib2048
#define lib2048

enum{HAUT, BAS, GAUCHE, DROITE};

typedef struct Ggrille
{
    int ** tab;
    int score;
    int sizeTab;
} grille;

// vider le buffer pour éviter le bug des scanf
void vider_buffer(void);

// Renvoie le nombre de chiffres d'un nombre (exemple: "2048" reverra 4)
int getSizeOfNumber(int number);

// Renvoie le plus grand nombre contenu dans le tableau
int getMaxNumber(grille * plate);

// Affiche un tableau 2D
void printGame(grille * plate);

// Place un "2" à une position libre (="0"), et aléatoire dans le tableau
void placeRandomNumber(grille * plate);

int tryRandomNumber(grille * plate);

// Détermine si la partie est terminé
int gameIsFinish(grille * plate);

int * getTabCourrante(grille  * plate, int i, int j, int direction);

void mergeGrille(grille  * plate, int direction);

void mooveGame(grille * plate);

// Créer un tableau vide avec une certaine dimension
grille * createGame(int size);

// Libère la mémoire prélevé par le tableau
void freeGame(grille * plate);

#endif