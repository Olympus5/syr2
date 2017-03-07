CC=gcc
CFLAGS=-W -Wall -I include/ -g
EXEC=tp4 nettoie

all: $(EXEC)

nettoie: bin/nettoie

tp4: bin/tp4

bin/nettoie: obj/nettoie.o
	$(CC) -o bin/nettoie obj/nettoie.o

bin/tp4: obj/tp4.o
	$(CC) -o bin/tp4 obj/tp4.o

obj/nettoie.o: src/nettoie.c include/nettoie.h
	$(CC) -o obj/nettoie.o -c src/nettoie.c $(CFLAGS)

obj/tp4.o: src/tp4.c
	$(CC) -o obj/tp4.o -c src/tp4.c $(CFLAGS)

.PHONY: mrproper clean

mrproper: clean
	rm -rf bin/*

clean:
	rm -rf obj/*
