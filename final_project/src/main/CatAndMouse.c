#include <stdio.h>
#include "../gui/MVP_utils.h"
#include "ErrorHandling.h"

/* global variables: */
int isError = 0;
int isQuit = 0;

int main(int argc, char * argv[]){

	char ** grid = NULL;
	int isCatFirst;
	int numTurns= 0 ;
	int worldNum = 6;
	int i, j;

	/*grid = (char **)malloc(7*sizeof(char *));
	if (grid == NULL){
		perrorPrint("malloc");
	}*/
	/*grid = initGrid();
	//initColumns(7, 7, grid);
	setEmptyGrid(grid);*/

	grid = initGameData(worldNum, &numTurns, &isCatFirst);



	for (int i = 0; i<7; i++){
		for (int j = 0; j<7; j++){
			printf("%c", grid[i][j]);
			if (j==6)
				printf("\n");
		}
	}



	/*if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);
	/*
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
*/

// initialize GUI structs mapping by state ids:
	/*GUI guis[10];
	guis[MAIN_MENU] = createGUIForState(MAIN_MENU);
	guis[CHOOSE_CAT] = createGUIForState(CHOOSE_CAT);
	guis[CHOOSE_MOUSE] = createGUIForState(CHOOSE_MOUSE);
	guis[CAT_SKILL] = createGUIForState(CAT_SKILL);
	guis[MOUSE_SKILL] = createGUIForState(MOUSE_SKILL);
	guis[LOAD_GAME] = createGUIForState(LOAD_GAME);
	guis[EDIT_GAME] = createGUIForState(EDIT_GAME);

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

