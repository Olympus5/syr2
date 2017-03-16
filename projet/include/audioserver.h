#ifndef AUDIO_SERVER_H
#define AUDIO_SERVER_H

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

int start_server();

int request_handling();

int lectura_expendiente();

#endif
