#ifndef GAME
#define GAME

#include "constantes.h"

void placeRandomNumber(grille * plate, int nb); 

int checkFreeSpace(grille * plate);

int gameOver(grille * plate);

int * getArrayPointer(grille * plate, int i, int j, int direction);

int updateGrid(grille * plate, int direction);

void saveGame(grille * plate);

int loadBestScore(int taille);

grille * loadGame(int taille);

grille * newGrid(int size);

void freeGrid(grille * plate);

void consoleGameLoop(grille * plate, int newGame);

void graphiqueGameLoop(grille * plate, int newGame);

#endif