#include "grid.h"
#include "settings.h"

/*
 * Stelt het twee-dimensionale veld voor. grid is een tweedimensionale array van pointers naar Cellen.
 * Jullie moeten deze variabele zelf niet gebruiken: in de plaats kan je gebruik maken van de functie get_cell hieronder.
 */
static struct Cell*** grid;

struct Cell*** allocate_grid(int grid_width, int grid_height){
	int a, b;
	struct Cell*** ptr;
	ptr = malloc(sizeof (struct Cell*) * grid_width);
	for(a = 0; a < grid_width; a++){
		ptr[a] = malloc(sizeof (struct Cell*) * grid_height);
		for(b = 0; b < grid_height; b++){
			ptr[a][b] = malloc(sizeof(struct Cell*));
			ptr[a][b]->is_apple = 0;
			ptr[a][b]->is_Sapple = 0;
			ptr[a][b]->state = VOID;
		}
	}
	return ptr;
}

static struct Snake** snake;

struct Snake** allocate_snake(int length){
	struct Snake** ptr;
	ptr = malloc(sizeof(struct Snake*) * length);
	for(int a = 0; a < length; a++){
		ptr[a] = malloc(sizeof (struct Snake*) * length);
		ptr[a]->x = 0;
		ptr[a]->y = 0;
		ptr[a]->state = VOID;
	}
	return ptr;
}

/*
 * Geeft een pointer terug naar de Cell die zich bevindt op positie (x,y) in het veld.
 * Let op: deze functie gaat ervan uit dat het coördinaat dat werd meegegeven correct is, en dus niet buiten
 * de grenzen van het veld valt.
 */
struct Cell *get_cell(int x, int y) {
	return grid[x][y];
}

struct Snake *get_snek(int idx){
	return snake[idx];
}

/*
 * Stelt een positie voor: elke positie heeft een x- en een y-coordinaat.
 */
struct Coordinate {
	int x, y;
};


void free_grid(){
	int counter = 0;

	while(counter != max_snake_length){
		struct Snake *piece = get_snek(counter);
		free(piece);
		counter++;
	}
	for(int a = 0; a < WIDTH; a++){
		for(int b = 0; b < HEIGHT; b++){
			struct Cell *cell = get_cell(a, b);
			free(cell);
		}
	}
	free(grid);
	free(snake);
}
}

void generate_random_apple(int s){		// 1 = special apple,	0 = normal apple
	int x = rand() % WIDTH;
	int y = rand() % HEIGHT;
	printf("random apple at: (%d, %d)?\n", x, y);

	struct Cell cell = *get_cell(x, y);

	if (s == 0){
		if(cell.state == OBSTACLE || cell.state == SNAKE || y == 0){
			generate_random_apple(0);
		} else {
			printf("placed apple\n");
			(*get_cell(x, y)).is_apple = 1;
			appleX = x;
			appleY = y;
		}
	}
	if (s == 1){
		if(cell.state == OBSTACLE || cell.state == SNAKE || y == 0){
			generate_random_apple(1);
		} else {
			printf("placed Special apple\n");
			(*get_cell(x, y)).is_Sapple = 1;
			SappleX = x;
			SappleY = y;
		}
	}
}

void initialize_grid(int grid_width, int grid_height) {
	grid = allocate_grid(grid_width, grid_height);
	snake = allocate_snake(200);
}
