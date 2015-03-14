#include <stdio.h>
#include "../gui/MVP_utils.h"
#include "ErrorHandling.h"

/* global variables: */
int isError = 0;
int isQuit = 0;

int main(int argc, char * argv[]){
	/*
	Widget * win = create_window(WIN_W, WIN_H);	Widget * panel1 = create_panel(20, 20, 300,300, 90,80,70);
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

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
			return 1;
		}
		atexit(SDL_Quit);
	printf("aa");

	GUI chooseCat =  createGUIForState(CHOOSE_MOUSE);


	startChooseCat(&chooseCat, NULL);
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

	menuStop(&chooseCat);

	/*
	// Initialize SDL and make sure it quits

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

// initialize GUI structs mapping by state ids:
	GUI guis[10];
	guis[MAIN_MENU] = createGUIForState(MAIN_MENU);
	guis[CHOOSE_CAT] = createGUIForState(CHOOSE_CAT);
	guis[CHOOSE_MOUSE] = createGUIForState(CHOOSE_MOUSE);
	guis[CAT_SKILL] = createGUIForState(CAT_SKILL);
	guis[MOUSE_SKILL] = createGUIForState(MOUSE_SKILL);

	// Starting the default/initial GUI:
	StateId nextStateId = MAIN_MENU;

	GUI activeGUI = guis[nextStateId];
	activeGUI.start(&activeGUI, NULL);

	while (!isError && nextStateId != QUIT) {
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0) {

			// translating the SDL event to a logical event using the view:
			void* logicalEvent = activeGUI.viewTranslateEvent(activeGUI.viewState, &event);

			// Handling the logical event using the presenter:
			nextStateId = activeGUI.presenterHandleEvent(activeGUI.model, activeGUI.viewState, logicalEvent);

			// if state has changed, stop the active GUI and move to the next one:
			if (activeGUI.stateId != nextStateId) {
				if (nextStateId == QUIT) {
					break;
				}
				else {
					void* nextGuiInitData = activeGUI.stop(&activeGUI);

					activeGUI = guis[nextStateId];
					activeGUI.start(&activeGUI, nextGuiInitData);
				}
			}
		}
		SDL_Delay(POLLING_DELAY);
	}

	// API may be extended with a "provideInitData" flag or something similar:
	activeGUI.stop(&activeGUI);

	return isError;
	*/

}

