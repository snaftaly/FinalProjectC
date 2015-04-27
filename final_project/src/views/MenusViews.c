#include "Views.h"
#include "../services/MVPutils.h"

/**** menus viewTranslateEvent (VTE) functions: ****/

/* simpleMenuVte is a function that is used as a VTE for the menus which have only
 * regular buttons (the menus that don't have value selection button */
void* simpleMenuVTE(void* viewState, SDL_Event* event, int numOfButtons){
	/* create the return event */
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}

	/* set type of return event to NO_EVENT as default */
	returnEvent->type = NO_EVENT;
	ViewStateref menuViewState = viewState;
	/* switch over event types */
	switch (event->type) {
		case (SDL_QUIT): /* if the close button (x) was pressed */
			returnEvent->type = DO_QUIT;
			break;
		case (SDL_KEYUP): /* handle key up events */
			if (event->key.keysym.sym == SDLK_TAB){
				/* if tab was pressed, event is mark next button */
				returnEvent->type = MARK_NEXT_BUTTON;
			}
			if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_KP_ENTER){
				/* if return button pressed, even is select curr button */
				returnEvent->type = SELECT_CURR_BUTTON;
			}
			break;
		case (SDL_MOUSEBUTTONUP): /* handle mouse button up events */
			for (int i = 0; i< numOfButtons; i++){ /* go over each button and check if mouse clicked on it */
				Widget * currButton = menuViewState->buttonsArr[i];
				if (isClickEventOnButton(event, currButton, REGULAR_BUTTON)){ /* check if button was clicked*/
					returnEvent->type = SELECT_BUTTON_NUM; /* set event type to select button with
					 	 	 	 	 	 	 	 	 	 	  the number of the button*/
					returnEvent->buttonNum = i;
					break;
				}
			}
			break;
		default:
			returnEvent->type = NO_EVENT;
	}
	return returnEvent;
}

/* complexMenuVte is a function that is used as a VTE for the menus which have one value selection
 * button and two other regular buttons */
void* complexMenuVTE(void* viewState, SDL_Event* event){
	/* create the return event */
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	int numOfButtons = COMMON_MENU_NUM_BUTTONS;
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	ViewStateref menuViewState = viewState;
	/* set type of return event to NO_EVENT as default */
	returnEvent->type = NO_EVENT;
	/* switch over event types */
	switch (event->type) {
		case (SDL_QUIT): /* if the close button (x) was pressed */
			returnEvent->type = DO_QUIT;
			break;
		case (SDL_KEYUP): /* handle key up events */
			/* if tab was pressed, event is mark next button */
			if (event->key.keysym.sym == SDLK_TAB){
				returnEvent->type = MARK_NEXT_BUTTON;
			}
			else if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_KP_ENTER){
				/* if return button pressed, event is select curr button */
				if (menuViewState->currButton != FIRST_BUTTON)
					returnEvent->type = SELECT_CURR_BUTTON;
			}
			else if (event->key.keysym.sym == SDLK_UP){
				if (menuViewState->currButton == FIRST_BUTTON)
					/* if the first button (value selection) is selected, event is increase value */
					returnEvent->type = INCREASE_VALUE;
			}
			else if (event->key.keysym.sym == SDLK_DOWN){
				if (menuViewState->currButton == FIRST_BUTTON)
					/* if the first button (value selection) is selected, event is decrease value */
					returnEvent->type = DECREASE_VALUE;
			}
			break;
		case (SDL_MOUSEBUTTONUP): /* handle mouse button up events */
			for (int i = 0; i< numOfButtons; i++){ /* go over each button and check if mouse clicked on it */
				Widget * currButton = menuViewState->buttonsArr[i];
				if (i == FIRST_BUTTON){
					/* for the value selection button first check if click is on arrow area or not */
					if(isClickEventOnButton(event, currButton, UP_ARROW_BUTTON)){
						if(menuViewState->currButton == 0) /* if the first button is already selected */
							returnEvent->type = INCREASE_VALUE; /* if up arrow area is clicked, event is increase value */
						else{ /* if the first button is not yet selected */
							returnEvent->type = MARK_VALUES_BUTTON; /* only mark the first button */
							returnEvent->buttonNum = i;
						}
						break;
					}
					if(isClickEventOnButton(event, currButton, DOWN_ARROW_BUTTON)){
						if(menuViewState->currButton == 0) /* if the first button is already selected */
							returnEvent->type = DECREASE_VALUE;/* if down arrow area is clicked, event is decrease value */
						else{ /* if the first button is not yet selected */
							returnEvent->type = MARK_VALUES_BUTTON; /* only mark the first button */
							returnEvent->buttonNum = i;
						}
						break;
					}
					if (isClickEventOnButton(event, currButton, REGULAR_BUTTON)){
						returnEvent->type = MARK_VALUES_BUTTON; /* if other parts are clicked event is mark values button */
						returnEvent->buttonNum = i;
						break;
					}
				}
				if (isClickEventOnButton(event, currButton, REGULAR_BUTTON)){
					/* for all other buttons (regular buttons) check if they were clicked */
					returnEvent->type = SELECT_BUTTON_NUM;
					returnEvent->buttonNum = i;
					break;
				}
			}
			break;
	}
	return returnEvent;
}

/* mainMenuVTE is the VTE function for the main menu gui
 * it calls simpleMenuVTE function */
void* mainMenuVTE(void* viewState, SDL_Event* event){
	logicalEventRef returnEvent = simpleMenuVTE(viewState, event, MAIN_MENU_NUM_BUTTONS);
	return returnEvent;
}

/* chooseAnimalVTE is the VTE function for the choose cat/mouse guis
 * it calls simpleMenuVTE function */
void* chooseAnimalVTE(void* viewState, SDL_Event* event){
	logicalEventRef returnEvent = simpleMenuVTE(viewState, event, COMMON_MENU_NUM_BUTTONS);
	return returnEvent;
}

