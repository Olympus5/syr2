#ifndef SHELL_2_H
#define SHELL_2_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * Si échec de la lecture clavier vide stdin des caractère en trops
 */
static void purger();

/**
 * Supprime le caractère '\n' de la fin de saisie au clavier par fgets()
 * @param string chaine saisie au clavier
 */
static void clean(char* string);

/**
 * fonction qui traite les commande envoyer par l'utilisateur
 * @param commande commande à traiter dans l'environnement
 * @return -1 si échec du traitement de la commande, 1 sinon
 */
int commandthread(char* commande);

/**
 * Analyse une commande afin de déterminer son nombre de paramètres
 * @param commande commande à analyser
 * @return le nombre de paramètre de la fonction
 */
int numberofparams(char* commande);

/**
 * fonction qui sépare le nom et les différents attributs d'une commande
 * @param commande commande à traiter
 * @param count nombre de paramètres
 * @param params tableau de paramètres
 * @return un code d'erreur, sinon 1:
 *   -1 si pas de commande,
 *   -2 si nombre de paramètres inférieur ou égale à zéro,
 *   -3 si tableau de paramètre non spécifié
 */
int splitcommand(char* commande, const int count, char* params[]);

#endif
