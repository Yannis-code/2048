#ifndef GRAPHIQUE
#define GRAPHIQUE

#include "constantes.h"

SDL_Surface * initSDL(SDL_Surface * ecran);

int eventSDL(void);

void displayGrid(SDL_Surface * ecran, grille* plate, int windowWidth, int windowHeight);

int quitSDL(void);

#endif