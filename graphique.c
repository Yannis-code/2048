#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_audio.h>

#include "constantes.h"
#include "graphique.h"

SDL_Surface * initSDL(SDL_Surface * ecran)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == -1) // Démarrage de la SDL. Si erreur :
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

    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        return NULL; // On retourne erreur
    }
    
    return ecran;
}

int eventSDL(void)
{
    SDL_Event event;
    SDL_WaitEvent(&event);
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

SDL_Surface * getFont(int score)
{
    TTF_Font *police = NULL;
    police = TTF_OpenFont("angelina.ttf", 65);
    SDL_Color couleurNoire = {0, 0, 0};

    SDL_Surface * texte = TTF_RenderText_Blended(police, "Salut les Zér0s !", couleurNoire);

    //TTF_CloseFont(police); /* Doit être avant TTF_Quit() */
    return texte;
}

int * getColorRGB(int value)
{
    int * RGB = (int *) malloc(3*sizeof(int));
    if (value == 0)
    {
        RGB[0] = 214; RGB[1] = 210; RGB[2] = 210; //214, 210, 210
    }
    else if (value == 2)
    {
        RGB[0] = 214; RGB[1] = 100; RGB[2] = 100; //214, 210, 210
    }
    else if (value == 4)
    {
        RGB[0] = 214; RGB[1] = 75; RGB[2] = 75; //214, 210, 210
    }
    else if (value == 8)
    {
        RGB[0] = 214; RGB[1] = 50; RGB[2] = 50; //214, 210, 210
    }
    else if (value == 16)
    {
        RGB[0] = 214; RGB[1] = 25; RGB[2] = 25; //214, 210, 210
    }
    else
    {
        RGB[0] = 214; RGB[1] = 0; RGB[2] = 0; //214, 210, 210
    }
    return RGB;
}

void displayGrid(SDL_Surface * ecran, grille* plate, int windowWidth, int windowHeight)
{
    TTF_Font *police = NULL;
    police = TTF_OpenFont("angelina.ttf", 40);
    SDL_Color couleurNoire = {0, 0, 0};

    float a =0.1;
    int intervale = (9* windowWidth/100)/(a * plate->sizeTab + plate->sizeTab + a);
    int sizeImage = intervale * 10;
    int sizeGrid = sizeImage * (a*plate->sizeTab+plate->sizeTab+a);
    
    SDL_Surface * grid = SDL_CreateRGBSurface(SDL_HWSURFACE, sizeGrid, sizeGrid, 32, 0, 0, 0, 0);
    SDL_Surface * tile = SDL_CreateRGBSurface(SDL_HWSURFACE, sizeImage, sizeImage, 32, 0, 0, 0, 0);
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 100, 100, 100));
    SDL_FillRect(grid, NULL, SDL_MapRGB(ecran->format, 193, 184, 184));;
    SDL_Rect posgrid, posImage, posTxt;

    posgrid.x = windowWidth/2 - sizeGrid/2;
    posgrid.y = windowHeight-sizeGrid - 0.05 * windowHeight;
    SDL_BlitSurface(grid, NULL, ecran, &posgrid);
    
    for (int i = 0; i < plate->sizeTab; i++)
    {
        for (int j = 0; j < plate->sizeTab; j++)
        {
            posImage.x = posgrid.x + (j+1) * intervale + (j * sizeImage);
            posImage.y = posgrid.y + (i+1) * intervale + (i * sizeImage);
            int * color = getColorRGB(plate->tab[i][j]);
            SDL_FillRect(tile, NULL, SDL_MapRGB(ecran->format, color[0], color[1], color[2]));
            SDL_BlitSurface(tile, NULL, ecran, &posImage);
            if (police != NULL)
            {
                posTxt.x =  posImage.x;
                posTxt.y = posImage.y;
                char txt[15] = "";
                sprintf(txt, "%d", plate->tab[i][j]);
                SDL_Surface * texte = TTF_RenderText_Blended(police, txt, couleurNoire);
                SDL_BlitSurface(texte, NULL, ecran, &posImage);
            }
            
            // Print TTF
        } 
    }
    SDL_Flip(ecran);
}

int quitSDL(void)
{
    // Libération des surfaces etc
    SDL_Quit();
    return EXIT_SUCCESS;
}

#include "constantes.h"
#include "graphique.h"