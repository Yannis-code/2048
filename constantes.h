#ifndef CONSTANTES
#define CONSTANTES

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define DEBUG fprintf(stderr, "[DEBUG]: File: %s, Func: %s, Line: %d\n", __FILE__, __func__, __LINE__);

enum{HAUT, BAS, GAUCHE, DROITE};
enum{IN_GAME, GAME_OVER};

typedef struct _timer
{
    double loadedTime;
    double timeElapsed;
    double start_;
    float days;
    float hours;
    float minutes;
    float secondes;
} timer;

typedef struct _grille
{
    int ** tab;
    int score;
    int bestScore;
    short sizeTab;
    short status;
    timer * gameTimer;
} grille;

typedef struct _font
{
    TTF_Font * font;
    int charWidth;
    int charHeight;
} font;

typedef struct _rect
{
    SDL_Surface * surface;
    SDL_Rect box;
} rect;

typedef struct _gameTextures
{
    SDL_Surface * ecran;
    SDL_Surface ** tilesRendered;
    font * font;
    rect * grid;
    rect * tile;
} gameTextures;

#endif