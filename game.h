#ifndef GAME_H_
#define GAME_H_

#include <stdio.h>

#include "grid.h"

/*
 * Deze functie wordt in het begin, eens het veld geïnitialiseerd is, aangeroepen.
 * Eens deze functie is aangeroepen moet het spel in beurten verdergaan tot de speler gewonnen of verloren heeft.
 * Na elke beurt moet het hele veld automatisch geprint worden, via de functie print_grid uit printer.h,
 * en moet er geprint worden hoeveel vlaggen nog beschikbaar zijn voor de speler.
 */
void run_game(int screen_width, int screen_height);

int WIDTH;
int HEIGHT;

void place__walls();

void change_direction(int dir);

void move_snake();

void initialize_snake(int x, int y);

void add_snakepiece(int x, int y);

int gamestate;

void save_game();

void load_game();

void gameloop();

#endif /* GAME_H_ */
