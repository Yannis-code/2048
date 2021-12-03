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

    if ((ecran = SDL_SetVideoMode(800, 1000, 32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF)) == NULL) // Démarrage de la SDL. Si erreur :
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

SDL_Surface * getFont(TTF_Font * font, char * str, int charWidth, int charHeight, int maxWidth, int maxHeight)
{
    SDL_Color couleurNoire = {0, 0, 0};
    SDL_Surface * finalSurface;

    float zoom = getFontScale(strlen(str), charWidth, charHeight, maxWidth, maxHeight);
    
    SDL_Surface * texte = TTF_RenderText_Blended(font, str, couleurNoire);
    
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

void displayGrid(SDL_Surface * ecran, grille* plate, SDL_Surface ** renderedFont, int windowWidth, int windowHeight)
{
    float a =0.1;
    int intervale = (9* windowWidth/100)/(a * plate->sizeTab + plate->sizeTab + a);
    int sizeImage = intervale * 10;
    int sizeGrid = sizeImage * (a*plate->sizeTab+plate->sizeTab+a);

    SDL_Rect posgrid, posImage, posTxt;
    
    int charWidth, charHeight;
    TTF_Font * police = TTF_OpenFont("./roboto-mono/RobotoMono-Medium.ttf", 150);
    TTF_SizeText(police, "0", &charWidth, &charHeight);

    SDL_Surface * grid = SDL_CreateRGBSurface(SDL_HWSURFACE, sizeGrid, sizeGrid, 32, 0, 0, 0, 0);
    SDL_Surface * tile = SDL_CreateRGBSurface(SDL_HWSURFACE, sizeImage, sizeImage, 32, 0, 0, 0, 0);
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 250, 248, 239));
    SDL_FillRect(grid, NULL, SDL_MapRGB(ecran->format, 187, 173, 160));

    char renderText[30] = "";
    sprintf(renderText, "Score: %d", plate->score);
    SDL_Surface * score = getFont(police, renderText, charWidth, charHeight, 0.4 * windowWidth, 0.3*windowHeight);
    posTxt.x = windowWidth/2 - score->w/2;
    posTxt.y = 0.05 * windowHeight;
    SDL_BlitSurface(score, NULL, ecran, &posTxt);
    SDL_FreeSurface(score);

    posgrid.x = (windowWidth - sizeGrid)/2;
    posgrid.y = windowHeight-sizeGrid - 0.05 * windowHeight;
    SDL_BlitSurface(grid, NULL, ecran, &posgrid);
    SDL_FreeSurface(grid);
    
    for (int i = 0; i < plate->sizeTab; i++)
    {
        for (int j = 0; j < plate->sizeTab; j++)
        {
            posImage.x = posgrid.x + (j+1) * intervale + (j * sizeImage);
            posImage.y = posgrid.y + (i+1) * intervale + (i * sizeImage);
            int * color = getColorRGB(plate->tab[i][j]);
            SDL_FillRect(tile, NULL, SDL_MapRGB(ecran->format, color[0], color[1], color[2]));
            free(color); color = NULL;
            SDL_BlitSurface(tile, NULL, ecran, &posImage);
            
            if (plate->tab[i][j])
            {
                char renderText[20] = "";
                sprintf(renderText, "%d", plate->tab[i][j]);
                
                int powerTile = getPoxer2(plate->tab[i][j]);
                if (renderedFont[powerTile] == NULL)
                    renderedFont[powerTile] = getFont(police, renderText, charWidth, charHeight, 0.8 * sizeImage, 0.8 * sizeImage);
                
                SDL_Surface * texte = renderedFont[powerTile];
            
                posTxt.x = posImage.x + sizeImage/2 - texte->w/2;
                posTxt.y = posImage.y + sizeImage/2 - texte->h/2;
                
                SDL_BlitSurface(texte, NULL, ecran, &posTxt);
            }
        } 
    }
    SDL_Flip(ecran);
    TTF_CloseFont(police);
    SDL_FreeSurface(tile);
}

int quitSDL(void)
{
    // Libération des surfaces etc
    SDL_Quit();
    return EXIT_SUCCESS;
}