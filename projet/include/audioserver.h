#ifndef AUDIO_SERVER_H
#define AUDIO_SERVER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <audio.h>

#define PORT 50000
#define BUFFER_SIZE 1024

/**
 * Initialise le serveur audio
 * @return un code entier inferieur à 0 si il y a une erreur, sinon le descripteur de fichier vers le socket du serveur
 */
int start_server();

/**
 * Traite les différentes requêtes reçu
 * @param fd descripteur de fichier du socket
 * @return un code entier inférieur à 0 si il y a une erreur, sinon 0
 */
int request_handling(int fd);

/**
 * Lecture d'un fichier audio et l'envoie au client
 * @param filename nom de fichier à lire
 * @param fd descripteur de fichier du socket
 * @param from interface réseau du client
 * @return un code entier inferieur à 0 si il y a une erreur, sinon 0
 */
int read_file(char* filename, int fd, struct sockaddr_in from);

/**
 * Envoie les metadonnées du fichier demandé par le client
 * @param fd
 * @param metadata
 * @param from
 * @flen
 * @return
 */
int send_metadata(int fd, char* metadata, struct sockaddr_in* from, int flen, fd_set* readfds, struct timeval* tv);

#endif
