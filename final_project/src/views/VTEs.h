#ifndef VIEWS_H_
#define VIEWS_H_

/* includes */
#include "../services/MVPutils.h"

/* function decelerations: */

/* menus VTEs */
void* simpleMenuVTE(void* viewState, SDL_Event* event, int numOfButtons);
void* complexMenuVTE(void* viewState, SDL_Event* event);
void* mainMenuVTE(void* viewState, SDL_Event* event);
void* chooseAnimalVTE(void* viewState, SDL_Event* event);

/* stand alone VTEs */
void* worldBuilderVTE(void* viewState, SDL_Event* event);
void* errMsgVTE(void* viewState, SDL_Event* event);
void* playGameVTE(void* viewState, SDL_Event* event);



#endif /* VIEWS_H_ */
