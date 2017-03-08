CC=gcc
CFLAGS=-W -Wall -I include/ -g -pedantic
EXEC=lecture

all: $(EXEC)

lecture: bin/lecture

bin/lecture: obj/lecture.o obj/audio.o
	$(CC) -o bin/lecture obj/lecture.o obj/audio.o

obj/audio.o: src/audio.c include/audio.h
	$(CC) -o obj/audio.o -c src/audio.c $(CFLAGS)

obj/lecture.o: src/lecture.c include/lecture.h
	$(CC) -o obj/lecture.o -c src/lecture.c $(CFLAGS)

.PHONY: mrproper clean

mrproper: clean
	@rm -rf bin/*
clean:
	@rm -rf obj/*.o
