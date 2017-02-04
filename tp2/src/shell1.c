#include <shell1.h>

int main(int argc, char* argv[])
{
  char commande[50];

  while(1) {
    printf("Entrez une commande: ");
    fgets(commande, sizeof(commande), stdin);
    clean(commande);
    printf("\n");

    commandthread(commande);
  }

  return 0;
}

static void clean(char* string) {
  char* p = strchr(string, '\n');

  if(p != NULL) {
    *p = '\0';
  } else {
    purger();
  }
}

static void purger() {
  int c;

  while((c = getchar()) != '\n' && c != EOF);
}

int commandthread(char* commande) {
  int ret = -1, status;
  pid_t pid = fork();

  if(pid == -1) {
    fprintf(stderr, "Erreur fork");
    exit(1);
  }

  if(!strcmp(commande, "exit")) exit(0);

  if(pid == 0) {
    execlp(commande, commande, NULL);
    ret = 1;
  } else {
    waitpid(-1, &status, 0);
    printf("\n");
    ret = 1;
  }

  return ret;
}
