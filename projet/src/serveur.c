#include <serveur.h>

int main(int argc, char* argv[]) {
  int fd, error;
  struct sockaddr_in addr;
  struct sockaddr_in from;
  socklen_t len, flen;
  char buf[1024], buf_test[64] = "LOURD !";

  printf("============= SERVEUR =============\n\n\n");

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  error = fd;

  if(error < 0) {
    perror("Error socket");
    exit(1);
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(50000);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  error = bind(fd, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));

  if(error < 0) {
    perror("Error bind");
    exit(1);
  }

  flen = sizeof(struct sockaddr_in);

  while(1) {
    len = recvfrom(fd, buf, strlen(buf)+1, 0, (struct sockaddr*) &from, &flen);

    error = len;

    if(error < 0) {
      perror("Error recvfrom");
      exit(1);
    }

    error = sendto(fd, buf_test, strlen(buf_test)+1, 0, (struct sockaddr*) &from, flen);

    if(error < 0) {
      perror("Error sendto");
      exit(1);
    }
  }

  close(fd);

  return 0;
}
