#include <audioserver.h>

int main(int argc, char* argv[]) {
  int fd, error;


  fd = error = start_server();

  if(error < 0) {
    perror("Erreur lors du démarrage du serveur\n");
    exit(1);
  }

  error = request_handling(fd);

  if(error < 0) {
    perror("Erreur lors du traitement des requêtes\n");
    exit(1);
  }

  close(fd);

  return 0;
}

int start_server() {
  int error, fd;
  struct sockaddr_in addr;

  printf("============= SERVEUR =============\n\n\n");

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  error = fd;
  if(error < 0) {
    return error;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

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

  while(1) {
    len = recvfrom(fd, buf, BUFFER_SIZE, 0, (struct sockaddr*) &from, &flen);

    printf("filename: %s\n", buf);
    bzero(buf, (size_t)BUFFER_SIZE);

    error = len;

    if(error < 0) {
      return error;
    }

    error = sendto(fd, buf_test, strlen(buf_test)+1, 0, (struct sockaddr*) &from, flen);

    if(error < 0) {
      return error;
    }
  }

  return 0;
}

int lectura_expendiente(char* filename) {
  int sample_rate = 44100;
  int sample_size = 16;
  int channels = 2;
  int fd_read, fd_write;
  char buf[sample_size];
  ssize_t error;

  fd_write = aud_writeinit(sample_rate, sample_size, channels);

  if(fd_write < 0) {
    fprintf(stderr, "Erreur, ouverture écriture audio");
    exit(1);
  }

  fd_read = aud_readinit(filename, &sample_rate, &sample_size, &channels);

  if(fd_read < 0) {
    fprintf(stderr, "Erreur, ouverture lecture audio");
    exit(1);
  }

  while((ssize_t)sample_size >= (error = read(fd_read, buf, (size_t)sample_size))) {
    error = write(fd_write, buf, (size_t)sample_size);
    if(error < 0) {
      perror(NULL);
      exit(1);
    }
    bzero(buf, sample_size);
  }
}
