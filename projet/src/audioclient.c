#include <audioclient.h>

/**
 * Pour lancer une requête au serveur audio:
 *      bin/audioclient <nom_fichier>
 */
int main(int argc, char* argv[]) {
  int fd, error;
  char* file = argv[1];
  char* extension;

  /* Vérification du fichier passé en paramètre */
  if(argc <= 1) {
    fprintf(stderr, "Erreur, veuillez indiquer le fichier audio à lire\n");
    exit(1);
  }
  /* Vérification du fichier passé est bien un .wav */
  extension = strrchr(file, '.');

  if(strcmp(extension, ".wav")) {
    fprintf(stderr, "Erreur, le fichier n'est pas de type WAV\n");
    exit(1);
  }

  /* Lancement du client */
  fd = error = init_client();

  if(error < 0) {
    perror("Erreur lors de l'initialisation du client\n");
    exit(1);
  }

  /* Envois de la requête */
  error = request_handling(fd, file);

  if(error < 0) {
    perror("Erreur lors de la requête client");
    exit(1);
  }

  /* Fermeture du client */
  close(fd);

  return 0;
}

int init_client() {
  int fd, error;

  printf("============= CLIENT =============\n\n\n");

  /* Initialise la socket */
  fd = socket(AF_INET, SOCK_DGRAM, 0);

  error = fd;

  if(error < 0) {
    return error;
  }

  return fd;
}

int request_handling(int fd, char* filename) {
  int error;
  int sample_rate, sample_size, channels;/*Metadonnées fichier audio*/
  int fd_write;
  struct sockaddr_in dest;
  char audio_metadata[BUFFER_SIZE];/*Buffers de reception des metadonnées*/
  char buf[BUFFER_SIZE];/*Buffers de reception des données*/
  struct timeval tv;/*Timeout du client*/
  int nb;/*nombre de descripteurs de fichiers prêts*/
  fd_set readfds;/*Listes des descripteur de fichiers*/

  dest.sin_family = AF_INET;
  dest.sin_port = htons(PORT);
  dest.sin_addr.s_addr = htonl(INADDR_ANY);

  FD_ZERO(&readfds);

  /*Demande d'une piste audio au serveur + reception des metadonnées*/
  do {
    /* Envois de la requête au serveur */
    error = sendto(fd, filename, BUFFER_SIZE, 0, (struct sockaddr*) &dest, sizeof(struct sockaddr_in));

    if(error < 0) {
      return error;
    }

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    FD_SET(fd, &readfds);

    nb = error = select(fd+1, &readfds, NULL, NULL, &tv);

    if(error < 0){
      return error;
    }

    if (FD_ISSET(fd, &readfds)) {
      /* Attente de la réponse du serveur et traitement de la réponse*/
      error = recvfrom(fd, audio_metadata, BUFFER_SIZE, 0, NULL, 0);

      if(error < 0){
        return error;
      }
    }

    FD_CLR(fd, &readfds);
  } while(nb == 0);

  printf("%s\n", audio_metadata);

  /*Initialisation des métadonnées du fichier audio*/
  sample_rate = atoi(strtok(audio_metadata, ";"));
  sample_size = atoi(strtok(NULL, ";"));
  channels = atoi(strtok(NULL, ";"));

  fd_write = error = aud_writeinit(sample_rate, sample_size, channels);

  if(error < 0) {
    return error;
  }

  /*Lecture du fichier audio*/

  do {

    do {
      error = sendto(fd, " ", 1, 0, (struct sockaddr*) &dest, sizeof(struct sockaddr_in));

      if(error < 0) {
        return error;
      }

      tv.tv_sec = 1;
      tv.tv_usec = 0;
      FD_SET(fd, &readfds);

      nb = error = select(fd+1, &readfds, NULL, NULL, &tv);

      if(error < 0){
        return error;
      }

      if (FD_ISSET(fd, &readfds)) {
        /* Attente de la réponse du serveur et traitement de la réponse*/
        error = recvfrom(fd, buf, BUFFER_SIZE, 0, NULL, 0);

        if(error < 0){
          return error;
        }
      }

      FD_CLR(fd, &readfds);
    } while (nb == 0);

    if(strncmp("FIN", buf, (size_t) 3) == 0) break;

    printf("Test\n");

    error = write(fd_write, buf, (size_t)sample_size);

    printf("%s\n", buf);

    if(error < 0) {
      return error;
    }

    bzero(buf, (size_t)BUFFER_SIZE);
  } while (error <= BUFFER_SIZE);

  if(error < 0) {
    return error;
  }

  return 0;
}
