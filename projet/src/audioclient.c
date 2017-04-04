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
  char* buf;/*Buffers de reception des données*/

  strcpy(audio_metadata, filename);

  dest.sin_family = AF_INET;
  dest.sin_port = htons(PORT);
  dest.sin_addr.s_addr = htonl(INADDR_ANY);

  /* Envois de la requête au serveur */
  error = sendto(fd, audio_metadata, BUFFER_SIZE, 0, (struct sockaddr*) &dest, sizeof(struct sockaddr_in));

  if(error < 0) {
    return error;
  }

  printf("============= CLIENT =============\n\n\n");

  /* Attente de la réponse du serveur et traitement de la réponse*/
  error = recvfrom(fd, audio_metadata, BUFFER_SIZE, 0, NULL, 0);

  printf("%s\n", audio_metadata);

  /*Initialisation des métadonnées du fichier audio*/
  sample_rate = atoi(strtok(audio_metadata, ";"));
  sample_size = atoi(strtok(NULL, ";"));
  channels = atoi(strtok(NULL, ";"));

  fd_write = error = aud_writeinit(sample_rate, sample_size, channels);

  if(error < 0) {
    return error;
  }

  buf = (char*) malloc(sample_size);

  /*Lecture du fichier audio*/
  error = sendto(fd, " ", 1, 0, (struct sockaddr*) &dest, sizeof(struct sockaddr_in));

  while(sample_size >= (error = recvfrom(fd, buf, sizeof(buf), 0, NULL, 0))) {

    if(strcmp("FIN", buf)) break;

    printf("Test\n");

    error = write(fd_write, buf, (size_t)sample_size);

    printf("%s\n", buf);

    if(error < 0) {
      return error;
    }

    error = sendto(fd, " ", 1, 0, (struct sockaddr*) &dest, sizeof(struct sockaddr_in));

    bzero(buf, sample_size);
  }

  free(buf);

  if(error < 0) {
    return error;
  }

  return 0;
}
