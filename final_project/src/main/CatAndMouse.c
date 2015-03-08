#include <stdio.h>
#include "../gui/MVP_utils.h"
#include "ErrorHandling.h"

/* global variables: */
int isError = 0;
int isQuit = 0;

int main(int argc, char * argv[]){
	/*
	Widget * win = create_window(WIN_W, WIN_H);
	Widget * panel1 = create_panel(20, 20, 300,300, 90,80,70);
	Widget * panel2 = create_panel(20, 500, 200, 200, 120,40,230);
	Widget * panel2_1 = create_panel(20, 20, 100, 100, 140,200,60);
	SDL_Surface *img = SDL_LoadBMP("images/window_BG.bmp");
	if (img == NULL) {
		fprintf(stderr,"ERROR: failed to load image: %s\n", SDL_GetError());
		return 1;
	}
	Widget * image = create_image(10, 10, 50, 50, img, 20, 20);
	Widget * image2 = create_image(10, 10, 20, 20, img, 40, 60);

	ListRef root = newList(win);
	ListRef child1 = addChildNode(root, panel1);
	ListRef child2 = addChildNode(root, panel2);
	ListRef child3 = addChildNode(child1, panel2_1);
	ListRef child4 = addChildNode(child3, image);
	ListRef child5 = addChildNode(child2, image2);
	treeDFS(root, addWidgetToParent);

	SDL_BlitSurface(panel1->surface, 0, win->surface, &panel1->location_rect);
	SDL_BlitSurface(panel2_1->surface, 0, panel1->surface, &panel2_1->location_rect);
	if (SDL_Flip(win->surface) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
	}

	int quit = 0;
	while (quit!=1){


		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				case (SDL_QUIT):
					quit = 1;
					break;
				default:
					break;
			}
		}

	}
	return 0;
	*/


	GUI mainMenu = createGUIForState(MAIN_MENU);
	startMainMenu(&mainMenu, NULL);
	SDL_Event e;
	int quit = 0;
	while (quit == 0){
		while (SDL_PollEvent(&e)!= 0) {
			switch (e.type) {
				case (SDL_QUIT):
					quit = 1;
					break;
				}
		}
		SDL_Delay(10);
	}
	menuStop(&mainMenu);

}

