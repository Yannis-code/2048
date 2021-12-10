#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constantes.h"
#include "game.h"

void menu(int * affichage, int * taille)
{
    printf("Quelle type d'interface voulez vous lancer? [1:Console] [2:Graphique]\n");
    while (!scanf("%d", affichage) || (*affichage != 1 && *affichage != 2))
    {
        printf("Quelle type d'interface voulez vous lancer? [1:Console] [2:Graphique]\n");
    }
    printf("Quelle taille de grille voulez vous jouer? [min: %d, max: %d]\n", SIZE_GRID_MIN, SIZE_GRID_MAX);
    while (!scanf("%d", taille) || *taille < 2 || *taille > 7)
    {
        printf("Quelle taille de grille voulez vous jouer? [min: %d, max: %d]\n", SIZE_GRID_MIN, SIZE_GRID_MAX);
    }
}

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
            consoleGameLoop(plate, plate->status);
        else
            graphiqueGameLoop(plate, plate->status);
    }

    freeGrid(plate);
    return EXIT_SUCCESS;
}