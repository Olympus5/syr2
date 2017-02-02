#ifndef SHELL_H
#define SHELL_H

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
 */
int commandthread(char* commande);

#endif
