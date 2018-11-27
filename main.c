#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "grid.h"
#include "GUI.h"
#include "settings.h"


/*
 * Initialiseert eerst het veld, via de functie initialize_grid uit grid.c, en roept daarna de functie
 * run_game uit game.c aan om het spel effectief te starten.
 */
int main(int argc, char *argv[]) {	// Width and height must be at least 20
	/*
	 * Naar deze statement hoef je nu niet te kijken.
	 */
	srand(time(NULL));

	if(argc != 3){
		WIDTH = 20;		// Default width/height
		HEIGHT = 20;
	} else {
		WIDTH = atoi(argv[1]);
		HEIGHT = atoi(argv[2]);
	}
	/*
	 * Initialiseert het speelveld met als dimensies de breedte en de hoogte die gedefinieerd zijn in settings.h,
	 * en met het aantal mijnen gelijk aan NR_OF_MINES uit settings.h.
	 */
	initialize_grid(WIDTH, HEIGHT);

	initialize_snake(5, 10);

	place_walls();

	generate_random_apple(0);

	start_gui(WIDTH, HEIGHT);

}
