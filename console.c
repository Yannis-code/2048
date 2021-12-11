/**
 * @file console.c
 * @author ROCHE Yannis  - yannis.roche@etu.uca.fr  - 22002168
 * @author DUPOIS Thomas - thomas.dupois@etu.uca.fr - 22001214
 * @brief Fichier de la partie affichage console du programme
 * @version 0.1
 * @date 2021-12-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

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
        for (int j = 0; j < plate->sizeTab; j++)
            if(plate->tab[i][j] > max)
                max = plate->tab[i][j];
    return max;
}

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

void printSeparator(int size)
{
    printf("======================");
    for (int i = 0; i < size; i++)
        printf("=");
    printf("\n");
}

void printGame(grille * plate) // Affiche un tableau 2D
{
    int max = getMaxNumber(plate);
    int size = getSizeOfNumber(plate->bestScore) + getSizeOfNumber(plate->score);
    system("clear");
    printSeparator(size);
    printf("       [d]    -> Droite\n   [q] ou [g] -> Gauche\n   [z] ou [h] -> Haut\n   [s] ou [b] -> Bas\n    [ESPACE]  -> Quitter\n    [ENTRER]  -> Valider\n");
    printSeparator(size);
    printf(" SCORE: %d | Meilleur: %d\n", plate->score, plate->bestScore);
    printSeparator(size);
    for (int i = 0; i < plate->sizeTab; i++)
    {
        for (int j = 0; j < plate->sizeTab; j++)
        {
            if (j == 0) printf("> ");
                printf("| %*llu ", getSizeOfNumber(max), plate->tab[i][j]);
        }
        printf("|\n");
    }
    printSeparator(size);
}