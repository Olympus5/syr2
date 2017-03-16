#ifndef AUDIO_CLIENT_H
#define AUDIO_CLIENT_H

#include <lecture.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 50000
#define BUFFER_SIZE 1024

int init_client();

int request_handling(int fd, char* filename);

#endif
