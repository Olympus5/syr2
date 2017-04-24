#include <audioclient.h>

static void purger() {
  int c;

  while((c = getchar()) != '\n' && c != EOF);
}

static void clean(char* string) {
  char* p = strchr(string, '\n');

  if(p != NULL) {
    *p = '\0';
  } else {
    purger();
  }
}

/**
 * Pour lancer une requête au serveur audio:
 *      bin/audioclient <nom_fichier>
 */
int main(int argc, char* argv[]) {
  int fd, error;
  char* file = argv[1];
  char* extension;

  /* Vérification du fichier passé en paramètre */
  if(argc <= 1) {
    fprintf(stderr, "Erreur, veuillez indiquer le fichier audio à lire\n");
    exit(1);
  }
  /* Vérification du fichier passé est bien un .wav */
  extension = strrchr(file, '.');

  if(strcmp(extension, ".wav")) {
    fprintf(stderr, "Erreur, le fichier n'est pas de type WAV\n");
    exit(1);
  }

  /* Lancement du client */
  fd = error = init_client();

  if(error < 0) {
    perror("Erreur lors de l'initialisation du client\n");
    close(fd);
    exit(1);
  }

  /* Envois de la requête */
  error = request_handling(fd, file);

  if(error < 0) {
    perror("Erreur lors de la requête client");
    close(fd);
    exit(1);
  }

  /* Fermeture du client */
  close(fd);

  return 0;
}

int init_client() {
  int fd, error;

  printf("============= CLIENT =============\n\n\n");

  /* Initialise la socket */
  fd = socket(AF_INET, SOCK_DGRAM, 0);

  error = fd;

  if(error < 0) {
    return error;
  }

  return fd;
}

void init_request(char* audio_metadata, char* filename, char* choix, fd_set* readfds, struct timeval *tv, struct sockaddr_in *dest) {
  FD_ZERO(readfds);

  (*tv).tv_sec = 10;
  (*tv).tv_usec = 0;

  (*dest).sin_family = AF_INET;
  (*dest).sin_port = htons(PORT);
  (*dest).sin_addr.s_addr = htonl(INADDR_ANY);

  /*Choix du filtre*/
  do {
    printf("Choississez le filtre à appliquer: (entre 1 et 5)\n1.Stereo vers mono\n2.Augmentation du volume\n3.Augmentation de la fréquence\n4.Diminution de la fréquence\n5.Aucun\n\n");

    fgets(choix, sizeof(choix), stdin);
    clean(choix);
  } while(atoi(choix) > 5 || atoi(choix) < 0);

  strcpy(audio_metadata, filename);
}

int send_metadata(int fd, char* audio_metadata, fd_set* readfds, struct timeval *tv, struct sockaddr_in *dest) {
  int error;

  error = sendto(fd, audio_metadata, BUFFER_SIZE, 0, (struct sockaddr*) dest, sizeof(struct sockaddr_in));

  if(error < 0) {
    return error;
  }

  FD_SET(fd, readfds);

  error = select(fd+1, readfds, NULL, NULL, tv);

  if(error < 0) {
    return error;
  }

  if(error == 0) {
    return 1;
  }

  if(FD_ISSET(fd, readfds)) {
    /* Attente de la réponse du serveur et traitement de la réponse*/
    error = recvfrom(fd, audio_metadata, BUFFER_SIZE, 0, NULL, 0);
  }

  FD_CLR(fd, readfds);

  return 0;
}

int init_write(char* audio_metadata, char* choix, char* sound, int* sample_rate, int* sample_size, int* channels) {
  int error;

  /*Initialisation des métadonnées du fichier audio*/
  *sample_rate = atoi(strtok(audio_metadata, ";"));
  *sample_size = atoi(strtok(NULL, ";"));
  *channels = atoi(strtok(NULL, ";"));

  *sound = '1';

  switch(atoi(choix)) {
    case 1:/*Stéréo vers mono*/
      *channels = 1;
    break;

    case 2:/*Augmentation du volume*/
      printf("Niveau du volume souhaité: ");
      fgets(sound, sizeof(sound), stdin);
      if(atoi(sound) < 1) *sound = '1';
    break;

    case 3:/*Augmentation de la fréquence*/
      *sample_rate = (*sample_rate) * 2;
    break;

    case 4:/*Diminution de la fréquence*/
      *sample_rate = (*sample_rate) / 2;
    break;
  }

  error = aud_writeinit(*sample_rate, *sample_size, *channels);

  if(error < 0) {
    return error;
  }

  return error;
}

int request_handling(int fd, char* filename) {
  int i;
  int error;
  int sample_rate, sample_size, channels;/*Metadonnées fichier audio*/
  int fd_write;
  struct sockaddr_in dest;/*En-tête du message à envoyer au serveur audio*/
  char audio_metadata[BUFFER_SIZE];/*Buffers de reception des metadonnées*/
  char buf[BUFFER_SIZE];/*Buffers de reception des données*/
  struct timeval tv;/*Timeout du client*/
  fd_set readfds;/*Liste contenant le descripteur de fichier du client*/
  int ttl;/*Durée de vie de la connexion quand il y a perte de connexion avec le serveur*/
  char choix[2];/*Choix du filtre*/
  char sound[100];

  init_request(audio_metadata, filename, choix, &readfds, &tv, &dest);

  ttl = 64;

  /*Permet de garantir l'envoie et la reception du premier datagramme*/
  do {
    error = send_metadata(fd, audio_metadata, &readfds, &tv, &dest);
    ttl--;
    if(error == 1) printf("Timeout écoulé, la requête n'a pas pu être pris en charge. Nombre d'essais restant %d.\n", ttl);
  } while(error == 1 && ttl > 0);

  if(ttl <= 0) {
    printf("Serveur injoignable. Veuillez ressayer.");
    return 0;
  }

  if(error < 0) {
    return error;
  }

  if(error == 1) {
    return error;
  }

  printf("%s\n", audio_metadata);

  fd_write = error = init_write(audio_metadata, choix, sound, &sample_rate, &sample_size, &channels);

  if(error < 0) {
    return error;
  }

  strcpy(buf, "XXX");
  ttl = 64;

  while((strncmp("FIN", buf, (size_t) 3) != 0) && (ttl > 0)) {
    bzero(buf, (size_t)BUFFER_SIZE);

    error = sendto(fd, " ", 1, 0, (struct sockaddr*) &dest, sizeof(struct sockaddr_in));

    if(error < 0) {
      return error;
    }

    FD_SET(fd, &readfds);

    tv.tv_sec = 2;
    tv.tv_usec = 0;

    error = select(fd+1, &readfds, NULL, NULL, &tv);

    if(error < 0) {
      return error;
    }

    if(error == 0) {
      ttl--;
      printf("Perte de la connexion veuillez patienter...\n");
      continue;
    }

    if(FD_ISSET(fd, &readfds)) {
      ttl = 64;
      error = recvfrom(fd, buf, BUFFER_SIZE, 0, NULL, 0);

      if(error < 0) {
        return error;
      }

      /*Modification du volume
      if(strncmp("FIN", buf, (size_t) 3) != 0) {
        sprintf(buf, "%d", (atoi(buf) << sound));
      }*/

      printf("%s\n", buf);

      char* p = buf;

      for (i = 0; i == sizeof(buf); i++){
        char* subbuf[sample_size];
        memcpy(subbuf, p, sample_size);
        sprintf(*subbuf, "%d", (atoi(*subbuf) * (atoi(sound))) );
        memcpy(p, subbuf, sample_size);
        p += sample_size;
        memcpy(buf+(sample_size*i), subbuf, sample_size);
      }

      error = write(fd_write, buf, (size_t)sample_size);

      if(error < 0) {
        return error;
      }
    }
  }

  if(error < 0) {
    return error;
  }

  if(ttl == 0) {
    printf("Connexion perdu. Fin de la connexion.\n");
  }

  return 0;
}
