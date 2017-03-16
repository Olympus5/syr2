#include <audioclient.h>

int main(int argc, char* argv[]) {
  int fd, error;
  char* file = argv[1];
  char* extension;

  if(argc <= 1) {
    fprintf(stderr, "Erreur, veuillez indiquer le fichier audio à lire\n");
    exit(1);
  }

  extension = strrchr(file, '.');

  if(strcmp(extension, ".wav")) {
    fprintf(stderr, "Erreur, le fichier n'est pas de type WAV\n");
    exit(1);
  }

  fd = error = init_client();

  if(error < 0) {
    perror("Erreur lors de l'initialisation du client\n");
    exit(1);
  }

  error = request_handling(fd, file);

  if(error < 0) {
    perror("Erreur lors de la requête client");
    exit(1);
  }

  close(fd);

  return 0;
}

int init_client() {
  int fd, error;

  printf("============= CLIENT =============\n\n\n");

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  error = fd;

  if(error < 0) {
    return error;
  }

  return fd;
}

int request_handling(int fd, char* filename) {
  int error;
  struct sockaddr_in dest;
  char buf[BUFFER_SIZE];

  strcpy(buf, filename);

  dest.sin_family = AF_INET;
  dest.sin_port = htons(PORT);
  dest.sin_addr.s_addr = htonl(INADDR_ANY);

  error = sendto(fd, buf, BUFFER_SIZE, 0, (struct sockaddr*) &dest, sizeof(struct sockaddr_in));

  if(error < 0) {
    perror("Error sendto");
    exit(1);
  }

  error = recvfrom(fd, buf, BUFFER_SIZE, 0, NULL, 0);

  if(error < 0) {
    perror("Error recvfrom");
    exit(1);
  }

  printf("%s\n", buf);

  return 0;
}
