#include <stdio.h>
#include <stdlib.h>

#include "constantes.h"
#include "console.h"

void placeRandomNumber(grille * plate) // Place un "2" à une position libre (="0"), et aléatoire dans le tableau
{
    int i, j, x;
    do {
        i = (rand() % plate->sizeTab);
        j = (rand() % plate->sizeTab);
    } while (plate->tab[i][j] != 0);

    x = rand() % 2;
    if (x)
        plate->tab[i][j] = 2;
    else
        plate->tab[i][j] = 4;
}

int tryRandomNumber(grille * plate)
{
    for (int i = 0; i < plate->sizeTab; i++)
    {
        for (int j = 0; j < plate->sizeTab; j++)
        {
            if (plate->tab[i][j] == 0)
                return 1;
        }
    }
    return 0;
}

int gameIsFinish(grille * plate) // Détermine si la partie est terminé
{
    for (int i = 0; i < plate->sizeTab; i++)
    {
        for (int j = 0; j < plate->sizeTab; j++)
        {
            if (i < (plate->sizeTab)-1)
            {
                if(plate->tab[i][j] == plate->tab[i+1][j] || plate->tab[i][j] == 0 || plate->tab[i+1][j] == 0)
                    return 0;
            }
            if (j < (plate->sizeTab)-1)
            {
                if(plate->tab[i][j] == plate->tab[i][j+1] || plate->tab[i][j] == 0 || plate->tab[i][j+1] == 0)
                    return 0;
            }
        }
    } 
    return 1;
}

int * getTabCourrante(grille * plate, int i, int j, int direction)
{
    if (direction == DROITE)
        return &plate->tab[i][plate->sizeTab-1-j];
    else if (direction == GAUCHE)
        return &plate->tab[i][j];
    else if (direction == HAUT)
        return &plate->tab[j][i];
    else if (direction == BAS)
        return &plate->tab[plate->sizeTab-1-j][i];
    return NULL;
}

void mergeGrille(grille * plate, int direction)
{
    int i = 0, j = 0, k = 0;
    int * curseur;

    for (i = 0; i < plate->sizeTab; i++) // 
    {
        k = 0;
        curseur = getTabCourrante(plate, i, k, direction);
        for (j = 1; j < plate->sizeTab; j++) // 
        {
            if (*curseur && !(*curseur - *getTabCourrante(plate, i, j, direction)))
            {
                *curseur *= 2;
                plate->score += *curseur;
                *getTabCourrante(plate, i, j, direction) = 0;
                k++;
                curseur = getTabCourrante(plate, i, k, direction);
            }
            else if (*curseur && *curseur - *getTabCourrante(plate, i, j, direction) && *getTabCourrante(plate, i, j, direction))
            {
                k++;
                curseur = getTabCourrante(plate, i, k, direction);
                if (curseur != getTabCourrante(plate, i, j, direction))
                {
                    *curseur = *getTabCourrante(plate, i, j, direction);
                    *getTabCourrante(plate, i, j, direction) = 0;
                }
            }
            else if (!*curseur && *getTabCourrante(plate, i, j, direction))
            {
                *curseur = *getTabCourrante(plate, i, j, direction);
                *getTabCourrante(plate, i, j, direction) = 0;
            }
        }
    }
}

void mooveGame(grille * plate)
{
    char c;
    placeRandomNumber(plate);
    do
    {
        if (tryRandomNumber(plate))
            placeRandomNumber(plate);
        else
        {
            printf("Perdu! \n");
            return;
        }
        printGame(plate);
        scanf("%c", &c);
        if (c == 'z' || c == 'Z' || c == 'h' || c == 'H') {mergeGrille(plate, HAUT);}
        else if (c == 'q' || c == 'Q' || c == 'g' || c == 'G') {mergeGrille(plate, GAUCHE);}
        else if (c == 's' || c == 'S' || c == 'b' || c == 'B') {mergeGrille(plate, BAS);}
        else if (c == 'd' || c == 'D') {mergeGrille(plate, DROITE);}
        vider_buffer();            
    } while (!(gameIsFinish(plate)));
    printf("PERDU! \n");
}

grille * createGame(int size) // Créer un tableau vide avec une certaine dimension 
{
    grille * plate = (grille *) malloc(sizeof(grille));
    if (plate == NULL)
        return NULL;

    plate->sizeTab = size;
    plate->score = 0;

    plate->tab = (int **) malloc(plate->sizeTab * sizeof(int*));
    if (plate->tab == NULL)
        return NULL;
    
    for (int i = 0; i < size; i++)
    {
        plate->tab[i] = (int*) malloc(plate->sizeTab * sizeof(int));
        if (plate->tab[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(plate->tab[j]);
                plate->tab[j] = NULL;
            }
            free(plate->tab);
            plate->tab = NULL;
            free(plate);
            plate = NULL;
            return NULL;
        }
        else
        {
            for (int j = 0; j < plate->sizeTab; j++)
            {
                plate->tab[i][j] = 0;
            }
        }
    }
    return plate;
}

void freeGame(grille * plate) // Libère la mémoire prélevé par le tableau
{
    for (int i = 0; i < plate->sizeTab; i++)
    {
        if (plate->tab[i] != NULL)
        {
            free(plate->tab[i]);
            plate->tab[i] = NULL;
        }
    }
    free(plate->tab);
    plate->tab = NULL;
    free(plate);
    plate->tab = NULL;
}