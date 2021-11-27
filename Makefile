# fichier Makefile permettant de compiler prog
CC = gcc
CFLAGS = -Wall
LDLIBS = -lm

all : 2048

2048 : 2048.o console.o game.o graphique.o
	$(CC) $(CFLAGS) -o 2048 2048.o console.o game.o graphique.o $(LDLIBS)

2048.o : 2048.c constantes.h
	$(CC) $(CFLAGS) -c 2048.c

console.o : console.c console.h constantes.h
	$(CC) $(CFLAGS) -c console.c

game.o : game.c game.h console.c console.h constantes.h
	$(CC) $(CFLAGS) -c game.c

graphique.o : graphique.c graphique.h
	$(CC) $(CFLAGS) -c graphique.c

clean :
	rm -f *.o core