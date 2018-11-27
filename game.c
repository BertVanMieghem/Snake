#include "settings.h"
#include "grid.h"
#include "game.h"
#include "GUI.h"

/****************************************************************************************************
 *								  		TE IMPLEMENTEREN FUNCTIES:						  			*
 ****************************************************************************************************/
int snake_idx = 0;		// # of snake pieces currently on screen	last piece = biggest idx, head = 0

int gamestate = 1;		// 1 = playing, 0 = paused/gameover
int gamecounter = 0;	// counts each cycle
int step_counter = 0;	// counts each gameloop

int Sapple_timer = 25;	// the snake can move 25 times before the special apple disappears
int Sapple_state = 0;	// 1 = special apple on screen

void initialize_snake(int x, int y){
	add_snakepiece(x, y);
	add_snakepiece(x-1, y);
	add_snakepiece(x-2, y);
	add_snakepiece(x-3, y);
}

void add_snakepiece(int x, int y){
	(*get_cell(x, y)).state = SNAKE;
	(*get_snek(snake_idx)).x = x;
	(*get_snek(snake_idx)).y = y;
	(*get_snek(snake_idx)).state = SNAKE;
	snake_idx += 1;
}

void remove_snakepiece(int idx){
	int x, y;
	x = (*get_snek(idx)).x;
	y = (*get_snek(idx)).y;
	(*get_cell(x, y)).state = VOID;
	(*get_snek(idx)).state = VOID;
	snake_idx -= 1;
}


int direction = 1;

void change_direction(int new_dir){		// -1=left	1=right		2=up	-2=down
	if(direction + new_dir == 0){	   //	if the snake is going left, you can't go right.
		return;						  // 	if the snake is going up,   you can't go down. And other way around
	} else {
		direction = new_dir;
	}
}

void grow_snake(){
	struct Snake last = (*get_snek(snake_idx-1));	//position of last snake piece
	int tailX = last.x;
	int tailY = last.y;

	// Place another snake piece on the last piece, this snake piece will follow the previously last snake piece
	// and thus become the last snake piece
	add_snakepiece(tailX, tailY);
}

void eat_apple(int x, int y, int s){
	if (s == 0){	// normal apple
		(*get_cell(x, y)).is_apple = 0;
		(*get_cell(x, y)).state = VOID;
		grow_snake();
		score_inc(5);
		generate_random_apple(0);
	}
	if (s == 1){	// special apple
		(*get_cell(x, y)).is_Sapple = 0;
		(*get_cell(x, y)).state = VOID;
		grow_snake();
		int a = Sapple_timer * 3;	//the earlier you eat the special apple, the more points you get
		score_inc(a);
		printf("score increased with %d\n", a);
		Sapple_timer = 20;
		Sapple_state = 0;
	}
}

void check_apple(int s){		// 0 = normal apple ,	1 = special apple
	struct Snake first = (*get_snek(0));	//position of first snakepiece
	int headX = first.x;
	int headY = first.y;

	if (s == 0){
		if((*get_cell(headX, headY)).is_apple == 1){
			eat_apple(headX, headY, 0);
		}
	}
	if (s == 1){
		if((*get_cell(headX, headY)).is_Sapple == 1){
			eat_apple(headX, headY, 1);
		}
	}
}

void chapple(int x, int y){		// Handles the special apple's timed disappearance
	if(Sapple_timer == 0){
		(*get_cell(x, y)).is_Sapple = 0;
		(*get_cell(x, y)).state = VOID;
		Sapple_state = 0;
		Sapple_timer = 20;
	} else {
		Sapple_timer -= 1;
		check_apple(1);
	}
}

void special_apple(){
	generate_random_apple(1);
	Sapple_state = 1;
}

int check_teleport(int headX, int headY){		// returns 1 if the snake has been teleported
											   //  returns 0 if the snake just needs to move 1 spot over
	if (direction == -1 && headX == 0){			//left wall
		(*get_snek(0)).x = WIDTH -1;
		return 1;
	}
	if (direction == 1 && headX == WIDTH -1){	//right wall
		(*get_snek(0)).x = 0;
		return 1;
	}
	if (direction == 2 && headY == 1){			//upper wall
		(*get_snek(0)).y = HEIGHT -1;
		return 1;
	}
	if (direction == -2 && headY == HEIGHT -1){	//lower wall
		(*get_snek(0)).y = 0;
		return 1;
	}
	return 0;
}

void move_snake(){
	struct Snake first = (*get_snek(0));	//position of first snakepiece
	int headX = first.x;
	int headY = first.y;

	struct Snake last = (*get_snek(snake_idx-1));

	if (check_teleport(headX, headY) == 0){
	if (direction == -1) {	(*get_snek(0)).x -= 1;	}	// left
	if (direction ==  1) {	(*get_snek(0)).x += 1;	}	// right
	if (direction ==  2) {	(*get_snek(0)).y -= 1;	}	// up
	if (direction == -2) {	(*get_snek(0)).y += 1;	}}	// down

	//make the tail follow the head
	for(int i = snake_idx-1; i > 1; i--){
		(*get_snek(i)).x = (*get_snek(i-1)).x;
		(*get_snek(i)).y = (*get_snek(i-1)).y;
		}

	(*get_snek(1)).x = headX;
	(*get_snek(1)).y = headY;

	(*get_cell(last.x, last.y)).state = VOID;

	//eat apple?
	check_apple(0);

}

void draw_snake(){
	for(int r = 0; r < snake_idx; r++){
		int x = (*get_snek(r)).x;
		int y = (*get_snek(r)).y;

		if((*get_cell(x, y)).state == OBSTACLE){	// check collision with obstacle before overwriting the cell's state
			printf("Obstacle collision\n");
		} else {
			(*get_cell(x, y)).state = SNAKE;
			(*get_snek(r)).state = SNAKE;
		}
	}
}

void check_collision(){
	struct Snake first = (*get_snek(0));	//position of first snakepiece
	int headX = first.x;
	int headY = first.y;

	for(int i = 1; i < snake_idx; i++){
		if(headX == (*get_snek(i)).x && headY == (*get_snek(i)).y){
			printf("snake body collision \n");
			gamestate = 0;
			print_leaderboard(score);
		}
	}

	if((*get_cell(headX, headY)).state == OBSTACLE){
		gamestate = 0;
		print_leaderboard(score);
	}
}

void place_obstacle(int x, int y){
	(*get_cell(x, y)).state = OBSTACLE;
}

void place_walls(){

	for(int i = 0; i < WIDTH; i++){
		if(i > WIDTH / 3 && i < WIDTH * 2/3)
			continue;
		place_obstacle(i, 1);
		place_obstacle(i, HEIGHT-1);
	}
	for(int i = 0; i < HEIGHT; i++){
		if(i > HEIGHT / 3 + 1 && i < HEIGHT * 2/3)
			continue;
		place_obstacle(0, i);
		place_obstacle(WIDTH-1, i);
	}
}


void save_game(){
	FILE *fp = fopen("gamesave.dat", "w");

	fprintf(fp, "%d %d %d %d %d %d %d %d %d %d \n", score, Sapple_state, Sapple_timer, step_counter, SappleX, SappleY, appleX, appleY, direction, snake_idx);
	int ctr =  0;
	while(ctr != snake_idx){
		fprintf(fp, "%d %d ", (*get_snek(ctr)).x, (*get_snek(ctr)).y);
		printf("(%d, %d)\n", (*get_snek(ctr)).x, (*get_snek(ctr)).y);
		ctr++;
	}
	printf("game saved!\n");
	fclose(fp);
}

void load_game(){
	FILE *fp = fopen("gamesave.dat", "r");
	int sc, Ss, St, Sc, sax, say, ax, ay, dir, indx;
	fscanf(fp, "%d %d %d %d %d %d %d %d %d %d ",&sc, &Ss, &St, &Sc, &sax, &say, &ax, &ay, &dir, &indx);

	// load score + Special apple variables
	direction = dir;
	score = sc;
	score_inc(0); 	//draw loaded score
	Sapple_state = Ss;
	Sapple_timer = St;
	step_counter = Sc;


	//load apple
	(*get_cell(appleX, appleY)).is_apple = 0;
	appleX = ax;
	appleY = ay;
	(*get_cell(appleX, appleY)).is_apple = 1;

	//load Sapple
	(*get_cell(SappleX, SappleY)).is_Sapple = 0;
	SappleX = sax;
	SappleY = say;
	if(Sapple_state == 1){ (*get_cell(SappleX, SappleY)).is_Sapple = 1; }

	//load snake
	int idx = 0;
	int x;
	int y;
	while (fscanf(fp, "%d %d ", &x, &y) != EOF) {
		(*get_cell((*get_snek(idx)).x, (*get_snek(idx)).y)).state = VOID;
		(*get_snek(idx)).state = VOID;
		(*get_snek(idx)).x = x;
		(*get_snek(idx)).y = y;
		idx++;
	}
	if(idx < snake_idx){
		for(int a = idx; a < snake_idx; a++){
			(*get_cell((*get_snek(a)).x, (*get_snek(a)).y)).state = VOID;
			(*get_snek(a)).state = VOID;
		}
	}
	snake_idx = indx;
	fclose(fp);
	printf("Game loaded!\n");
}

void gameloop(){
	if (gamecounter == 120000 - score * 80){	// higher score -> faster snake
		move_snake();
		draw_snake();
		check_collision();
		if (Sapple_state == 0){ step_counter += 1; }
		if (Sapple_state == 1){ chapple(SappleX, SappleY); }
		if (gamestate == 1){ draw_grid(WIDTH, HEIGHT); }
		gamecounter = 0;
	} else {
		gamecounter += 1;
	}
	if (step_counter == 50){
		special_apple();
		step_counter = 0;
	}

}
