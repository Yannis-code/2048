/**
 * @file 2048.c
 * @author ROCHE Yannis  - yannis.roche@etu.uca.fr  - 22002168
 * @author DUPOIS Thomas - thomas.dupois@etu.uca.fr - 22001214
 * @brief Fichier principale du programme
 * @version 0.1
 * @date 2021-12-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constantes.h"
#include "game.h"
#include "console.h"

int main(int argc, char const *argv[])
{
    int affichage = 0, taille = 0;
    grille * plate;
    srand(time(NULL));

    if (argc < 3 || !sscanf(argv[1], "%d", &affichage) || !sscanf(argv[2], "%d", &taille) ||
    (affichage != 1 && affichage != 2) || taille < SIZE_GRID_MIN || taille > SIZE_GRID_MAX)
        menu(&affichage, &taille);
    
    if ( (plate = loadGame(taille)) != NULL)
    {
        if (affichage == 1)
            consoleGameLoop(plate);
        else
            graphiqueGameLoop(plate);
    }

    freeGrid(plate);
    return EXIT_SUCCESS;
}