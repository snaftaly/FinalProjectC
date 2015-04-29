#ifndef MEVIEWS_H_
#define MEVIEWS_H_

/* includes */
#include "../services/LogEventItems.h"
#include "../services/GUIUtils.h"
#include "../services/CatAndMouseUtils.h"


/* structs */
/* a struct for the view state used by all GUIs*/
typedef struct ViewState{
	/* SDL_Surface refs for the different images used by the gui */
	SDL_Surface * image;
	SDL_Surface * bgImage;
	SDL_Surface * gridItemImage; /* used by play game and world builder */
	/* Widgets * arrays that will hold different widgets used by the gui for easy access */
	Widget ** buttonsArr;
	Widget ** labelArr;
	Widget ** gridItemsImgArr; /* used by play game and world builder */
	int currButton; /* holds the current selected button */

	ListRef UITree; /* holds all the widgets of the GUI (except grid items images widgets) */

	void * ViewExt; /* an extension to the view for specific GUI features */

} ViewState;

typedef struct ViewState * ViewStateref;

/* function decelerations: */

/* menus VTEs */
void* simpleMenuVTE(void* viewState, SDL_Event* event, int numOfButtons);
void* complexMenuVTE(void* viewState, SDL_Event* event);
void* mainMenuVTE(void* viewState, SDL_Event* event);
void* chooseAnimalVTE(void* viewState, SDL_Event* event);

/* err msg alone VTE */
void* errMsgVTE(void* viewState, SDL_Event* event);



#endif /* MEVIEWS_H_ */
