CC=gcc
CFLAGS=-W -Wall -c -g -I ./include
EXEC=shell1 shell2

all : $(EXEC)

shell2: obj/shell2.o
	$(CC) -o bin/shell2 obj/shell2.o

obj/shell2.o: src/shell2.c include/shell2.h
	$(CC) -o obj/shell2.o -c src/shell2.c $(CFLAGS)

shell1: obj/shell1.o
	$(CC) -o bin/shell1 obj/shell1.o

obj/shell1.o: src/shell1.c include/shell1.h
	$(CC) -o obj/shell1.o -c src/shell1.c $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf obj/*.o

mrproper: clean
	rm -rf $(EXEC)
