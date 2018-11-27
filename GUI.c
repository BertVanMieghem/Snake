#include "GUI.h"
#include "grid.h"
#include "game.h"
#include "settings.h"


#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

int score = -5;

/*
 * Dit is het venster dat getoond zal worden en waarin het speelveld weergegeven wordt.
 * Dit venster wordt aangemaakt bij het initialiseren van de GUI en wordt weer afgebroken
 * wanneer het spel eindigt.
 */
static SDL_Surface *window;

/*
 * Deze array bevat de 5 afbeeldingen die gebruikt worden tijdens het spel:
 * images[0]: de afbeelding die een snake voorstelt
 * images[1]: de afbeelding die een apple voorstelt
 * images[2]: de afbeelding die void voorstelt
 * images[3]: de afbeelding die een obstacle voorstelt
 * images[4]: de afbeelding die een Sapple voorstelt
 */
SDL_Surface* images[5];

void clear_screen() {
	SDL_FillRect(window, NULL, 0x00000000);
	SDL_Flip(window);
}


void print_score(){
	//The surfaces
	SDL_Surface *message = NULL;

	//The text to be printed
	char* text = "score";

	//The font that's going to be used
	TTF_Font *font = NULL;

	//The color of the font
	SDL_Color textColor = { 126, 255, 126 };
	font = TTF_OpenFont("digifaw.ttf", 18);


	message = TTF_RenderText_Solid(font, text, textColor);

	TTF_CloseFont(font);

	SDL_Rect posText;
	posText.x = 10;
	posText.y = 0;

	SDL_BlitSurface(message, NULL, window, &posText);
	SDL_Flip(window);
	SDL_FreeSurface(message);

}

void score_inc(int a){

	clear_screen();
	draw_grid(WIDTH, HEIGHT);

	print_score();
	score += a;

	//The surfaces
	SDL_Surface *score_nmbr = NULL;

	//The text to be printed
	char sc[64];
	sprintf(sc, "%d", score);

	//The font that's going to be used
	TTF_Font *font = NULL;

	//The color of the font
	SDL_Color textColor = { 126, 255, 126 };
	font = TTF_OpenFont("digifaw.ttf", 18);


	score_nmbr = TTF_RenderText_Solid(font, sc, textColor);

	TTF_CloseFont(font);

	SDL_Rect posText;
	posText.x = 100;
	posText.y = 0;

	SDL_BlitSurface(score_nmbr, NULL, window, &posText);
	SDL_Flip(window);
	SDL_FreeSurface(score_nmbr);
}


void pause_unpause(){

	if(gamestate == 1){
		gamestate = 0;

		SDL_Surface *pause;
		char* text = "pause";

		TTF_Font *font;
		SDL_Color textColor = { 255, 126, 126 };
		font = TTF_OpenFont("digifaw.ttf", 18);

		pause = TTF_RenderText_Solid(font, text, textColor);

		TTF_CloseFont(font);

		SDL_Rect posText;
		posText.x = 330;
		posText.y = 0;

		SDL_BlitSurface(pause, NULL, window, &posText);
		SDL_Flip(window);
		SDL_FreeSurface(pause);

	} else { score_inc(0); gamestate = 1; }
}


/*
 * Deze functie moet je zelf vervolledigen: je mag alles aan deze functie
 * (inclusief return-type en argumenten) aanpassen, indien nodig.
 */
void draw_grid(int grid_width, int grid_height) {

	for(int a = 0; a < grid_width; a++){
		for(int b = 1; b < grid_height; b++){
			SDL_Rect tile;
			tile.x = a * IMAGE_WIDTH;
			tile.y = b * IMAGE_HEIGHT;

			if((*get_cell(a, b)).is_apple == 1){
				SDL_BlitSurface(images[1], NULL, window, &tile);}
			else{
				if((*get_cell(a, b)).is_Sapple == 1){
					SDL_BlitSurface(images[4], NULL, window, &tile);}
				else{
					if((*get_cell(a, b)).state == VOID){
						SDL_BlitSurface(images[2], NULL, window, &tile);}
					else{
						if((*get_cell(a, b)).state == OBSTACLE){
							SDL_BlitSurface(images[3], NULL, window, &tile);}
						else{
							if((*get_cell(a, b)).state == SNAKE){
								SDL_BlitSurface(images[0], NULL, window, &tile);}
						}
					}
				}
			}
		}
	}

	SDL_Flip(window);
}

void stop_gui() {
	SDL_Quit();
}
/*
 * Vangt de input uit de GUI op. Deze functie is al deels geïmplementeerd, maar je moet die zelf
 * nog afwerken. Je mag opnieuw alles aanpassen aan deze functie, inclusies return-type en argumenten.
 */
void read_input() {

	SDL_Event event;

	/*
	 * Handelt alle input uit de GUI af.
	 * Telkens de speler een input aan de GUI geeft (bv. een muisklik, muis bewegen, toets indrukken enz.)
	 * wordt er een 'event' (van het type SDL_Event) gegenereerd dat hier wordt afgehandeld.
	 *
	 * Let wel op: niet al deze events zijn relevant voor jou: als de muis bv. gewoon wordt bewogen, hoef
	 * je niet te reageren op dit event. Je zal dus (eventueel) een manier moeten vinden om alle niet-relevante
	 * events weg te filteren.
	 *
	 * Zie ook https://wiki.libsdl.org/SDL_PollEvent en http://www.parallelrealities.co.uk/2011_09_01_archive.html
	 */
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					change_direction(-1);
					break;
				case SDLK_RIGHT:
					change_direction(1);
					break;
				case SDLK_UP:
					change_direction(2);
					break;
				case SDLK_DOWN:
					change_direction(-2);
					break;
				case SDLK_e:
					stop_gui();
					free_grid();
					exit(0);
					break;
				case SDLK_p:
					pause_unpause();
					break;
				case  SDLK_s:
					save_game();
					break;
				case  SDLK_l:
					load_game();
					break;
			}
				break;
		case SDL_QUIT:
			//sluit game met kruisje
			stop_gui();
			free_grid();
			exit(0);
		}
	}
}

void display_leaderboard(int first, int second, int third, int fourth, int fifth){

	clear_screen();

	TTF_Font *font = TTF_OpenFont("digifaw.ttf", 22);
	SDL_Color textColor = { 80, 255, 80 };

	// "Leaderboard"
	SDL_Surface *title_screen;
	char* title = "Leaderboard";
	title_screen = TTF_RenderText_Solid(font, title, textColor);

	// position one to five
	SDL_Surface *Sfirst;
	char Cfirst[64];
	sprintf(Cfirst, "1       %d", first);
	Sfirst = TTF_RenderText_Solid(font, Cfirst, textColor);

	SDL_Surface *Ssecond;
	char Csecond[64];
	sprintf(Csecond, "2       %d", second);
	Ssecond = TTF_RenderText_Solid(font, Csecond, textColor);

	SDL_Surface *Sthird;
	char Cthird[64];
	sprintf(Cthird, "3       %d", third);
	Sthird = TTF_RenderText_Solid(font, Cthird, textColor);

	SDL_Surface *Sfourth;
	char Cfourth[64];
	sprintf(Cfourth, "4       %d", fourth);
	Sfourth = TTF_RenderText_Solid(font, Cfourth, textColor);

	SDL_Surface *Sfifth;
	char Cfifth[64];
	sprintf(Cfifth, "5       %d", fifth);
	Sfifth = TTF_RenderText_Solid(font, Cfifth, textColor);

	TTF_CloseFont(font);

	SDL_Rect posText;
	posText.x = 115;
	posText.y = 50;

	SDL_Rect posText1;
	posText1.x = 130;
	posText1.y = 130;

	SDL_Rect posText2;
	posText2.x = 130;
	posText2.y = 160;

	SDL_Rect posText3;
	posText3.x = 130;
	posText3.y = 190;

	SDL_Rect posText4;
	posText4.x = 130;
	posText4.y = 220;

	SDL_Rect posText5;
	posText5.x = 130;
	posText5.y = 250;

	SDL_BlitSurface(title_screen, NULL, window, &posText);
	SDL_BlitSurface(Sfirst,  NULL, window, &posText1);
	SDL_BlitSurface(Ssecond, NULL, window, &posText2);
	SDL_BlitSurface(Sthird,  NULL, window, &posText3);
	SDL_BlitSurface(Sfourth, NULL, window, &posText4);
	SDL_BlitSurface(Sfifth,  NULL, window, &posText5);
	SDL_Flip(window);
	SDL_FreeSurface(title_screen);
	SDL_FreeSurface(Sfirst);
	SDL_FreeSurface(Ssecond);
	SDL_FreeSurface(Sthird);
	SDL_FreeSurface(Sfourth);
	SDL_FreeSurface(Sfifth);
}


void print_leaderboard(int score){
	char first_c[10];
	char second_c[10];
	char third_c[10];
	char fourth_c[10];
	char fifth_c[10];
	int first, second, third, fourth, fifth;

	FILE *fp = fopen("leaderboard.dat", "ab+");	// ab+ = create file if it doen't exist
	//read
	fscanf(fp, "%s %s %s %s %s", first_c, second_c, third_c, fourth_c, fifth_c);
	first = atoi(first_c);
	second = atoi(second_c);
	third = atoi(third_c);
	fourth = atoi(fourth_c);
	fifth = atoi(fifth_c);

	//erase
	fclose(fopen ("leaderboard.dat", "w+"));
    fclose(fp);

    //place your score in leaderboards
    if(score < fifth){
    	printf("You did not reach the leaderboard");}

    else { if(score > first){
    	fifth = fourth;
    	fourth = third;
    	third = second;
    	second = first;
    	first = score;

    } else { if(score > second){
    	fifth = fourth;
    	fourth = third;
      	third = second;
    	second = score;

    } else { if(score > third){
    	fifth = fourth;
    	fourth = third;
      	third = score;

    } else { if(score > fourth){
    	fifth = fourth;
    	fourth = score;

    } else { fifth = score; }
    }}}}

	//write
    FILE *fptr = fopen("leaderboard.dat", "w");
	fprintf(fptr, "%d %d %d %d %d", first, second, third, fourth, fifth);
	fclose(fptr);

	//print in console
	printf ("\nLeaderboard\n1: %d\n2: %d\n3: %d\n4: %d\n5: %d\n", first, second, third, fourth, fifth);

	//show in-game
	display_leaderboard(first, second, third, fourth, fifth);
}

/*
 * Laadt de afbeeldingen die getoond moeten worden in. Alle afbeeldingen worden samen bijgehouden
 * in een array 'images'.
 */
void initialize_figures() {
	images[0] = SDL_LoadBMP("Images/snake.bmp");
	images[1] = SDL_LoadBMP("Images/apple.bmp");
	images[2] = SDL_LoadBMP("Images/void.bmp");
	images[3] = SDL_LoadBMP("Images/obstacle.bmp");
	images[4] = SDL_LoadBMP("Images/sapple.bmp");
}

/*
 * Initialiseer het venster.
 */
void initialize_window(char *title, int grid_width, int grid_height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    int window_width = grid_width * IMAGE_WIDTH;
    int window_height = grid_height * IMAGE_HEIGHT;
    printf("Width: %d, Height: %d\n", window_width, window_height);
    window = SDL_SetVideoMode(window_width, window_height, 0, SDL_HWPALETTE | SDL_DOUBLEBUF);
    if (window == NULL) {
        printf("Couldn't set screen mode to required dimensions: %s\n", SDL_GetError());
        exit(1);
    }
    /* Set the screen title */
    SDL_WM_SetCaption(title, NULL);
}

/*
 * Initialiseer de GUI. De functie krijgt de breedte en de hoogte van het speelveld mee als argumenten.
 */
void initialize_gui(int grid_width, int grid_height) {
	initialize_window("Snake - Bert Van Mieghem", grid_width, grid_height);
	initialize_figures();
	TTF_Init();
	atexit(stop_gui);
}

int start_gui(int grid_width, int grid_height){

	initialize_gui(grid_width, grid_height);
	score_inc(5);
	while (1) {
		if(gamestate == 1){
			read_input();
			gameloop();
		} else {
			read_input();
		}
	}
	return 0;
}



