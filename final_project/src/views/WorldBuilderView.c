#include "Views.h"
#include "../services/MVPutils.h"

/* worldBuilderVTE is the VTE function for the world builder gui */
void* worldBuilderVTE(void* viewState, SDL_Event* event){
	/* create the return event */
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	/* set type of return event to NO_EVENT as default */
	returnEvent->type = NO_EVENT;
	ViewStateref wbViewState = viewState;
	SDLKey key;
	switch (event->type) {
		case (SDL_QUIT):
			returnEvent->type = DO_QUIT; /* if the close button (x) was pressed */
			break;
		case (SDL_KEYUP):/* handle key up events */
			key = event->key.keysym.sym;
			if (key == SDLK_s || key == SDLK_F1 || key == SDLK_ESCAPE ||
					key == SDLK_m || key == SDLK_c || key == SDLK_p || key == SDLK_w || key == SDLK_SPACE){
				/* handle case where a button key was pressed */
				returnEvent->type = SELECT_BUTTON_NUM;
				returnEvent->buttonNum = getWBButtonNum(key);
			}
			else
				/* check and handle arrow key events */
				handleThreePartLayoutArrowKey(key, returnEvent);
			break;
		case (SDL_MOUSEBUTTONUP): /* handle mouse button up events */
			handleThreePartLayoutMouseSelect(event, returnEvent, wbViewState->buttonsArr, WB_NUM_BUTTONS);
			break;
		default:
			returnEvent->type = NO_EVENT;
	}
	return returnEvent;
}
