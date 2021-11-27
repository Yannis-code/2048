#ifndef CONSTANTES
#define CONSTANTES

#define DEBUG fprintf(stderr, "[DEBUG]: File: %s, Func: %s, Line: %d\n", __FILE__, __func__, __LINE__);

enum{HAUT, BAS, GAUCHE, DROITE};

typedef struct Ggrille
{
    int ** tab;
    int score;
    int sizeTab;
} grille;

#endif