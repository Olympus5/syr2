CC=gcc
CFLAGS=-W -Wall -I include/ -g -pedantic
EXEC=lecture serveur client audioserver audioclient

all: $(EXEC)

lecture: bin/lecture

serveur: bin/serveur

client: bin/client

audioserver: bin/audioserver

audioclient: bin/audioclient

bin/lecture: obj/lecture.o obj/audio.o
	$(CC) -o bin/lecture obj/lecture.o obj/audio.o

bin/serveur: obj/serveur.o
	$(CC) -o bin/serveur obj/serveur.o

bin/client: obj/client.o
	$(CC) -o bin/client obj/client.o

bin/audioserver: obj/audioserver.o obj/audio.o
	$(CC) -o bin/audioserver obj/audioserver.o obj/audio.o

bin/audioclient: obj/audioclient.o obj/audio.o
	$(CC) -o bin/audioclient obj/audioclient.o obj/audio.o

obj/audio.o: src/audio.c include/audio.h
	$(CC) -o obj/audio.o -c src/audio.c $(CFLAGS)

obj/lecture.o: src/lecture.c include/lecture.h
	$(CC) -o obj/lecture.o -c src/lecture.c $(CFLAGS)

obj/serveur.o: src/serveur.c include/serveur.h
	$(CC) -o obj/serveur.o -c src/serveur.c $(CFLAGS)

obj/client.o: src/client.c include/client.h
	$(CC) -o obj/client.o -c src/client.c $(CFLAGS)

obj/audioserver.o: src/audioserver.c include/audioserver.h
	$(CC) -o obj/audioserver.o -c src/audioserver.c $(CFLAGS)

obj/audioclient.o: src/audioclient.c include/audioclient.h
	$(CC) -o obj/audioclient.o -c src/audioclient.c $(CFLAGS)

.PHONY: mrproper clean

mrproper: clean
	@rm -rf bin/*
clean:
	@rm -rf obj/*.o
