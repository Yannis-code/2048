#ifndef GRAPHIQUE
#define GRAPHIQUE

#include "constantes.h"

SDL_Surface * initSDL(SDL_Surface * ecran);

int eventSDL(void);

float getFontScale(int strLen, int charWidth, int charHeight, int maxWidth, int maxHeight);

SDL_Surface * getFont(TTF_Font * font, char * str, int charWidth, int charHeight, int maxWidth, int maxHeight);

int * getColorRGB(int value);

void displayGrid(SDL_Surface * ecran, grille* plate, SDL_Surface ** renderedFont, int windowWidth, int windowHeight);

int quitSDL(void);

#endif