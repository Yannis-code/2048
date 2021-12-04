# fichier Makefile permettant de compiler le 2048
CC = gcc
CFLAGS = -Wall
LDFLAG = $(​sdl-config --cflags --libs)​
EXEC = 2048
LDLIBS = -lm -lSDL -lSDL_ttf -lSDL_gfx
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all : $(EXEC)

# Compilation de l'éxecutable final
​$(​EXEC​)​ : ​$(​OBJ​)
	$(​CC​)​ $(CFLAGS) -o ​$@​ ​$^​ ​$(​LDFLAG​) $(LDLIBS)

2048 : 2048.o game.o console.o graphique.o

​%​.o​:​%​.c
	$(​CC​)​ ​$(​CFLAG​) -o ​$@​ -c ​$<​ ​$(​LDFLAG​)  $(LDLIBS)

.PHONY: clean mrproper rebuild

clean:
	rm -rf *.o

mrproper: clean
	rm -rf ​$(​EXEC​)

rebuild: mrproper all

​run​: all 
	./jeu