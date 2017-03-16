#include <client.h>

int main(int argc, char* argv[]) {
  int fd, error;
  struct sockaddr_in dest;
  char buf[1024];

  printf("============= CLIENT =============\n\n\n");

  fd = socket(AF_INET, SOCK_DGRAM, 0);

  error = fd;

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

  printf("%s\n", buf);

  close(fd);

  return 0;

}
