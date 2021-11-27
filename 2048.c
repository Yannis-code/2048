#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constantes.h"
#include "console.h"
#include "graphique.h"
#include "game.h"

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    int size = 4;
    grille * plate = createGame(size);
    if (plate == NULL)
    {
        fprintf(stderr, "Impossible de creer de grille de jeu de taille %dx%d\n", size, size);
        return EXIT_FAILURE;
    }
    
    mooveGame(plate);
    freeGame(plate);
    return EXIT_SUCCESS;
}
// Quand tu as ton tableau de rempli, qu'il faut impérativement jouer
// en bas MAIS que tu joues à droite alors que ça ne fait rien, il ne 
//faut pas qu'il t'afficher perdu mais qu'il te redemande une entrée clavier