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
	GUI returnGUI;
	returnGUI.stateId = stateId;
	returnGUI.model = NULL;
	returnGUI.viewState = NULL;
	switch(stateId){
		case(MAIN_MENU):
			returnGUI.start = startMainMenu;
			returnGUI.stop = stopMainMenu;
			/*...*/
			break;
		case(LOAD_GAME):
			returnGUI.start = startLoadGame;
			returnGUI.stop = stopLoadGame;
			/*...*/
			break;
		case(MAIN_MENU):
			returnGUI.start = startLoadGame;
			returnGUI.stop = stopLoadGame;
			break;
		case(NEW_GAME):
			returnGUI.start = startLoadGame;
			returnGUI.stop = stopLoadGame;
			break;
		case(CHOOSE_CAT):
			returnGUI.start = startLoadGame;
			returnGUI.stop = stopLoadGame;
			break;
		case(CHOOSE_MOUSE):
			returnGUI.start = startLoadGame;
			returnGUI.stop = stopLoadGame;
			break;
		case(CAT_SKILL):
			returnGUI.start = startLoadGame;
			returnGUI.stop = stopLoadGame;
			break;
		case(MOUSE_SKILL):
			returnGUI.start = startLoadGame;
			returnGUI.stop = stopLoadGame;
			break;
		case(LOAD_GAME):
			returnGUI.start = startLoadGame;
			returnGUI.stop = stopLoadGame;
			break;
		case(EDIT_GAME):
			returnGUI.start = startLoadGame;
			returnGUI.stop = stopLoadGame;
			break;
		case(WORLD_BUILDER):
			returnGUI.start = startLoadGame;
			returnGUI.stop = stopLoadGame;
			break;
		case(SAVE_WORLD):
			returnGUI.start = startLoadGame;
			returnGUI.stop = stopLoadGame;
			break;
		case(PLAY_GAME):
			returnGUI.start = startLoadGame;
			returnGUI.stop = stopLoadGame;
			break;
		case(ERR_MSG):
			returnGUI.start = startLoadGame;
			returnGUI.stop = stopLoadGame;
			break;
		default:
			break;
	}
	return returnGUI;
}

void startMainMenu(GUIref gui, void* initData){
	Widget *win = create_window(WIN_W,WIN_H);
	Widget *panel = create_panel(50,50,300,600)

}
