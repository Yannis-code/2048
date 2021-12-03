#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_audio.h>

#include "game.h"
#include "console.h"
#include "graphique.h"

void placeRandomNumber(grille * plate, int nb) // Place un "2" à une position libre (="0"), et aléatoire dans le tableau
{
    int k, i, j, x;
    for (k = 0; k < nb; k++)
    {
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
}

int checkFreeSpace(grille * plate)
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

int gameOver(grille * plate) // Détermine si la partie est terminé
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

int * getArrayPointer(grille * plate, int i, int j, int direction)
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

int updateGrid(grille * plate, int direction)
{
    int i = 0, j = 0, k = 0, change = 0;
    int * curseur;

    for (i = 0; i < plate->sizeTab; i++) // 
    {
        k = 0;
        curseur = getArrayPointer(plate, i, k, direction);
        for (j = 1; j < plate->sizeTab; j++) // 
        {
            if (*curseur && !(*curseur - *getArrayPointer(plate, i, j, direction)))
            {
                *curseur *= 2;
                plate->score += *curseur;
                *getArrayPointer(plate, i, j, direction) = 0;
                k++;
                curseur = getArrayPointer(plate, i, k, direction);
                change ++;
            }
            else if (*curseur && *curseur - *getArrayPointer(plate, i, j, direction) && *getArrayPointer(plate, i, j, direction))
            {
                k++;
                curseur = getArrayPointer(plate, i, k, direction);
                if (curseur != getArrayPointer(plate, i, j, direction))
                {
                    *curseur = *getArrayPointer(plate, i, j, direction);
                    *getArrayPointer(plate, i, j, direction) = 0;
                    change ++;
                }
            }
            else if (!*curseur && *getArrayPointer(plate, i, j, direction))
            {
                *curseur = *getArrayPointer(plate, i, j, direction);
                *getArrayPointer(plate, i, j, direction) = 0;
                change ++;
            }
        }
    }
    return change;
}

void saveGame(grille * plate)
{
    char path[30];
    sprintf(path, "./games/save_%dx%d.txt", plate->sizeTab, plate->sizeTab);
    FILE * file = fopen(path, "w+");
    if (file == NULL)
    {
        fprintf(stderr, "Impossible de sauvegarder la partie\n");
        return;
    }
    fprintf(file, "%d\n", plate->score);
    for (int i = 0; i < plate->sizeTab; i++)
    {
        for (int j = 0; j < plate->sizeTab; j++)
        {
            fprintf(file, "%d ", plate->tab[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

grille * loadGame(int taille)
{
    char path[30];
    sprintf(path, "./games/save_%dx%d.txt", taille, taille);
    FILE * file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Aucune sauvegarde disponible\n");
        return NULL;
    }
    
    grille * plate = newGrid(taille);
    if (plate == NULL)
    {
        fclose(file);
        return NULL;
    }
    fscanf(file, "%d\n", &plate->score);
    for (int i = 0; i < plate->sizeTab; i++)
    {
        for (int j = 0; j < plate->sizeTab; j++)
        {
            fscanf(file, "%d ", &plate->tab[i][j]);
        }
        fscanf(file, "\n");
    }
    fclose(file);
    return plate;
}

grille * newGrid(int size) // Créer un tableau vide avec une certaine dimension 
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

void freeGrid(grille * plate) // Libère la mémoire prélevé par le tableau
{
    for (int i = 0; i < plate->sizeTab; i++)
    {
        if (plate->tab[i] != NULL)
        {
            free(plate->tab[i]);
        }
    }
    free(plate->tab);
    free(plate);
}


int getMaxTheoricalTile(int gridSize)
{
    int maxScore = 8;
    for (int i = 2; i < gridSize*gridSize; i++)
        maxScore += maxScore;
    return maxScore;
}


void consoleGameLoop(grille * plate, int newGame)
{
    char c;
    int direction, nbchangement = 1;
    if (newGame)
        placeRandomNumber(plate, 2);
    do
    {
        printGame(plate);
        vider_buffer();
        scanf("%c", &c);
        if (c == 'z' || c == 'Z' || c == 'h' || c == 'H') {direction = HAUT;}
        else if (c == 'q' || c == 'Q' || c == 'g' || c == 'G') {direction = GAUCHE;}
        else if (c == 's' || c == 'S' || c == 'b' || c == 'B') {direction = BAS;}
        else if (c == 'd' || c == 'D') {direction = DROITE;}
        nbchangement = updateGrid(plate, direction);
        if (checkFreeSpace(plate) && nbchangement)
            placeRandomNumber(plate, 1);
        saveGame(plate);
    } while (!(gameOver(plate)));
    char deleteFile[40];
    sprintf(deleteFile, "rm -f ./games/save_%dx%d.txt", plate->sizeTab, plate->sizeTab);
    system(deleteFile);
    printf("PERDU! \n");
}

void graphiqueGameLoop(grille * plate, int newGame)
{
    int nbchangement = 1, mainloop = 1, event = -10;
    int maxTheoricTile = getMaxTheoricalTile(plate->sizeTab);
    SDL_Surface * ecran = NULL;
    SDL_Surface ** renderedFont = (SDL_Surface **) malloc(maxTheoricTile * sizeof(SDL_Surface *));
    if ((ecran = initSDL(ecran)) == NULL)
    {
        fprintf(stderr, "Impossible de lancer une interface graphique\n");
        exit(EXIT_FAILURE);
    }

    if (newGame)
        placeRandomNumber(plate, 2);
    do
    {
        displayGrid(ecran, plate, renderedFont, 800, 1000);
        event = eventSDL();
        if (event == -1)
            mainloop = 0;
        if (event != -10 && event != -1)
        {
            nbchangement = updateGrid(plate, event);
            if (checkFreeSpace(plate) && nbchangement)
                placeRandomNumber(plate, 1);
            saveGame(plate);
        }
    } while (!(gameOver(plate)) && mainloop);
    char deleteFile[40];
    sprintf(deleteFile, "rm -f ./games/save_%dx%d.txt", plate->sizeTab, plate->sizeTab);
    system(deleteFile);
    printf("PERDU! \n");

    SDL_FreeSurface(ecran);
    for (int i = 0; i < maxTheoricTile; i++)
    {
        if (renderedFont[i] != NULL)
        {
            SDL_FreeSurface(renderedFont[i]);
        }
    }
    free(renderedFont);
    
    quitSDL();
}