/**
 * @file graphique.c
 * @author ROCHE Yannis  - yannis.roche@etu.uca.fr  - 22002168
 * @author DUPOIS Thomas - thomas.dupois@etu.uca.fr - 22001214
 * @brief Fichier de la partie affichage graphique du programme
 * @version 0.1
 * @date 2021-12-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_audio.h>
#include <SDL/SDL_rotozoom.h>

#include "constantes.h"
#include "graphique.h"

SDL_Surface * initSDL(SDL_Surface * ecran)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) // Démarrage de la SDL. Si erreur :
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
        return NULL; // On retourne erreur
    }

    SDL_WM_SetIcon(SDL_LoadBMP("./img/logo.bmp"), NULL);
    SDL_WM_SetCaption("2048", NULL);

    if ((ecran = SDL_SetVideoMode(WIN_WIDTH, WIN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) // Démarrage de la SDL. Si erreur :
    {
        fprintf(stderr, "Erreur d'initialisation de la fenetre : %s\n", SDL_GetError()); // Écriture de l'erreur
        return NULL; // On retourne erreur
    }

    if(TTF_Init() != 0)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        return NULL; // On retourne erreur
    }
    
    return ecran;
}

gameTextures * initGraphicAssets(int maxTheoricTile)
{
    gameTextures * gameAsset = (gameTextures *) malloc(sizeof(gameTextures));
    gameAsset->grid = (rect *) malloc(sizeof(rect));
    gameAsset->grid->surface = NULL;
    gameAsset->tile = (rect *) malloc(sizeof(rect));
    gameAsset->tile->surface = NULL;
    gameAsset->menu = (rect *) malloc(sizeof(rect));
    gameAsset->menu->surface = NULL;
    gameAsset->undo = (rect *) malloc(sizeof(rect));
    gameAsset->undo->surface = NULL;
    gameAsset->font = (font *) malloc(sizeof(font));
    gameAsset->font->font = NULL;
    gameAsset->tilesRendered = (SDL_Surface **) malloc(maxTheoricTile * sizeof(SDL_Surface *));
    for (int i = 0; i < maxTheoricTile; i++)
        gameAsset->tilesRendered[i] = NULL;
    gameAsset->nbTilesRendered = maxTheoricTile;
    
    return gameAsset;
}

void freeRectStruct(rect * rectToFree)
{
    if (rectToFree != NULL)
    {
        if (rectToFree->surface != NULL)
        {
            SDL_FreeSurface(rectToFree->surface);
            rectToFree->surface = NULL;
        }
        free(rectToFree);
        rectToFree = NULL;
    }
}

void freeGraphics(gameTextures * gameAsset)
{
    if (gameAsset != NULL)
    {
        if (gameAsset->tilesRendered != NULL)
        {
            for (int i = 0; i < gameAsset->nbTilesRendered; i++)
                if (gameAsset->tilesRendered[i] != NULL)
                {
                    SDL_FreeSurface(gameAsset->tilesRendered[i]);
                    gameAsset->tilesRendered[i] = NULL;
                }
            gameAsset->tilesRendered = NULL;
        }
        
        if (gameAsset->font != NULL)
        {
            if (gameAsset->font->font != NULL)
            {
                TTF_CloseFont(gameAsset->font->font);
                gameAsset->font->font = NULL;
            }
            free(gameAsset->font);
            gameAsset->font = NULL;
        }

        freeRectStruct(gameAsset->grid);
        freeRectStruct(gameAsset->tile);
        freeRectStruct(gameAsset->menu);
        freeRectStruct(gameAsset->undo);
        
        if (gameAsset->ecran != NULL)
        {
            SDL_FreeSurface(gameAsset->ecran);
            gameAsset->ecran = NULL;
        }
        free(gameAsset);
    }
    TTF_Quit();
    SDL_Quit();
}

int eventSDL(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT:
            return EXIT; // Click sur fermer la fenetre
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                case SDLK_ESCAPE: // Affichage du menu
                    return MENU;
                    break;
                case SDLK_UP: // Fleche du haut
                    return HAUT;
                    break;
                case SDLK_DOWN: // Fleche du bas
                    return BAS;
                    break;
                case SDLK_RIGHT: // Fleche de droite
                    return DROITE;
                    break;
                case SDLK_LEFT: // Fleche de gauche
                    return GAUCHE;
                    break;
                case SDLK_BACKSPACE: // Touche supprimer
                    return UNDO;
                    break;
                case SDLK_RETURN: // Touche entrée
                    return VALIDE;
                    break;
                default: // Défaut renvoie un valeur négative non interprété
                    return -10;
                    break;
            }
            break;
        default: // Défaut renvoie un valeur négative non interprété
            return -10;
            break;
    }
}

float getFontScale(int strLen, int charWidth, int charHeight, int maxWidth, int maxHeight)
{
    float diagoFont = sqrtf(pow(strLen * charWidth, 2) + powf(charHeight, 2));
    float diagoOut = sqrtf(powf(maxWidth, 2) + powf(maxHeight, 2));
    return diagoOut / diagoFont; 
}

SDL_Surface * getFont(TTF_Font * font, char * str, int charWidth, int charHeight, int maxWidth, int maxHeight, int colorR, int colorG, int colorB)
{
    SDL_Surface * finalSurface;
    SDL_Color color = {colorR, colorG, colorB};

    float zoom = getFontScale(strlen(str), charWidth, charHeight, maxWidth, maxHeight);
    
    SDL_Surface * texte = TTF_RenderText_Blended(font, str, color);
    
    finalSurface = rotozoomSurface(texte, 0.0, zoom, 1);

    SDL_FreeSurface(texte);
    
    return finalSurface;
}

int * getColorRGB(int value)
{
    int * RGB = (int *) malloc(3*sizeof(int));
    if (value == 0) {RGB[0] = 205; RGB[1] = 193; RGB[2] = 180;}
    else {
        switch (value) 
        {
        case 2:   RGB[0] = 238; RGB[1] = 228; RGB[2] = 218; break;
        case 4:   RGB[0] = 238; RGB[1] = 225; RGB[2] = 201; break;
        case 8:   RGB[0] = 243; RGB[1] = 178; RGB[2] = 122; break;
        case 16:  RGB[0] = 246; RGB[1] = 150; RGB[2] = 100; break;
        case 32:  RGB[0] = 247; RGB[1] = 124; RGB[2] = 95;  break;
        case 64:  RGB[0] = 247; RGB[1] = 95;  RGB[2] = 59;  break;
        case 128: RGB[0] = 237; RGB[1] = 208; RGB[2] = 115; break;
        case 256: RGB[0] = 234; RGB[1] = 178; RGB[2] = 24;  break;
        default:
            RGB[0] = ((RGB[0] = ((value*70)%255)) < 100) ? RGB[0] + 155 : RGB[0]; 
            RGB[1] = ((RGB[1] = ((value*20)%255)) < 100) ? RGB[1] + 155 : RGB[1]; 
            RGB[2] = ((RGB[2] = ((value*30)%255)) < 100) ? RGB[2] + 155 : RGB[2]; 
            break;
        }
    }
    return RGB;
}

void displayGrid(grille* plate, gameTextures * gameAsset)
{
    char textToDisplay[50] = "";
    //Calcul des dimension des zones d'affichage
    float a =0.1;
    int intervale = (9* WIN_WIDTH/100)/(a * plate->sizeTab + plate->sizeTab + a);
    int sizeImage = intervale * 10;
    int sizeGrid = sizeImage * (a*plate->sizeTab+plate->sizeTab+a);

    SDL_FillRect(gameAsset->ecran, NULL, SDL_MapRGB(gameAsset->ecran->format, 250, 248, 239));
    
    SDL_Rect posTxt;

    // Initialisation de l'affichage de la touche echap
    if (gameAsset->menu->surface == NULL)
    {
        SDL_Surface * load_escape = IMG_Load("./img/escape.png");
        gameAsset->menu->surface = rotozoomSurface(load_escape, 0, (0.05*WIN_HEIGHT)/load_escape->h, 1);
        SDL_FreeSurface(load_escape);
        gameAsset->menu->box.x = 0.01*WIN_WIDTH;
        gameAsset->menu->box.y = 0.01*WIN_WIDTH;
    }
    SDL_BlitSurface(gameAsset->menu->surface, NULL, gameAsset->ecran, &gameAsset->menu->box);

    // Initialisation de l'affichage de la touche return
    if (gameAsset->undo->surface == NULL)
    {
        SDL_Surface * load_undo = IMG_Load("./img/return.png");
        gameAsset->undo->surface = rotozoomSurface(load_undo, 0, (0.05*WIN_HEIGHT)/load_undo->h, 1);
        SDL_FreeSurface(load_undo);
        gameAsset->undo->box.x = 0.99*WIN_WIDTH - gameAsset->undo->surface->w;
        gameAsset->undo->box.y = 0.01*WIN_WIDTH;
    }
    SDL_BlitSurface(gameAsset->undo->surface, NULL, gameAsset->ecran, &gameAsset->undo->box);
    
    // Initialisation de la grille si elle n'existe pas déjà
    if (gameAsset->grid->surface == NULL)
    {
        gameAsset->grid->surface = SDL_CreateRGBSurface(SDL_HWSURFACE, sizeGrid, sizeGrid, 32, 0, 0, 0, 0);
        gameAsset->grid->box.x = (WIN_WIDTH - sizeGrid)/2;
        gameAsset->grid->box.y = WIN_HEIGHT-sizeGrid - 0.05 * WIN_HEIGHT;
    }
    // Initialisation de la case si elle n'existe pas déjà
    if (gameAsset->tile->surface == NULL)
    {
        gameAsset->tile->surface = SDL_CreateRGBSurface(SDL_HWSURFACE, sizeImage, sizeImage, 32, 0, 0, 0, 0);
    }

    // Initialisation de la police si elle n'existe pas déjà
    if (gameAsset->font->font == NULL)
    {
        gameAsset->font->font = TTF_OpenFont(FONT_PATH, FONT_SIZE_LOAD);
        TTF_SizeText(gameAsset->font->font, "0", &(gameAsset->font->charWidth), &(gameAsset->font->charHeight));
    }
    
    sprintf(textToDisplay, "Menu");
    SDL_Surface * text = getFont(gameAsset->font->font, textToDisplay, gameAsset->font->charWidth,
        gameAsset->font->charHeight, gameAsset->menu->surface->w, gameAsset->menu->surface->h, 0, 0, 0);
    posTxt.x = gameAsset->menu->box.x + gameAsset->menu->surface->w + 0.01*WIN_WIDTH;
    posTxt.y = gameAsset->menu->box.y + (gameAsset->menu->surface->h - text->h)/2;
    SDL_BlitSurface(text, NULL, gameAsset->ecran, &posTxt);
    SDL_FreeSurface(text);

    sprintf(textToDisplay, "Annuler");
    text = getFont(gameAsset->font->font, textToDisplay, gameAsset->font->charWidth,
        gameAsset->font->charHeight, gameAsset->undo->surface->w, gameAsset->undo->surface->h, 0, 0, 0);
    posTxt.x = gameAsset->undo->box.x - gameAsset->undo->surface->w - 0.01*WIN_WIDTH;
    posTxt.y = gameAsset->undo->box.y + (gameAsset->undo->surface->h - text->h)/2;
    SDL_BlitSurface(text, NULL, gameAsset->ecran, &posTxt);
    SDL_FreeSurface(text);

    sprintf(textToDisplay, "Score: %d", plate->score);
    text = getFont(gameAsset->font->font, textToDisplay, gameAsset->font->charWidth,
        gameAsset->font->charHeight, 0.4 * WIN_WIDTH, 0.3*WIN_HEIGHT, 0, 0, 0);
    posTxt.x = WIN_WIDTH/2 - text->w/2;
    posTxt.y = gameAsset->grid->box.y - gameAsset->grid->box.y/2 - text->h;
    SDL_BlitSurface(text, NULL, gameAsset->ecran, &posTxt);
    SDL_FreeSurface(text);
    
    sprintf(textToDisplay, "Meilleur score: %d", plate->bestScore);
    text = getFont(gameAsset->font->font, textToDisplay, gameAsset->font->charWidth,
        gameAsset->font->charHeight, 0.4 * WIN_WIDTH, 0.3*WIN_HEIGHT, 100, 100, 100);
    posTxt.x = WIN_WIDTH/2 - text->w/2;
    posTxt.y = gameAsset->grid->box.y - gameAsset->grid->box.y/2;
    SDL_BlitSurface(text, NULL, gameAsset->ecran, &posTxt);
    SDL_FreeSurface(text);

    sprintf(textToDisplay, "Temps: %d:%.2d:%.2d:%.2d", (int) plate->gameTimer->days, (int) plate->gameTimer->hours, (int) plate->gameTimer->minutes, (int) plate->gameTimer->secondes);
    text = getFont(gameAsset->font->font, textToDisplay, gameAsset->font->charWidth,
        gameAsset->font->charHeight, gameAsset->grid->box.w/2, 0.15*WIN_HEIGHT, 75, 75, 75);
    posTxt.x = gameAsset->grid->box.x;
    posTxt.y = WIN_HEIGHT-sizeGrid - 0.05 * WIN_HEIGHT - text->h;
    SDL_BlitSurface(text, NULL, gameAsset->ecran, &posTxt);
    SDL_FreeSurface(text);
    
    SDL_FillRect(gameAsset->grid->surface, NULL, SDL_MapRGB(gameAsset->ecran->format, 187, 173, 160));
    SDL_BlitSurface(gameAsset->grid->surface, NULL, gameAsset->ecran, &(gameAsset->grid->box));
    
    for (int i = 0; i < plate->sizeTab; i++)
    {
        for (int j = 0; j < plate->sizeTab; j++)
        {
            gameAsset->tile->box.x = gameAsset->grid->box.x + (j+1) * intervale + (j * sizeImage);
            gameAsset->tile->box.y = gameAsset->grid->box.y + (i+1) * intervale + (i * sizeImage);
            int * color = getColorRGB(plate->tab[i][j]);
            SDL_FillRect(gameAsset->tile->surface, NULL, SDL_MapRGB(gameAsset->ecran->format, color[0], color[1], color[2]));
            free(color); color = NULL;
            SDL_BlitSurface(gameAsset->tile->surface, NULL, gameAsset->ecran, &(gameAsset->tile->box));
            
            if (plate->tab[i][j])
            {
                sprintf(textToDisplay, "%s", "");
                unsigned long long int value = plate->tab[i][j];

                if (value >= 50000 && value < 1000000)
                    sprintf(textToDisplay, "%lluk", value/1000);
                else if (value >= 1000000 && value < 1000000000)
                    sprintf(textToDisplay, "%lluM", value/1000000);
                else if (value >= 1000000000)
                    sprintf(textToDisplay, "%lluMd", value/1000000000);
                else
                    sprintf(textToDisplay, "%llu", value);

                
                int indice = (int) abs(log2(value)-1);
                if (gameAsset->tilesRendered[indice] == NULL)
                {
                    if (value == 2 || value == 4)
                        gameAsset->tilesRendered[indice] = getFont(gameAsset->font->font, textToDisplay,
                        gameAsset->font->charWidth, gameAsset->font->charHeight, 0.8 * sizeImage, 0.8 * sizeImage, 119, 110, 101);
                    else
                        gameAsset->tilesRendered[indice] = getFont(gameAsset->font->font, textToDisplay,
                        gameAsset->font->charWidth, gameAsset->font->charHeight, 0.8 * sizeImage, 0.8 * sizeImage, 249, 246, 242);
                }
                
                SDL_Surface * texte = gameAsset->tilesRendered[indice];

                posTxt.x = gameAsset->tile->box.x + sizeImage/2 - texte->w/2;
                posTxt.y = gameAsset->tile->box.y + sizeImage/2 - texte->h/2;
                
                SDL_BlitSurface(texte, NULL, gameAsset->ecran, &posTxt);
            }
        }
    }
    
    if (plate->status == GAME_OVER)
    {
        SDL_Surface * gameOver = SDL_CreateRGBSurface(SDL_HWSURFACE, sizeGrid, sizeGrid, 32, 0, 0, 0, 0);
        SDL_FillRect(gameOver, NULL, SDL_MapRGB(gameAsset->ecran->format, 247, 95, 59));
        SDL_SetAlpha(gameOver, SDL_SRCALPHA | SDL_RLEACCEL, 100);

        SDL_BlitSurface(gameOver, NULL, gameAsset->ecran, &gameAsset->grid->box);
        SDL_FreeSurface(gameOver);

        sprintf(textToDisplay, "PERDU");
        text = getFont(gameAsset->font->font, textToDisplay, gameAsset->font->charWidth,
            gameAsset->font->charHeight, 0.4 * WIN_WIDTH, 0.3*WIN_HEIGHT, 255, 255, 255);
        posTxt.x = gameAsset->grid->box.x + (sizeGrid - text->w)/2;
        posTxt.y = gameAsset->grid->box.y + (sizeGrid - text->h)/2;
        SDL_BlitSurface(text, NULL, gameAsset->ecran, &posTxt);
        SDL_FreeSurface(text);
    }

    SDL_Flip(gameAsset->ecran);
}

void displayMenu(gameTextures * gameAsset, int select)
{
    char textToDisplay[50] = "";
    SDL_Rect posTxt;

    SDL_FillRect(gameAsset->ecran, NULL, SDL_MapRGB(gameAsset->ecran->format, 250, 248, 239));
    
    int colorText1[3] = {100, 100, 100};
    int colorText2[3] = {100, 100, 100};
    int colorText3[3] = {100, 100, 100};
    if (select == 0) {colorText1[0] = 0; colorText1[1] = 150; colorText1[2] = 0;}
    if (select == 1) {colorText2[0] = 0; colorText2[1] = 0; colorText2[2] = 150;}
    if (select == 2) {colorText3[0] = 150; colorText3[1] = 0; colorText3[2] = 0;}
    

    sprintf(textToDisplay, "Reprendre");
    SDL_Surface * text = getFont(gameAsset->font->font, textToDisplay, gameAsset->font->charWidth,
        gameAsset->font->charHeight, 0.4 * WIN_WIDTH, 0.3*WIN_HEIGHT, colorText1[0], colorText1[1], colorText1[2]);
    posTxt.x = WIN_WIDTH/2 - text->w/2;
    posTxt.y = WIN_HEIGHT/4 - text->h/2;
    SDL_BlitSurface(text, NULL, gameAsset->ecran, &posTxt);
    SDL_FreeSurface(text);
    
    sprintf(textToDisplay, "Nouvelle partie");
    text = getFont(gameAsset->font->font, textToDisplay, gameAsset->font->charWidth,
        gameAsset->font->charHeight, 0.4 * WIN_WIDTH, 0.3*WIN_HEIGHT, colorText2[0], colorText2[1], colorText2[2]);
    posTxt.x = WIN_WIDTH/2 - text->w/2;
    posTxt.y = 2*WIN_HEIGHT/4 - text->h/2;
    SDL_BlitSurface(text, NULL, gameAsset->ecran, &posTxt);
    SDL_FreeSurface(text);

    sprintf(textToDisplay, "Quitter");
    text = getFont(gameAsset->font->font, textToDisplay, gameAsset->font->charWidth,
        gameAsset->font->charHeight, 0.4 * WIN_WIDTH, 0.3*WIN_HEIGHT, colorText3[0], colorText3[1], colorText3[2]);
    posTxt.x = WIN_WIDTH/2 - text->w/2;
    posTxt.y = 3*WIN_HEIGHT/4 - text->h/2;
    SDL_BlitSurface(text, NULL, gameAsset->ecran, &posTxt);
    SDL_FreeSurface(text);

    SDL_Flip(gameAsset->ecran);
}