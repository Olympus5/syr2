#include <audioserver.h>

int main(int argc, char* argv[]) {
  int fd, error;
  char* name;

  if(argc > 1) {
    name = argv[1];
  } else {
    name = "localhost";
  }

  /*Lancement du serveur audio*/
  fd = error = start_server(name);

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

  close(fd);

  return 0;
}

int start_server(char* name) {
  int error, fd;
  struct sockaddr_in addr;
  struct hostent *resolve;
  char* adresse;

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

  /*Récupère les informations du serveur pour les afficher*/
  resolve = gethostbyname(name);

  printf("Testttt\n");

  if(resolve == NULL) {
    fprintf(stderr, "Adresse non trouvé pour: %s\n", name);
    exit(1);
  }

  adresse = inet_ntoa(*((struct in_addr*) (resolve->h_addr_list)[0]));

  printf("NOM D'HOTE: %s\nADRESSE: %s\nPORT: %d\n", name, adresse, PORT);

  return fd;
}

int request_handling(int fd) {
  int error;
  socklen_t len, flen;
  char buf[BUFFER_SIZE];
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

    /*error = sendto(fd, buf_test, strlen(buf_test)+1, 0, (struct sockaddr*) &from, flen);*/

    error = send_file(buf, fd, from);

    if(error < 0) {
      return error;
    }

    bzero(buf, (size_t)BUFFER_SIZE);
  }

  return 0;
}

int send_file(char* filename, int fd, struct sockaddr_in from) {
  int sample_rate, sample_size, channels;/*Metadonnées du fichier audio*/
  int fd_read;
  socklen_t flen;
  char audio_metadata[BUFFER_SIZE];/*Buffers pour l'envoie des metadonnées*/
  char buf_data[BUFFER_SIZE];/*Buffers pour l'envoie des données*/
  char buf[1];/*buffer qui stocke le flag de requête pour recevoir la partie suivante de la donnée*/
  char* end = "FIN";/*flag de fin d'envoie de la donnée*/
  ssize_t error;
  struct timeval tv;/*Timeout du serveur*/
  int nb;/*nombre de descripteurs de fichiers prêts*/
  fd_set readfds;/*Listes des descripteur de fichiers en lecture*/

  /*Initialisation de select pour gérer un TIMEOUT*/
  tv.tv_sec = 10;
  tv.tv_usec = 0;
  FD_ZERO(&readfds);

  /*Initialisation du descripteur de fichier pour lire la piste audio*/
  fd_read = error = aud_readinit(filename, &sample_rate, &sample_size, &channels);

  if(error < 0) {
    return error;
  }

  flen = sizeof(struct sockaddr_in);

  sprintf(audio_metadata, "%d;%d;%d", sample_rate, sample_size, channels);

  error = sendto(fd, audio_metadata, BUFFER_SIZE, 0, (struct sockaddr*) &from, flen);

  if(error < 0) {
    return error;
  }

  bzero(audio_metadata, (size_t)BUFFER_SIZE);

  /*int i = 0;*/

  /*Envoie des informtions du fichier audio au client*/
  while((ssize_t) sample_size <= (error = read(fd_read, buf_data, (size_t) sample_size))) {
    tv.tv_sec = 10;
    tv.tv_usec = 0;

    if(error < 0) {
      return error;
    }

    FD_SET(fd, &readfds);

    /*printf("Valeur de i=%d\n", i);
    printf("Attente d'une requête...\n");*/

    nb = error = select(fd+1, &readfds, NULL, NULL, &tv);

    if(error < 0) {
      return error;
    }

    if(nb == 0) {/*Si timeout atteind...*/
      bzero(buf_data, (size_t)BUFFER_SIZE);
      break;
    }

    if (FD_ISSET(fd, &readfds)) {
      error = recvfrom(fd, buf, 1, 0, (struct sockaddr*) &from, &flen);
      if(error < 0) {
        return error;
      }

      error = sendto(fd, buf_data, BUFFER_SIZE, 0, (struct sockaddr*) &from, flen);

      /*printf("%d => %d\n", sample_size, error);*/

      if(error < 0) {
        return error;
      }
    }

    /*i++;*/

    FD_CLR(fd, &readfds);
    bzero(buf_data, (size_t)BUFFER_SIZE);
  }

  /*Pour gérer le timeout*/
  if(nb == 0) {
    printf("Délais expiré\n");
  } else {
    error = recvfrom(fd, buf, 1, 0, (struct sockaddr*) &from, &flen);

    if(error < 0) {
      return error;
    }

    error = sendto(fd, end, strlen(end), 0, (struct sockaddr*) &from, flen);
  }

  if(error < 0) {
    return error;
  }

  return 0;
}
