#ifndef GAME
#define GAME

#include "constantes.h"

void placeRandomNumber(grille * plate, int nb); 

int tryRandomNumber(grille * plate);

int gameIsFinish(grille * plate);

int * getTabCourrante(grille * plate, int i, int j, int direction);

int mergeGrille(grille * plate, int direction);

void mooveGame(grille * plate, int newGame);

void save(grille * plate);

grille * load(int taille);

grille * createGame(int size);

void freeGame(grille * plate);

#endif