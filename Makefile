CC = gcc
CFLAGS = -Wall
LDFLAG = $(​sdl-config --cflags --libs)​
EXEC = 2048
LDLIBS = -lm -lSDL -lSDL_ttf -lSDL_gfx -lSDL_image
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all : $(EXEC)

​$(​EXEC​)​ : ​$(​OBJ​) constantes.o
	$(​CC​)​ $(CFLAGS) -o ​$@​ ​$^​ ​$(​LDFLAG​) $(LDLIBS)

2048 : 2048.o game.o console.o graphique.o constantes.o

​%​.o​:​%​.c constantes.o
	$(​CC​)​ ​$(​CFLAG​) -o ​$@​ -c ​$<​ ​$(​LDFLAG​)  $(LDLIBS)

.PHONY: clean mrproper rebuild run

clean:
	rm -rf *.o

mrproper: clean
	rm -rf ​$(​EXEC​)

rebuild: mrproper all

run: all
	./$(EXEC)