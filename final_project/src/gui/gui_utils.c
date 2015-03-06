/* includes */
#include "gui_utils.h"


int addChildWidgetsToParent(ListRef parent){
	ListRef currChild = parent->child;
	if (currChild==NULL){
		return 0;
	}
	Widget * parentWidget = parent->data;
	while(currChild != NULL){
		Widget * childWidget = currChild->data;
		childWidget->parentWidget = parentWidget;
		if (blitChildToParentWidget(childWidget, parentWidget) == -1)
			return -1;
		currChild = currChild->next;
	}
	return 0;
}

int blitChildToParentWidget(Widget * childWidget, Widget * parentWidget){
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
	}
	return 0;
}

int calcAbsWidgetXY(ListRef node){
	Widget * nodeWidget = node->data;
	if (nodeWidget->type == WINDOW){
		nodeWidget->absX = nodeWidget->absY = 0;
	}
	else { /* node has to have a parent node! */
		Widget * parentWidget = node->parent->data;
		nodeWidget->absX = parentWidget->absX + nodeWidget->location_rect.x;
		nodeWidget->absY = parentWidget->absY + nodeWidget->location_rect.y;
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
		/*
		case(LOAD_GAME):
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
			*/
		default:
			break;
	}
	return returnGUI;
}

int isClickEventOnButton(SDL_Event* event, Widget * button){
	if ((event->button.x > button->absX) && (event->button.x < button->absX + button->location_rect.w)
			&& (event->button.y > button->absY) && (event->button.y < button->absY + button->location_rect.h))
		return 1;
	return 0;
}

/* we assume the buttons are in the same surface */
/* return value ??????????????????????????????????????????? */
int changeSelectedButton(Widget * oldButton, Widget * newButton){
	setButtonNotSelected(oldButton);
	setButtonSelected(newButton);
	if (blitChildToParentWidget(oldButton, oldButton->parentWidget) == -1)
		return -1;
	if (blitChildToParentWidget(newButton, newButton->parentWidget) == -1)
		return -1;
	Widget * currChild = newButton->parentWidget;
	while(currChild->parentWidget != NULL){
		if (blitChildToParentWidget(currChild, currChild->parentWidget) == -1)
			return -1;
		currChild = currChild->parentWidget;
	}
	if (SDL_Flip(currChild) != 0) { /* we are in the top level widget - the window */
		sdlErrorPrint("failed to flip buffer");
	}
	return 0;
}

GameDataRef initGameDataToDefault(){
	GameDataRef gameData = (GameDataRef)calloc(1, sizeof(gameData));
	if (gameData == NULL){
		perrorPrint("calloc");
		return NULL;
	}
	gameData->catSkill = DEFAULT_SKILL;
	gameData->mouseSkill = DEFAULT_SKILL;
	gameData->currWorld = NULL;

	/* what else ???? */

	return gameData;
}

ViewStateref initializeGUIViewState(){
	/* allocate memory for create viewState */
	ViewStateref viewState = (ViewStateref)malloc(sizeof(ViewState));
	if (viewState == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	/* initialize viewState fields */
	viewState->image = NULL;
	viewState->menuButtons = NULL;
	viewState->UITree = NULL;
	return viewState;
}
