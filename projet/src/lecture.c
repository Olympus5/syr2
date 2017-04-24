#include <lecture.h>

int main(int argc, char* argv[]) {
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

  start(file);

  return 0;
}

void start(char* file) {
  int sample_rate = 44100;
  int sample_size = 16;
  int channels = 2;
  int fd_read, fd_write, fd_write_2, fd_write_3;
  char buf[sample_size];
  ssize_t error;

  fd_write = aud_writeinit(sample_rate, sample_size, channels);

  fd_write_2 = aud_writeinit(sample_rate-3000, sample_size, channels);

  if(fd_write < 0) {
    fprintf(stderr, "Erreur, ouverture écriture audio");
    exit(1);
  }

  if(fd_write_2 < 0) {
    fprintf(stderr, "Erreur, ouverture écriture audio");
    exit(1);
  }

  fd_read = aud_readinit(file, &sample_rate, &sample_size, &channels);

  if(fd_read < 0) {
    fprintf(stderr, "Erreur, ouverture lecture audio");
    exit(1);
  }

  while((ssize_t)sample_size <= (error = read(fd_read, buf, (size_t)sample_size))) {
    error = write(fd_write, buf, (size_t)sample_size);
    if(error < 0) {
      perror(NULL);
      exit(1);
    }

    error = write(fd_write_2, buf, (size_t)sample_size);
    if(error < 0) {
      perror(NULL);
      exit(1);
    }

    bzero(buf, sample_size);
  }
}
