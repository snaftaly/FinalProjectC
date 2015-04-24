#include "Views.h"


/* VTE function for play game gui */
void* playGameVTE(void* viewState, SDL_Event * event){
	/* create the return event */
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	/* set type of return event to NO_EVENT as default */
	returnEvent->type = NO_EVENT;
	ViewStateref pgViewState = viewState;
	SDLKey key;
	switch (event->type) {
		case (SDL_QUIT): /* if the close button (x) was pressed */
			returnEvent->type = DO_QUIT;
			break;
		case (SDL_KEYUP):
			key = event->key.keysym.sym;
			if (key == SDLK_SPACE || key == SDLK_F1 || key == SDLK_F2 || key == SDLK_F3 || key == SDLK_F4 ||
					key == SDLK_ESCAPE){
				/* handle case where a button key was pressed */
				returnEvent->type = SELECT_BUTTON_NUM;
				returnEvent->buttonNum = getPGButtonNum(key);
			}
			else
				/* check and handle arrow key events */
				handleThreePartLayoutArrowKey(key, returnEvent);
			break;
		case (SDL_MOUSEBUTTONUP):/* handle mouse button up events */
			handleThreePartLayoutMouseSelect(event, returnEvent, pgViewState->menuButtons, PG_NUM_BUTTONS);
			break;
		default:
			returnEvent->type = NO_EVENT;
	}
	return returnEvent;
}


