#ifndef AUDIO_CLIENT_H
#define AUDIO_CLIENT_H

#include <lecture.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <audio.h>

#define PORT 50000
#define BUFFER_SIZE 1024

/**
 * Initialise une connection avec un serveur audio
 * @return un code entier inferieur à 0 si il y a une erreur, sinon un descripteur de fichier vers le socket du client
 */
int init_client();

/**
 * Traite les différentes étapes de la requête vers le serveur audio
 * @return un code entier inferieur à 0 si il y a une erreur, sinon 0
 */
int request_handling(int fd, char* filename);

#endif
