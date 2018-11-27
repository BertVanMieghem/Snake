all: game.o grid.o main.o GUI.o
	gcc game.o grid.o main.o GUI.o -lSDL -lSDL_ttf -L./  -o main

game.o: game.c game.h settings.h
	gcc game.c -c -o game.o

grid.o: grid.c grid.h
	gcc grid.c -c -o grid.o

GUI.o: GUI.c GUI.h grid.h
	gcc GUI.c -c -o GUI.o

main.o: main.c game.h grid.h
	gcc main.c  -c -o main.o