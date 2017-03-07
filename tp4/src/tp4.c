#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/sem.h>

#define TAILLE 1024

void ecrire_tableau(int *compteur, char *tableau) {
  char message[64], *msg=message;

  snprintf(message, 64, "Je suis le processus %d!\n", getpid());

  while ((*compteur<TAILLE)&&(*msg)) {
    tableau[*compteur] = *msg;
    msg++;
    usleep(100000);
    (*compteur)++;
  }
}

int main() {
  int id, *compteur;
  char *tableau;

  /*Identifiant de la sémaphore*/
  int idSem;
  int error;
  /*Etat UP et DOWN*/
  struct sembuf up = {0, 1, 0};
  struct sembuf down = {0, -1, 0};

  id = shmget((key_t)1234,TAILLE+sizeof(int),0600|IPC_CREAT);

  if (id < 0) { perror("Error shmget"); exit(1); }

  compteur = (int*) shmat(id,0,0);

  if ( compteur==NULL ) { perror("Error shmat"); exit(1); }

  tableau = (char *)(compteur + 1);


  /*
  ------------------------------------Création, initialisation et DOWN de la sémaphore-------------------------------------------------
  */
  idSem = semget((key_t)1234, 1, 0600|IPC_EXCL|IPC_CREAT);
  error = idSem;

  if(error < 0) {
    perror("Error semget");
    exit(1);
  }

  error = semctl(idSem, 0, SETVAL, 1);

  if(error < 0) {
    perror("Erreur init MUTEX");
    exit(1);
  }

  error = semop(idSem, &down, 1);

  if(error < 0) {
    perror("Error semop DOWN");
    exit(1);
  }

  ecrire_tableau(compteur, tableau);
  printf("%s\n", tableau);

  /*
  ------------------------------------UP et destruction de la sémaphore-------------------------------------------------
  */

  error = semop(idSem, &up, 1);

  if(error < 0) {
    perror("Error semop UP");
    exit(1);
  }

  error = semctl(idSem, 0, IPC_RMID, 0);

  if(error < 0) {
    perror("Error shmctl");
    exit(1);
  };

  if (shmdt(compteur) < 0) { perror("Error shmdt"); exit(1); }
  return 0;
}
