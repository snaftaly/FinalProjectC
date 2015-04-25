#ifndef VIEWS_H_
#define VIEWS_H_

/* includes */
#include "../services/LogEventItems.h"
#include "../services/GUIUtils.h"
#include "../services/CatAndMouseUtils.h"


/* structs */
/* a struct for the view state */
typedef struct ViewState{
	/* SDL_Surface refs for the different images used by the gui */
	SDL_Surface * image;
	SDL_Surface * bgImage;
	SDL_Surface * gridItemImage; /* used by play game and world builder */
	/* Widgets * arrays that will hold different widgets used by the gui for easy access */
	Widget ** buttonsArr;
	Widget ** labelArr;
	Widget ** gridItemsImgArr; /* used by play game and world builder */

	int currButton; /* holds the menus current selected button */

	ListRef UITree; /* holds all the widgets of the GUI (except grid items images widgets) */
	/* list refs of the top/side panel nodes, used by play game/ world builder for easy access */
	ListRef topPanelNode;
	ListRef sidePanelNode;
	/* the panel of the grid, used by play game/ world builder for easy access */
	Widget * gridPanel;

} ViewState;

typedef struct ViewState * ViewStateref;

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
