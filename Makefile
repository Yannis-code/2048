# fichier Makefile permettant de compiler le 2048
CC = gcc
CFLAGS = -Wall
LDFLAGS = 
EXEC = 2048
LDLIBS = -lm

all : $(EXEC)

# Recompilation de l'éxecutable final
2048 : console.o graphique.o game.o 2048.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
	@echo "========== Compilation terminé\n========== Execution du programme compilé\n\n"
	@./$(EXEC)

# Recompilation du fichier objet game
game.o : game.c console.o graphique.o
	$(CC) $(CFLAGS) -c $<

# Recompilation du fichier objet console
console.o : console.c
	$(CC) $(CFLAGS) -c $<

# Recompilation du fichier objet graphique
graphique.o : graphique.c
	$(CC) $(CFLAGS) -c $<

# Recompilation du fichier objet 2048
2048.o : 2048.c game.o
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f *.o core