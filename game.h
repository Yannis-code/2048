/**
 * @file game.h
 * @author ROCHE Yannis  - yannis.roche@etu.uca.fr  - 22002168
 * @author DUPOIS Thomas - thomas.dupois@etu.uca.fr - 22001214
 * @brief Fichier header de la partie logique de jeu du programme
 * @version 0.1
 * @date 2021-12-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef GAME
#define GAME

#include "constantes.h"

/**
 * @brief Créer une stucture "struct _grille"
 * 
 * @param size La taille de la grille à créer
 * @return grille* Un pointeur sur la structure créée
 */
grille * newGrid(int size);

/**
 * @brief Libere la mémoire alloué à la stucture "struct _grille" passée en paramètre
 * 
 * @param plate Un pointeur sur la stucture "struct _grille" à libérer
 */
void freeGrid(grille * plate);

/**
 * @brief Copie le contenu d'une stucture "struct _grille" vers une autre stucture "struct _grille"
 * 
 * @param plateCpy Un pointeur sur la stucture "struct _grille" de destination
 * @param plateSrc Un pointeur sur la stucture "struct _grille" source
 */
void copyGrid(grille * plateCpy, grille * plateSrc);

/**
 * @brief Sauvergarde les données de la partie en cours dans un fichier propre à la taille de la grille
 * 
 * @param plate Un pointeur sur la stucture "struct _grille" à sauvegarder
 */
void saveGame(grille * plate);

/**
 * @brief Charge une partie à partir d'un fichier de sauvegarde s'il existe
 * 
 * @param taille La taille de la grille à charger
 * @return grille* Un pointeur sur une stucture "struct _grille" chargé ou créé si aucune sauvegarde n'est disponible
 */
grille * loadGame(int taille);

/**
 * @brief Place un nombre aléatoire (2 ou 4) dans la grille à un emplacement libre choisi aléatoirement
 * 
 * @param plate Un pointeur sur la stucture "struct _grille" où placer un nombre aléatoire
 * @param nb Le nombre de nombre aléatoire à placer
 */
void placeRandomNumber(grille * plate, int nb);

/**
 * @brief Vérifie s'il existe au moins un emplacement de libre dans la grille de jeu
 * 
 * @param plate Un pointeur sur la stucture "struct _grille" où effectuer la vérification
 * @return int 1 si un espace est disponible, 0 sinon
 */
int checkFreeSpace(grille * plate);

/**
 * @brief Vérifie si plus aucun mouvement n'est possible dans la grille
 * 
 * @param plate Un pointeur sur la stucture "struct _grille" où effectuer la vérification
 * @return int 1 si plus aucun mouvement n'est possible, 0 sinon
 */
int gameOver(grille * plate);

/**
 * @brief Récupère un pointeur sur un case du tableau 2D d'une stucture "struct _grille" en fonction de la direction de déplacement
 *          dans le tableau et d'indices de coordonnées
 * 
 * @param plate Un pointeur sur la stucture "struct _grille" où récupérer l'indice
 * @param i Indice du premier axe d'évolution dans le tableau
 * @param j Indice du second axe d'évolution dans le tableau
 * @param direction Un entier correspondant à la direction d'évolution dans le tableau de jeu
 * @return unsigned long long int* Un pointeur sur la case du tableau obtenue par l'interprétation des paramètres
 */
unsigned long long int * getArrayPointer(grille * plate, int i, int j, int direction);

/**
 * @brief Effectue les déplacements des cases du tableau en fonction de la direction donnée par le joueur
 * 
 * @param plate Un pointeur sur la stucture "struct _grille" où mettre à jour la position et la valeur des cases
 * @param direction La direction du déplaceent du joueur
 * @return int Le nombre de déplacement ayant été affectuées
 */
int updateGrid(grille * plate, int direction);

/**
 * @brief Met à jour le temps passé depuis le lancement du programme
 * 
 * @param gameTimer Un pointeur sur la structure "struct _timer" à mettre à jour
 */
void updateTimer(timer * gameTimer);

/**
 * @brief Boucle de jeu principale pour le jeu en console
 * 
 * @param plate Un pointeur sur la stucture "struct _grille" de jeu
 */
void consoleGameLoop(grille * plate);

/**
 * @brief Boucle de jeu principale pour le jeu en SDL
 * 
 * @param plate Un pointeur sur la stucture "struct _grille" de jeu
 */
void graphiqueGameLoop(grille * plate);

#endif