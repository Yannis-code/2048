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
    printf("Quelle taille de grille voulez vous jouer? [min: 2, max: 16]\n");
    while (!scanf("%d", taille) || *taille < 2 || *taille > 16)
    {
        printf("Quelle taille de grille voulez vous jouer? [min: 2, max: 16]\n");
    }
}

int main(int argc, char const *argv[])
{
    int affichage = 0, taille = 0;
    grille * plate;
    srand(time(NULL));

    if (argc < 3 || !sscanf(argv[1], "%d", &affichage) || !sscanf(argv[2], "%d", &taille) ||
    (affichage != 1 && affichage != 2) || taille < 2 || taille > 16)
        menu(&affichage, &taille);
    
    if ( (plate = loadGame(taille)) == NULL)
    {
        plate = newGrid(taille);
        plate->bestScore = loadBestScore(taille);
        if (affichage == 1)
            consoleGameLoop(plate, 1);
        else
            graphiqueGameLoop(plate, 1);
    }
    else
    {
        if (affichage == 1)
            consoleGameLoop(plate, 0);
        else
            graphiqueGameLoop(plate, 0);
    }

    freeGrid(plate);
    return EXIT_SUCCESS;
}