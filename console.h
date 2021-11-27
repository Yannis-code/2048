#ifndef CONSOLE
#define CONSOLE

#include "constantes.h"

// vider le buffer pour éviter le bug des scanf
void vider_buffer(void);

// Renvoie le nombre de chiffres d'un nombre (exemple: "2048" reverra 4)
int getSizeOfNumber(int number);

// Renvoie le plus grand nombre contenu dans le tableau
int getMaxNumber(grille * plate);

// Affiche un tableau 2D
void printGame(grille * plate);

#endif