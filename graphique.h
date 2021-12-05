#ifndef GRAPHIQUE
#define GRAPHIQUE

#include "constantes.h"

SDL_Surface * initSDL(SDL_Surface * ecran);

int eventSDL(void);

float getFontScale(int strLen, int charWidth, int charHeight, int maxWidth, int maxHeight);

SDL_Surface * getFont(TTF_Font * font, char * str, int charWidth, int charHeight, int maxWidth, int maxHeight, int colorR, int colorG, int colorB);

int * getColorRGB(int value);

font * loadFont(char * path);

gameTextures * initGraphicAssets(int maxTheoricTile);

void displayGrid(grille* plate, gameTextures * gameAsset, timer * gameTimer, int windowWidth, int windowHeight);

void freeGameTextures(grille* plate, gameTextures * gameAsset);

int quitSDL(void);

#endif