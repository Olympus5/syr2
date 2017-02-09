#include <shell3.h>

int main(int argc, char* argv[])
{
  char commande[50];
  char commande2[50];

  while(1) {
    printf("Entrez une premiere commande: ");
    fgets(commande, sizeof(commande), stdin);
    clean(commande);

    printf("\nEntrez une deuxieme commande: ");
    fgets(commande, sizeof(commande2), stdin);
    clean(commande2);

    printf("\n");

    commandthread(commande, commande2);
    break;
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

int commandthread(char* commande, char* commande2) {
  int ret = -1,
      status,
      size = numberofparams(commande),
      size2 = numberofparams(commande2),
      error, error2;
  char* params[size];
  char* params2[size2];
  pid_t pid;

  //tableau d'entier qui stocke les tampons I/O du pipe
  int fd[2];

  error = splitcommand(commande, size, params);
  error2 = splitcommand(commande2, size2, params2);

  //Création du pipe AVANT le fork, on ne crée un tube que dans un seul processus
  pipe(fd);

  if(error > 0 && error2 > 0) {
    pid = fork();

    if(pid == -1) {
      perror("Erreur fork");
      exit(1);
    }

    if(!strcmp(params[0], "exit")) exit(0);

    if(pid == 0) {
      execvp(params[0], params);
      //Fermeture du pipe in
      close(fd[1]);
    } else {
      waitpid(-1, &status, 0);
      //Fermeture du pipe out
      close(fd[0]);
      printf("\n");
    }

    ret = 1;
  } else {
    printf("%d\n", error);
  }

  return ret;
}

int numberofparams(char* commande) {
  int c = 0;
  char* token = NULL;
  char tmp[strlen(commande)];

  //Copie la commande pour pouvoir l'utiliser par la suite dans d'autre fonctions
  strcpy(tmp, commande);

  //Calcul le nombre de paramètre de la commande
  token = strtok(tmp, " ");
  if(token != NULL) c++;

  while(token != NULL) {
    token = strtok(NULL, " ");
    c++;
  }

  return c;
}

int splitcommand(char* commande, const int count, char* params[]) {
  int ret = -1;
  int i;
  char* token;

  //Commande inexistante
  if(commande != NULL) {
    //Nombre de paramètre inférieur ou égal à 0
    ret = -2;

    if(count > 0) {
      //Pas de tableau pour ranger les paramètres
      ret = -3;

      if(params != NULL) {

        if(count > 0) {
          token = strtok(commande, " ");

          for(i = 0; i < count; i++) {
            params[i] = token;

            token = strtok(NULL, " ");
          }
        }

        ret = 1;
      }
    }
  }


  return ret;
}
