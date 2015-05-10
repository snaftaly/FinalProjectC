#include <SDL.h>
#include <SDL_video.h>
#include "../factories/GUIsFactory.h"


int main(int argc, char * argv[]){
	/* check arguments and start console mode if needed */
	if (argc == 3 && strcmp(argv[1], "-console") == 0){
		/* two argument passed and the first is -console */
		if (strcmp(argv[2], "cat") == 0){
			/* enter console mode with the cat as the player*/
			consoleMode(1);
			return isError;
		}
		else if (strcmp(argv[2], "mouse") == 0){
			/* enter console mode with the cat as the player*/
			consoleMode(0);
			return isError;
		}
		else{ /* two arguments passed, the first is -console, but the second is not cat nor mouse */
			generalErrorprint("Arguments passed to program are invalid");
			return isError;
		}

	}
	else if ((argc == 3 && strcmp(argv[1], "-console") != 0) || argc == 2 || argc > 3){
		/* two argument passed but the first is not -console, or only one arg passed, or more than
		 * two args passed */
		generalErrorprint("Arguments passed to program are invalid");
		return isError;
	}

	/* start gui mode if console mode wasn't started or there was no error */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		sdlErrorPrint("unable to init SDL");
		return isError;
	}
	atexit(SDL_Quit);

/* initialize GUI structs mapping by state ids: */
	GUI guis[11];
	guis[MAIN_MENU] = createGUIForState(MAIN_MENU);
	guis[CHOOSE_CAT] = createGUIForState(CHOOSE_CAT);
	guis[CHOOSE_MOUSE] = createGUIForState(CHOOSE_MOUSE);
	guis[CAT_SKILL] = createGUIForState(CAT_SKILL);
	guis[MOUSE_SKILL] = createGUIForState(MOUSE_SKILL);
	guis[LOAD_GAME] = createGUIForState(LOAD_GAME);
	guis[EDIT_GAME] = createGUIForState(EDIT_GAME);
	guis[WORLD_BUILDER] = createGUIForState(WORLD_BUILDER);
	guis[SAVE_WORLD] = createGUIForState(SAVE_WORLD);
	guis[PLAY_GAME] = createGUIForState(PLAY_GAME);
	guis[ERR_MSG] = createGUIForState(ERR_MSG);

	/* Starting the default/initial GUI: */
	StateId nextStateId = MAIN_MENU;

	GUI activeGUI = guis[nextStateId];
	activeGUI.start(&activeGUI, NULL);

	while (!isError && nextStateId != QUIT) {
		if (activeGUI.stateId == PLAY_GAME){ /* if we are currently playing the game */
			updateMachineMoveIfNeeded(activeGUI); /* make machine move if it is machibe turn */
			if (isError)
				break;
		}
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0) {

			/* translating the SDL event to a logical event using the view: */
			void* logicalEvent = activeGUI.viewTranslateEvent(activeGUI.viewState, &event);
			if (isError) /* PHE function may result in an error */
				break;

			/* Handling the logical event using the presenter: */
			nextStateId = activeGUI.presenterHandleEvent(activeGUI.model, activeGUI.viewState, logicalEvent);
			if (isError) /* PHE function may result in an error */
				break;

			/* if state has changed, stop the active GUI and move to the next one: */
			if (activeGUI.stateId != nextStateId) {
				if (nextStateId == QUIT) {
					break;
				}
				else {
					void* nextGuiInitData = activeGUI.stop(&activeGUI);
					if (isError) /* stop function may result in an error */
						break;
					activeGUI = guis[nextStateId];
					activeGUI.start(&activeGUI, nextGuiInitData);
				}
			}
		}
		SDL_Delay(POLLING_DELAY);
	}

	/* stop the active GUI (stop function will return NULL stop if called from here) */
	activeGUI.stop(&activeGUI);

	return isError;
}

