CC=gcc
CFLAGS=-W -Wall -c -I ./include
EXEC=shell1

all : $(EXEC)

shell1: obj/shell1.o
	$(CC) -o bin/shell1 obj/shell1.o

obj/shell1.o: src/shell1.c include/shell1.h
	$(CC) -o obj/shell1.o -c src/shell1.c $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
