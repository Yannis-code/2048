/**
 * @file console.h
 * @author ROCHE Yannis  - yannis.roche@etu.uca.fr  - 22002168
 * @author DUPOIS Thomas - thomas.dupois@etu.uca.fr - 22001214
 * @brief Fichier header de la partie affichage console du programme
 * @version 0.1
 * @date 2021-12-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef CONSOLE
#define CONSOLE

#include "constantes.h"

/**
 * @brief Vide le buffer "stdin" pour éviter les problème liée au scanf
 * 
 */
void vider_buffer(void);

/**
 * @brief Détermine le nombre de chiffres dans un nombre passé en paramètre
 * 
 * @param number Le nombre dont on veut déterminer le nombre de chiffres
 * @return int Le nombre de chiffres
 */
int getSizeOfNumber(int number);

/**
 * @brief Détermine la taille maximale des chiffres présent dans la grille de jeu
 * 
 * @param plate Un pointeur sur la stucture "struct _grille"
 * @return int La taille maximale trouvé
 */
int getMaxNumber(grille * plate);

/**
 * @brief Affiche un menu au début du programme pour déterminer le mode d'affichage à utiliser et la taille de la grille
 * 
 * @param affichage Un pointeur sur l'entier stockant le mode d'affichage
 * @param taille Un pointeur sur l'entier stockant la taille de la grille
 */
void menu(int * affichage, int * taille);

/**
 * @brief Affiche des séparateur horizotaux dans la console
 * 
 * @param size Le nombre de caractère à afficher
 */
void printSeparator(int size);

/**
 * @brief Affiche la grille de jeu dans la console
 * 
 * @param plate Un pointeur sur la stucture "struct _grille" de jeu
 */
void printGame(grille * plate);

#endif