#include <audioserver.h>

int main(int argc, char* argv[]) {
  int fd, error;

  /*Lancement du serveur audio*/
  fd = error = start_server();

  if(error < 0) {
    perror("Erreur lors du démarrage du serveur\n");
    close(fd);
    exit(1);
  }

  /*Mise en écoute du serveur*/
  error = request_handling(fd);

  if(error < 0) {
    perror("Erreur lors du traitement des requêtes\n");
    close(fd);
    exit(1);
  }

  return 0;
}

int start_server() {
  int error, fd;
  struct sockaddr_in addr;

  printf("============= SERVEUR =============\n\n\n");

  /*Initialise la socket*/
  fd = socket(AF_INET, SOCK_DGRAM, 0);

  error = fd;
  if(error < 0) {
    return error;
  }

  /*Initialise l'adresse réseau et le port du serveur*/
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  /*Met les informations du serveur sur la socket*/
  error = bind(fd, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));

  if(error < 0) {
    return error;
  }

  char* adresse = "0.0.0.0";

  printf("PORT: %d\nADRESSE: %s\n", PORT, adresse);

  return fd;
}

int request_handling(int fd) {
  int error;
  socklen_t len, flen;
  char buf[BUFFER_SIZE], buf_test[64] = "LOURD !";
  struct sockaddr_in from;

  flen = sizeof(struct sockaddr_in);

  /*Mise sous écoute du serveur sur le port PORT (audioserver.h)*/
  while(1) {
    /*Reception d'une requête (ici du nom du fichier audio à lire)*/
    len = recvfrom(fd, buf, BUFFER_SIZE, 0, (struct sockaddr*) &from, &flen);

    printf("\n--------------------------------------\n\n");

    printf("filename: %s\n", buf);

    error = len;

    if(error < 0) {
      return error;
    }

    //error = sendto(fd, buf_test, strlen(buf_test)+1, 0, (struct sockaddr*) &from, flen);

    error = lectura_expendiente(buf, fd, from);

    if(error < 0) {
      return error;
    }

    bzero(buf, (size_t)BUFFER_SIZE);
  }

  return 0;
}

int lectura_expendiente(char* filename, int fd, struct sockaddr_in from) {
  int sample_rate, sample_size, channels;/*Metadonnées du fichier audio*/
  int fd_read;
  int flen;
  char audio_metadata[BUFFER_SIZE];/*Buffers pour l'envoie des metadonnées*/
  char* buf;/*Buffers pour l'envoie des données*/
  ssize_t error;

  /*Initialisation du descripteur de fichier pour lire la piste audio*/
  fd_read = error = aud_readinit(filename, &sample_rate, &sample_size, &channels);

  if(error < 0) {
    return error;
  }

  buf = (char*) malloc(sample_size);

  flen = sizeof(struct sockaddr_in);

  sprintf(audio_metadata, "%d;%d;%d", sample_rate, sample_size, channels);

  error = sendto(fd, audio_metadata, BUFFER_SIZE, 0, (struct sockaddr*) &from, flen);

  if(error < 0) {
    return error;
  }

  bzero(audio_metadata, BUFFER_SIZE);

  /*Envoie des informtions du fichier audio au client*/
  while((ssize_t)sample_size <= (error = read(fd_read, buf, (size_t)sample_size))) {
    error = sendto(fd, buf, sample_size, 0, (struct sockaddr*) &from, flen);

    //printf("%d => %d\n", sample_size, error);

    error = 10;

    if(error < 0) {
      return error;
    }

    bzero(buf, sample_size);
  }

  free(buf);

  if(error < 0) {
    return error;
  }

  return 0;
}
