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

    SDL_WM_SetCaption("2048", NULL);

    if ((ecran = SDL_SetVideoMode(800, 1000, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) // Démarrage de la SDL. Si erreur :
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

int eventSDL(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT:
            return -1;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                case SDLK_ESCAPE: // Veut arrêter le jeu
                    return -1;
                    break;
                case SDLK_UP: // Demande à jouer
                    return HAUT;
                    break;
                case SDLK_DOWN: // Demande à jouer
                    return BAS;
                    break;
                case SDLK_RIGHT: // Demande à jouer
                    return DROITE;
                    break;
                case SDLK_LEFT: // Demande à jouer
                    return GAUCHE;
                    break;
                default: // Demande l'éditeur de niveaux
                    return -10;
                    break;
            }
            break;
        default:
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

int getPoxer2(int valeur)
{
    int power = 1;
    int test = 2;
    while ( (test *= 2) <= valeur)
        power++;
    return power;
}

gameTextures * initGraphicAssets(int maxTheoricTile)
{
    gameTextures * gameAsset = (gameTextures *) malloc(sizeof(gameTextures));
    gameAsset->grid = (rect *) malloc(sizeof(rect));
    gameAsset->tile = (rect *) malloc(sizeof(rect));
    gameAsset->font = (font *) malloc(sizeof(font));
    gameAsset->tilesRendered = (SDL_Surface **) malloc(maxTheoricTile * sizeof(SDL_Surface *));
    return gameAsset;
}

void displayGrid(grille* plate, gameTextures * gameAsset, timer * gameTimer, int windowWidth, int windowHeight)
{
    char textToDisplay[50] = "";
    //Calcul des dimension des zones d'affichage
    float a =0.1;
    int intervale = (9* windowWidth/100)/(a * plate->sizeTab + plate->sizeTab + a);
    int sizeImage = intervale * 10;
    int sizeGrid = sizeImage * (a*plate->sizeTab+plate->sizeTab+a);

    SDL_FillRect(gameAsset->ecran, NULL, SDL_MapRGB(gameAsset->ecran->format, 250, 248, 239));
    
    SDL_Rect posTxt;

    // Initialisation de la grille si elle n'existe pas déjà
    if (gameAsset->grid->surface == NULL)
    {
        gameAsset->grid->surface = SDL_CreateRGBSurface(SDL_HWSURFACE, sizeGrid, sizeGrid, 32, 0, 0, 0, 0);
        gameAsset->grid->box.x = (windowWidth - sizeGrid)/2;
        gameAsset->grid->box.y = windowHeight-sizeGrid - 0.05 * windowHeight;
    }
    // Initialisation de la case si elle n'existe pas déjà
    if (gameAsset->tile->surface == NULL)
    {
        gameAsset->tile->surface = SDL_CreateRGBSurface(SDL_HWSURFACE, sizeImage, sizeImage, 32, 0, 0, 0, 0);
    }
    
    // Initialisation de la police si elle n'existe pas déjà
    if (gameAsset->font->font == NULL)
    {
        gameAsset->font->font = TTF_OpenFont("./roboto-mono/RobotoMono-Medium.ttf", 150);
        TTF_SizeText(gameAsset->font->font, "0", &(gameAsset->font->charWidth), &(gameAsset->font->charHeight));
    }
    

    sprintf(textToDisplay, "Score: %d", plate->score);
    SDL_Surface * text = getFont(gameAsset->font->font, textToDisplay, gameAsset->font->charWidth,
        gameAsset->font->charHeight, 0.4 * windowWidth, 0.3*windowHeight, 0, 0, 0);
    posTxt.x = windowWidth/2 - text->w/2;
    posTxt.y = 0.05 * windowHeight;
    SDL_BlitSurface(text, NULL, gameAsset->ecran, &posTxt);
    SDL_FreeSurface(text);
    
    sprintf(textToDisplay, "Meilleur score: %d", plate->bestScore);
    text = getFont(gameAsset->font->font, textToDisplay, gameAsset->font->charWidth,
        gameAsset->font->charHeight, 0.3 * windowWidth, 0.2*windowHeight, 0, 0, 0);
    posTxt.x = (windowWidth - sizeGrid)/2;
    posTxt.y = windowHeight-sizeGrid - 0.05 * windowHeight - text->h;
    SDL_BlitSurface(text, NULL, gameAsset->ecran, &posTxt);
    SDL_FreeSurface(text);

    sprintf(textToDisplay, "%d:%.2d:%.2d:%.2d", (int) gameTimer->days, (int) gameTimer->hours, (int) gameTimer->minutes, (int) gameTimer->secondes);
    text = getFont(gameAsset->font->font, textToDisplay, gameAsset->font->charWidth,
        gameAsset->font->charHeight, 0.2 * windowWidth, 0.15*windowHeight, 0, 0, 0);
    posTxt.x = (windowWidth + sizeGrid)/2 - text->w;
    posTxt.y = windowHeight-sizeGrid - 0.05 * windowHeight - text->h;
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
                sprintf(textToDisplay, "%d", plate->tab[i][j]);
                
                int indice = getPoxer2(plate->tab[i][j])-1;
                if (gameAsset->tilesRendered[indice] == NULL)
                    gameAsset->tilesRendered[indice] = getFont(gameAsset->font->font, textToDisplay,
                    gameAsset->font->charWidth, gameAsset->font->charHeight, 0.8 * sizeImage, 0.8 * sizeImage, 0, 0, 0);
                
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

        sprintf(textToDisplay, "PERDU");
        text = getFont(gameAsset->font->font, textToDisplay, gameAsset->font->charWidth,
            gameAsset->font->charHeight, 0.4 * windowWidth, 0.3*windowHeight, 255, 255, 255);
        posTxt.x = gameAsset->grid->box.x + (sizeGrid - text->w)/2;
        posTxt.y = gameAsset->grid->box.y + (sizeGrid - text->h)/2;
        SDL_BlitSurface(text, NULL, gameAsset->ecran, &posTxt);
        SDL_FreeSurface(text);
    }

    SDL_Flip(gameAsset->ecran);
}

void freeGameTextures(grille* plate, gameTextures * gameAsset)
{
    for (int i = 0; i < (plate->sizeTab * plate->sizeTab)+1; i++)
        if (gameAsset->tilesRendered[i] != NULL)
            SDL_FreeSurface(gameAsset->tilesRendered[i]);

    if (gameAsset->font->font != NULL)
        TTF_CloseFont(gameAsset->font->font);
    free(gameAsset->font);

    if (gameAsset->grid->surface != NULL)
        SDL_FreeSurface(gameAsset->grid->surface);
    free(gameAsset->grid);

    if (gameAsset->tile->surface != NULL)
        SDL_FreeSurface(gameAsset->tile->surface);
    free(gameAsset->tile);
    SDL_FreeSurface(gameAsset->ecran);
    TTF_Quit();
    SDL_Quit();
}