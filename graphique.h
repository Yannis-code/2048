/**
 * @file graphique.h
 * @author ROCHE Yannis  - yannis.roche@etu.uca.fr  - 22002168
 * @author DUPOIS Thomas - thomas.dupois@etu.uca.fr - 22001214
 * @brief Fichier header de la partie affichage graphique du programme
 * @version 0.1
 * @date 2021-12-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef GRAPHIQUE
#define GRAPHIQUE

#include "constantes.h"

/**
 * @brief Initialise les librairies SDL utilisées et renvoie un pointeur sur la surface ecran principale
 * 
 * @param ecran Un pointuer sur la SDL_Surface ecran a utiliser comme fenetre
 * @return SDL_Surface* Le même pointeur passé en paramètre
 */
SDL_Surface * initSDL(SDL_Surface * ecran);

/**
 * @brief Initialise la structure "struct _gameTextures" contenant les surfaces en cache a afficher à l'écran
 * 
 * @param maxTheoricTile La puissance théorique maximale d'une case pouvant être atteinte en fonctiond de la taille de la grille
 * @return gameTextures* Un pointeur sur la stucture "struct _gameTextures" créé
 */
gameTextures * initGraphicAssets(int maxTheoricTile);

/**
 * @brief Libère la stucture contenant tous les éléments graphiques stockés en cache
 * 
 * @param gameAsset Un pointuer sur la stucture "struct _gameTextures" à libérer
 */
void freeGraphics(gameTextures * gameAsset);

/**
 * @brief Récupère les evenement clavier de l'utilisateur et renvoie un code en fonction de la touche appuyé
 * 
 * @return int Un code correspondant à la touche appuyé
 */
int eventSDL(void);

/**
 * @brief Calcule le zoom a effectuer sur une SDL_Surface de texte en fonction de la taille et de la taille voulu
 * 
 * @param strLen La longueur de la chaine de texte
 * @param charWidth La largeur en pixel d'un caractère
 * @param charHeight La hauteur en pixel d'un caractère
 * @param maxWidth La largeur maximale du texte final
 * @param maxHeight La hauteur maximale du texte final
 * @return float Le zoom a effectuer
 */
float getFontScale(int strLen, int charWidth, int charHeight, int maxWidth, int maxHeight);

/**
 * @brief Créer une surface de texte à partir d'une chaîne de caractère donnée
 * 
 * @param font Un pointuer sur la TTF_Font chargée
 * @param str La chaine de caractère à afficher
 * @param charWidth La largeur en pixel d'un caractère
 * @param charHeight La hauteur en pixel d'un caractère
 * @param maxWidth La largeur maximale du texte final
 * @param maxHeight La hauteur maximale du texte final
 * @param colorR La valeur de la couleur rouge de texte a afficher
 * @param colorG La valeur de la couleur verte de texte a afficher
 * @param colorB La valeur de la couleur bleue de texte a afficher
 * @return SDL_Surface* Un pointeur sur la SDL_Surface créé
 */
SDL_Surface * getFont(TTF_Font * font, char * str, int charWidth, int charHeight, int maxWidth, int maxHeight, int colorR, int colorG, int colorB);

/**
 * @brief Détermine la couleur à afficher pour une case du tableau de jeu en fonction de sa valeur
 * 
 * @param value La valeur de la case a afficher
 * @return int* Un tableau contenant les canaux RGB de la case a afficher
 */
int * getColorRGB(int value);

/**
 * @brief Affiche les éléments constituant le jeu dans une fenetre graphique
 * 
 * @param plate Un pointeur sur la stucture "struct _grille" de jeu
 * @param gameAsset Un pointuer sur la stucture "struct _gameTextures" des éléments graphiques en cache
 */
void displayGrid(grille* plate, gameTextures * gameAsset);

/**
 * @brief 
 * 
 * @param gameAsset Un pointuer sur la stucture "struct _gameTextures" des éléments graphiques en cache
 * @param select Un entier correspondant à la séléction du texte sur l'écran de pause
 */
void displayMenu(gameTextures * gameAsset, int select);

#endif