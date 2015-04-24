#include "Views.h"


/* VTE function for Error Message gui */
void* errMsgVTE(void* viewState, SDL_Event* event){
	/* create the return event */
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	/* set type of return event to NO_EVENT as default */
	returnEvent->type = NO_EVENT;
	ViewStateref menuViewState = viewState;
	Widget * currButton = menuViewState->menuButtons[0]; /* there is only one button */
	switch (event->type) {
		case (SDL_QUIT): /* if the close button (x) was pressed */
			returnEvent->type = DO_QUIT;
			break;
		case (SDL_KEYUP): /* handle key up events - for return click */
			if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_KP_ENTER){
				returnEvent->type = SELECT_CURR_BUTTON;
			}
			break;
		case (SDL_MOUSEBUTTONUP): /* handle mouse button up events on the back button */
			if (isClickEventOnButton(event, currButton, REGULAR_BUTTON)){
				returnEvent->type = SELECT_BUTTON_NUM;
				returnEvent->buttonNum = 0;
			}
			break;
		default:
			returnEvent->type = NO_EVENT;
	}

	return returnEvent;
}


