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
 * Initialise l'envoie des metadonnées
 * @param audio_metadata metadonnées à envoyer au client
 * @param filename nom du fichier à lire
 * @param readfds liste des descripteur de fichier du client à mettre à zéro
 * @param tv timeout de la première requête
 * @param dest information du destinataire
 */
void init_request(char* audio_metadata, char* filename, char* choix, fd_set* readfds, struct timeval *tv, struct sockaddr_in *dest);

/**
 * envoie les métadonnées au serveur afin d'établir une connexion
 * @param fd descripteur de fichier du socket
 * @param audio_metadata metadonnées à envoyer au client
 * @param readfds liste des descripteur de fichier du client à mettre à zéro
 * @param tv timeout de la première requête
 * @param dest information du destinataire
 * @return un code entier inferieur à 0 si il y a une erreur, 1 si la connexion n'a pu etre initialisée, 0 sinon
 */
int send_metadata(int fd, char* audio_metadata, fd_set* readfds, struct timeval *tv, struct sockaddr_in *dest);

/**
 * initialise l'écriture d'un fichier audio sur une sortie audio
 * @param audio_metadata
 * @param choix
 * @param sample_rate
 * @param sample_size
 * @param channels
 * @return un code entier inferieur à 0 si il y a une erreur, 0 sinon
 */
int init_write(char* audio_metadata, char* choix, char* sound, int* sample_rate, int* sample_size, int* channels);

/**
 * Traite les différentes étapes de la requête vers le serveur audio
 * @param fd
 * @param filename
 * @return un code entier inferieur à 0 si il y a une erreur, 0 sinon
 */
int request_handling(int fd, char* filename);

#endif
