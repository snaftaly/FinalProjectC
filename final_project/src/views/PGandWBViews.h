#ifndef PGWBVIEWS_H_
#define PGWBVIEWS_H_

/* includes */
#include "../services/LogEventItems.h"
#include "../services/GUIUtils.h"
#include "../services/CatAndMouseUtils.h"

/* struct */
/* a struct for the ThreePartView Extension used by
 * world builder and play game */
typedef struct ThreePartViewExt{

	/* listRefs of the top/side panel nodes, used by play game/ world builder for easy access */
	ListRef topPanelNode;
	ListRef sidePanelNode;
	/* the panel of the grid, used by play game/ world builder for easy access */
	Widget * gridPanel;

} ThreePartViewExt;

typedef struct ThreePartViewExt * ThreePartViewExtRef;

/* function decelerations: */

/* stand alone VTEs */
void* worldBuilderVTE(void* viewState, SDL_Event* event);
void* playGameVTE(void* viewState, SDL_Event* event);


#endif /* PGWBVIEWS_H_ */
