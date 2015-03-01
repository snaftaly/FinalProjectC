/* includes */
#include "gui_utils.h"



int addWidgetToParent(ListRef parent){
	ListRef currChild = parent->child;
	if (currChild==NULL){
		return 0;
	}
	Widget * parentWidget = parent->data;
	while(currChild != NULL){
		Widget * childWidget = currChild->data;
		switch (childWidget->type){
			case(PANEL):
				if (SDL_BlitSurface(childWidget->surface, NULL,
						parentWidget->surface, &childWidget->location_rect) != 0){
					fprintf(stderr, "ERROR: failed to blit panel: %s\n", SDL_GetError());
					return -1;
				}
				break;
			case(BUTTON):
				if (SDL_BlitSurface(childWidget->surface, &childWidget->img_rect,
						parentWidget->surface, &childWidget->location_rect) != 0){
					fprintf(stderr, "ERROR: failed to blit button: %s\n", SDL_GetError());
					return -1;
				}
				break;
			case(IMAGE):
				if (SDL_BlitSurface(childWidget->surface, &childWidget->img_rect,
						parentWidget->surface, &childWidget->location_rect) != 0){
					fprintf(stderr, "ERROR: failed to blit image/label: %s\n", SDL_GetError());
					return -1;
				}
				break;
			default:
				break;
		}
		currChild = currChild->next;
	}
	return 0;
}

GUI createGUIForState(StateId stateId){
	GUI returnGUI = NULL;
	returnGUI.stateId = stateId;
	returnGUI.model = NULL;
	returnGUI.viewState = NULL;
	returnGUI.start = startGUI;
	returnGUI.stop = stopGUI;
	/*...*/
}

