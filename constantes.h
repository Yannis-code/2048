#ifndef CONSTANTES
#define CONSTANTES

#include <SDL/SDL_ttf.h>

#define DEBUG fprintf(stderr, "[DEBUG]: File: %s, Func: %s, Line: %d\n", __FILE__, __func__, __LINE__);

enum{HAUT, BAS, GAUCHE, DROITE};

typedef struct Ggrille
{
    int ** tab;
    int score;
    int bestScore;
    int sizeTab;
} grille;

#endif