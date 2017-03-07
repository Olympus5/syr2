#include <nettoie.h>

int main(int argc, char* argv[]) {
  int idSegment = shmget((key_t)1234, TAILLE+sizeof(int), 0600|IPC_CREAT);
  int error = idSegment;

  if(error == -1) {
    perror("Error shmget");
    exit(1);
  }

  error = shmctl(idSegment, IPC_RMID, NULL);

  if(error == -1) {
    perror("Error shmctl");
    exit(1);
  }

  return 0;
}
