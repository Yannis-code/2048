/**
 * @file constantes.h
 * @author ROCHE Yannis  - yannis.roche@etu.uca.fr  - 22002168
 * @author DUPOIS Thomas - thomas.dupois@etu.uca.fr - 22001214
 * @brief Fichier header des constantes du programme
 * @version 0.1
 * @date 2021-12-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef CONSTANTES
#define CONSTANTES

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define DEBUG fprintf(stderr, "[DEBUG]: File: %s, Func: %s, Line: %d\n", __FILE__, __func__, __LINE__);

enum{HAUT, BAS, GAUCHE, DROITE, UNDO, MENU, VALIDE, EXIT};
enum{GAME_OVER, IN_GAME};

const int WIN_HEIGHT;
const int WIN_WIDTH;

const int SIZE_GRID_MIN;
const int SIZE_GRID_MAX;

const int FONT_SIZE_LOAD;
const char FONT_PATH[100];

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
    unsigned long long int ** tab;
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
    rect * menu;
    rect * undo;
    int nbTilesRendered;
} gameTextures;

#endif