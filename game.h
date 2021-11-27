#ifndef GAME
#define GAME

#include "constantes.h"

void placeRandomNumber(grille * plate); 

int tryRandomNumber(grille * plate);

int gameIsFinish(grille * plate);

int * getTabCourrante(grille * plate, int i, int j, int direction);

void mergeGrille(grille * plate, int direction);

void mooveGame(grille * plate);

grille * createGame(int size);

void freeGame(grille * plate);

#endif