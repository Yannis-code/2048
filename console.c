#include <stdio.h>
#include <stdlib.h>

#include "constantes.h"
#include "console.h"

void vider_buffer(void)	 			// vider le buffer pour éviter le bug des scanf
{
    int c; 
    do {
        c = getchar();
    } while(c != '\n' && c != EOF);
}

int getSizeOfNumber(int number) // Renvoie le nombre de chiffres d'un nombre (exemple: "2048" reverra 4)
{
    int n = 0;
    while(number >= 10)
    {
        n++;
        number /= 10;
    }
    return n+1;
}

int getMaxNumber(grille * plate) // Renvoie le plus grand nombre contenu dans le tableau
{
    int max = 0;
    for (int i = 0; i < plate->sizeTab; i++)
    {
        for (int j = 0; j < plate->sizeTab; j++)
        {
            if(plate->tab[i][j] > max)
                max = plate->tab[i][j];
        }
    }
    return max;
}

void printGame(grille * plate) // Affiche un tableau 2D
{
    int max = getMaxNumber(plate);
    printf("SCORE: %d\n", plate->score);
    for (int i = 0; i < plate->sizeTab; i++)
    {
        for (int j = 0; j < plate->sizeTab; j++)
            printf("| %*d ", getSizeOfNumber(max), plate->tab[i][j]);
        printf("|\n");
    }
    printf("\n");
}
