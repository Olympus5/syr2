#ifndef _REVERSE_H
#define _REVERSE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct file_list file_list;

struct file_list
{
  int index;
  char buf[1024];
  int nbByte;
  file_list* next;
};

/**
 * Met le contenu d'un fichier dans une liste chainé
 * @fd descripteur du fichier ouvert
 * @return la tête de liste, sinon NULL
 */
file_list* fileToList(int fd);

/**
 * Renverse la liste chainé ainsi que le contenu des buffers des maillons
 * @header tête de liste
 * @return -1 si échec, 0 sinon
 */
int reverse(file_list** header);

/**
 * Créer un nouvelle élément de type file_list
 * @return renvoie le nouvelle élément si succès, sinon NULL
 */
file_list* newElement();

/**
 * Supprime un chainon dans la liste chainé
 * @param fl le chainon à supprimer
 */
void freeElement(file_list* fl);

#endif
