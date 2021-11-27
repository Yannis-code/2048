#ifndef GAME
#define GAME

#include "constantes.h"

void placeRandomNumber(grille * plate); 

int tryRandomNumber(grille * plate);

int gameIsFinish(grille * plate);

int * getTabCourrante(grille * plate, int i, int j, int direction);

int mergeGrille(grille * plate, int direction);

void mooveGame(grille * plate);

void save(grille * plate);

grille * load(void);

grille * createGame(int size);

void freeGame(grille * plate);

#endif