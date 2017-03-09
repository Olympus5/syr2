#ifndef LECTURE_H
#define LECTURE_H

#include <stdlib.h>
#include <stdio.h>
#include <audio.h>
#include <string.h>
#include <unistd.h>

/**
 Lance la lecture d'une piste audio
 @param file nom du fichier audio à lire
 */
void start(char* file);

#endif
