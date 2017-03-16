#include <client.h>

int main(int argc, char* argv[]) {
  int fd, error;
  struct sockaddr_in addr;
  struct sockaddr_in dest;
  socklen_t len;
  char buf[1024], buf_test[64] = "LOURD !";

  printf("============= CLIENT =============\n\n\n");

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  if(error < 0) {
    perror("Error socket");
    exit(1);
  }

  dest.sin_family = AF_INET;
  dest.sin_port = htons(50000);
  dest.sin_addr.s_addr = htonl(INADDR_ANY);

  error = sendto(fd, buf, strlen(buf)+1, 0, (struct sockaddr*) &dest, sizeof(struct sockaddr_in));

  if(error < 0) {
    perror("Error sendto");
    exit(1);
  }

  error = recvfrom(fd, buf, strlen(buf)+1, 0, NULL, 0);

  if(error < 0) {
    perror("Error recvfrom");
    exit(1);
  }

  println("%s\n", buf);

  close(fd);

  return 0;

}
