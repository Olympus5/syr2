FLAGS = -g -Wall -I ./include/
CC = gcc

reverse: obj/reverse.o

	$(CC) -o bin/reverse obj/reverse.o

reverse.o: src/reverse.c include/reverse.h

	$(CC) -c $(FLAGS) src/reverse.c -o obj/reverse.o 



