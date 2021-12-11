/**
 * @file game.c
 * @author ROCHE Yannis  - yannis.roche@etu.uca.fr  - 22002168
 * @author DUPOIS Thomas - thomas.dupois@etu.uca.fr - 22001214
 * @brief Fichier de la partie logique de jeu du programme
 * @version 0.1
 * @date 2021-12-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <bits/time.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_audio.h>

#include "game.h"
#include "console.h"
#include "graphique.h"

grille * newGrid(int size) // Créer un tableau vide avec une certaine dimension 
{
    grille * plate = (grille *) malloc(sizeof(grille));
    if (plate == NULL)
        return NULL;

    plate->sizeTab = size;
    plate->score = 0;
    plate->status = IN_GAME;

    plate->gameTimer = (timer *) malloc(sizeof(timer));

    plate->tab = (unsigned long long int **) malloc(plate->sizeTab * sizeof(unsigned long long int*));
    if (plate->tab == NULL)
        return NULL;
    
    for (int i = 0; i < size; i++)
    {
        plate->tab[i] = (unsigned long long int*) malloc(plate->sizeTab * sizeof(unsigned long long int));
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
            for (int j = 0; j < plate->sizeTab; j++)
                plate->tab[i][j] = 0;
    }
    return plate;
}

void freeGrid(grille * plate) // Libère la mémoire prélevé par le tableau
{
    if (plate != NULL)
    {
        if (plate->tab != NULL)
            for (int i = 0; i < plate->sizeTab; i++)
                if (plate->tab[i] != NULL)
                    free(plate->tab[i]);

        if (plate->gameTimer != NULL)
            free(plate->gameTimer);
        free(plate->tab);
        free(plate);
    }
}

void copyGrid(grille * plateCpy, grille * plateSrc)
{
    if (plateSrc != NULL)
    {        
        if (plateSrc->tab != NULL)
            for (int i = 0; i < plateSrc->sizeTab; i++)
                for (int j = 0; j < plateSrc->sizeTab; j++)
                    plateCpy->tab[i][j] = plateSrc->tab[i][j];
        else
            return;
        if (plateSrc->gameTimer != NULL)
        {
            plateCpy->gameTimer->start_ = plateSrc->gameTimer->start_;
            plateCpy->gameTimer->loadedTime = plateSrc->gameTimer->loadedTime;
            plateCpy->gameTimer->timeElapsed = plateSrc->gameTimer->timeElapsed;
            plateCpy->gameTimer->days = plateSrc->gameTimer->days;
            plateCpy->gameTimer->hours = plateSrc->gameTimer->hours;
            plateCpy->gameTimer->minutes = plateSrc->gameTimer->minutes;
            plateCpy->gameTimer->secondes = plateSrc->gameTimer->secondes;
        }
        plateCpy->bestScore = plateSrc->bestScore;
        plateCpy->score = plateSrc->score;
        plateCpy->sizeTab = plateSrc->sizeTab;
        plateCpy->status = plateSrc->status;
    }
}

void saveGame(grille * plate)
{
    char path[30];
    sprintf(path, "./games/%dx%d_save", plate->sizeTab, plate->sizeTab);
    
    FILE * file = fopen(path, "w+");
    if (file == NULL)
    {
        fprintf(stderr, "Impossible de sauvegarder la partie\n");
        return;
    }
    if (plate->status == IN_GAME)
    {
        fprintf(file, "%d %d %d %lf\n", plate->status, plate->score, plate->bestScore, plate->gameTimer->timeElapsed);
        for (int i = 0; i < plate->sizeTab; i++)
        {
            for (int j = 0; j < plate->sizeTab; j++)
                fprintf(file, "%llu ", plate->tab[i][j]);
            fprintf(file, "\n");
        }
    }
    else
        fprintf(file, "%d %d\n", plate->status, plate->score);
    
    fclose(file);
}

grille * loadGame(int taille)
{
    char path[30];
    sprintf(path, "./games/%dx%d_save", taille, taille);
    
    grille * plate = newGrid(taille);
    if (plate == NULL)
        return NULL;

    FILE * file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Aucune sauvegarde disponible\n");
        plate->status = GAME_OVER;
        plate->bestScore = 0;
        plate->score = 0;
        return plate;
    }

    fscanf(file, "%hd ", &plate->status);
    if (plate->status == IN_GAME)
    {
        fscanf(file, "%d %d %lf\n", &plate->score, &plate->bestScore, &plate->gameTimer->loadedTime);
        for (int i = 0; i < plate->sizeTab; i++)
        {
            for (int j = 0; j < plate->sizeTab; j++)
                fscanf(file, "%llu ", &plate->tab[i][j]);
            fscanf(file, "\n");
        }
    }
    else
        fscanf(file, "%d\n", &plate->bestScore);
    
    fclose(file);
    return plate;
}

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
        for (int j = 0; j < plate->sizeTab; j++)
            if (plate->tab[i][j] == 0)
                return 1;
    return 0;
}

int gameOver(grille * plate) // Détermine si la partie est terminé
{
    for (int i = 0; i < plate->sizeTab; i++)
    {
        for (int j = 0; j < plate->sizeTab; j++)
        {
            if (i < (plate->sizeTab)-1)
                if(plate->tab[i][j] == plate->tab[i+1][j] || plate->tab[i][j] == 0 || plate->tab[i+1][j] == 0)
                    return 0;
            if (j < (plate->sizeTab)-1)
                if(plate->tab[i][j] == plate->tab[i][j+1] || plate->tab[i][j] == 0 || plate->tab[i][j+1] == 0)
                    return 0;
        }
    } 
    return 1;
}

unsigned long long int * getArrayPointer(grille * plate, int i, int j, int direction)
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
    unsigned long long int * curseur;

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

void updateTimer(timer * gameTimer)
{
    clock_t timer = (clock() - gameTimer->start_)/CLOCKS_PER_SEC + gameTimer->loadedTime;
    gameTimer->days = timer/86400;
    gameTimer->hours = timer/3600 - gameTimer->days*24;
    gameTimer->minutes = timer/60 - gameTimer->hours*60 - gameTimer->days*1440;
    gameTimer->secondes = timer - gameTimer->minutes*60 - gameTimer->hours*3600 - gameTimer->days*86400;
    gameTimer->timeElapsed = timer;
}

void consoleGameLoop(grille * plate)
{
    char c = '\0';
    int direction, nbchangement = 0, mainloop = 1;
    if (plate->status == GAME_OVER)
    {
        placeRandomNumber(plate, 2);
        plate->status = IN_GAME;
    }
    do
    {
        printGame(plate);
        c = '\0';
        if (!gameOver(plate))
            while (!(c == 'z' || c == 'Z' || c == 'h' || c == 'H' || c == 'q' || c == 'Q' || c == 'g' || c == 'G'
                || c == 's' || c == 'S' || c == 'b' || c == 'B' || c == 'd' || c == 'D' || c == ' '))
            {
                printf("> ");
                scanf("%c", &c);
                vider_buffer();
            }
        if (c == 'z' || c == 'Z' || c == 'h' || c == 'H') {direction = HAUT;}
        else if (c == 'q' || c == 'Q' || c == 'g' || c == 'G') {direction = GAUCHE;}
        else if (c == 's' || c == 'S' || c == 'b' || c == 'B') {direction = BAS;}
        else if (c == 'd' || c == 'D') {direction = DROITE;}
        else if (c == ' ') mainloop = 0;
        if (gameOver(plate))
            mainloop = 0;
        if (mainloop)
            nbchangement = updateGrid(plate, direction);
        if (checkFreeSpace(plate) && nbchangement)
            placeRandomNumber(plate, 1);
    } while (mainloop);
    if (gameOver(plate))
    {
        printf("\n=== PERDU ===\n");
        printf("[ENTRER] pour quitter\n");
        printf(">");
        scanf("%c", &c);
    }
    saveGame(plate);
    system("clear");
}

void graphiqueGameLoop(grille * plate)
{
    int mainloop = 1, menuloop = 0;
    int nbchangement = 1, event = -10, canRewind = 0, menuSelect = 0;
    int maxTheoricTile = (plate->sizeTab * plate->sizeTab)+1;
    grille * prevMove = newGrid(plate->sizeTab);
    gameTextures * gameAsset = initGraphicAssets(maxTheoricTile);
    plate->gameTimer->start_ = clock();
    if ((gameAsset->ecran = initSDL(gameAsset->ecran)) == NULL)
    {
        fprintf(stderr, "Impossible de lancer une interface graphique\n");
        exit(EXIT_FAILURE);
    }

    if (plate->status == GAME_OVER)
    {
        placeRandomNumber(plate, 2);
        plate->status = IN_GAME;
    }
    do
    {
        if (!gameOver(plate))
            updateTimer(plate->gameTimer);
        displayGrid(plate, gameAsset);
        event = eventSDL();

        if (event == EXIT)
            mainloop = 0;
        if (event != -10 && event != EXIT && event != UNDO && event != MENU && !gameOver(plate))
        {
            copyGrid(prevMove, plate);
            canRewind = 1;
            nbchangement = updateGrid(plate, event);
            if (checkFreeSpace(plate) && nbchangement)
                placeRandomNumber(plate, 1);
        }
        if (event == UNDO && canRewind)
        {
            copyGrid(plate, prevMove);
            canRewind = 0;
        }
        
        if (event == MENU){menuloop = 1; menuSelect = 0;}
        while (menuloop)
        {
            displayMenu(gameAsset, menuSelect);
            event = eventSDL();
            if (event == EXIT)
            {
                mainloop = 0;
                menuloop = 0;
            }
            else if (event == MENU)
            {
                menuloop = 0;
                event = -10;
            }
            else if (event == BAS)
                menuSelect = (menuSelect == 0)?1:(menuSelect == 1)?2:0;
            else if (event == HAUT)
                menuSelect = (menuSelect == 0)?2:(menuSelect == 1)?0:1;
            else if (event == VALIDE)
            {
                if (menuSelect == 0)
                    menuloop = 0;
                else if (menuSelect == 1)
                {
                    int taille = plate->sizeTab;
                    int bestScore = plate->bestScore;
                    freeGrid(plate);
                    plate = newGrid(taille);
                    placeRandomNumber(plate, 2);
                    plate->bestScore = bestScore;
                    plate->gameTimer->timeElapsed = 0;
                    plate->gameTimer->start_ = clock();
                    canRewind = 0;
                    menuloop = 0;
                }
                else if (menuSelect == 2)
                {
                    mainloop = 0;
                    menuloop = 0;
                }
            }
            
        }
        if (gameOver(plate))
            plate->status = GAME_OVER;
        
        if (plate->score >= plate->bestScore)
            plate->bestScore = plate->score;
        
    } while (mainloop);
    freeGraphics(gameAsset);
    free(prevMove->gameTimer);
    saveGame(plate);
}