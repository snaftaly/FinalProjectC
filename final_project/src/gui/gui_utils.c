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
		default:
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


/*int isClickEventOnButton(SDL_Event* event, Widget * button){
	if ((event->button.x > button->absX) && (event->button.x < button->absX + button->location_rect.w)
			&& (event->button.y > button->absY) && (event->button.y < button->absY + button->location_rect.h))
		return 1;
	return 0;
}*/

int isClickEventOnButton(SDL_Event* event, Widget * button, int buttonType){
	int isClicked;
	switch (buttonType){
		case (UP_ARROW_BUTTON):
			isClicked = isClickEventOnSpecificButton(event, button, ARROW_BUTTON_X, ARROW_BUTTON_Y, 0);
			break;
		case (DOWN_ARROW_BUTTON):
			isClicked = isClickEventOnSpecificButton(event, button, ARROW_BUTTON_X, 0, ARROW_BUTTON_Y);
			break;
		case (REGULAR_BUTTON):
			isClicked = isClickEventOnSpecificButton(event, button, 0, 0, 0);
			break;
		default:
			isClicked = 0;
	}
	return isClicked;
}


int isClickEventOnSpecificButton(SDL_Event* event, Widget * button, int Xdelta, int YdeltaUp, int YdeltaDown){
	if ((event->button.x > button->absX + Xdelta) &&
			(event->button.x < button->absX + button->location_rect.w) &&
			(event->button.y > button->absY + YdeltaDown) &&
			(event->button.y < button->absY + button->location_rect.h - YdeltaUp))
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
	if (SDL_Flip(currChild->surface) != 0) { /* we are in the top level widget - the window */
		sdlErrorPrint("failed to flip buffer");
	}
	return 0;
}

void setValuesButtonFromInit(int value, Widget* valuesButton){
	valuesButton->button_selected_rect.y += (value-1)*MENU_BUTTON_H;
	valuesButton->button_non_selected_rect.y += (value-1)*MENU_BUTTON_H;
	valuesButton->img_rect = valuesButton->button_non_selected_rect;
}

void increaseValuesButton(int* currValue, int maxValue, Widget* valuesButton){
	if (*currValue < maxValue){
		valuesButton->button_selected_rect.y += MENU_BUTTON_H;
		valuesButton->button_non_selected_rect.y += MENU_BUTTON_H;
		*currValue += 1;
	}
}

void decreaseValuesButton(int * currValue, int maxValue, Widget * valuesButton){
	if (*currValue > MIN_VALUE){
		valuesButton->button_selected_rect.y -= MENU_BUTTON_H;
		valuesButton->button_non_selected_rect.y -= MENU_BUTTON_H;
		*currValue -= 1;
	}
}

MenuDataRef initMenuDataToDefault(){
	MenuDataRef menuData = malloc(sizeof(MenuData));
	if (menuData == NULL){
		perrorPrint("calloc");
		return NULL;
	}

	menuData->mainMenuButton = 0;
	menuData->chooseCatButton = 0;
	menuData->chooseMouseButton = 0;
	menuData->catSkillButton = 0;
	menuData->mouseSkillButton = 0;
	menuData->loadGameButton = 0;
	menuData->editGameButton = 0;
	menuData->saveWorldButton = 0;

	menuData->catSkill = DEFAULT_SKILL;
	menuData->mouseSkill = DEFAULT_SKILL;

	menuData->editedWorld = MIN_VALUE;
	menuData->loadGameWorld = MIN_VALUE;
	menuData->saveOnWorld = MIN_VALUE;

	menuData->currValueTemp = 0;

	menuData->currWorld = NULL;

	menuData->preWorldBuilder = MAIN_MENU;
	/* what else ???? */

	return menuData;
}

int calcPanelX(int titleWidth){
	return (WIN_W - calcPanelWidth(titleWidth))/2;
}

int calcPanelY(int numButtons){
	return (WIN_H - calcPanelHeight(numButtons))/2;
}

int calcPanelWidth(int titleWidth){
	return 2*MENU_TITLE_X_GAP + titleWidth;
}

int calcPanelHeight(int numButtons){
	return 2*MENU_TITLE_Y_GAP + MENU_TITLE_H + numButtons*(MENU_BUTTON_H + MENU_BUTTON_GAP);
}

int calcMenuButtonX(int titleWidth){
	return (calcPanelWidth(titleWidth)-MENU_BUTTON_W)/2;
}

int calcMenuButtonY(){ /* maybe change to a constant */
	return 2*MENU_TITLE_Y_GAP+MENU_TITLE_H;
}

int calcTopButtonX(){
	return (WIN_W - (3*WB_BUTTON_W + 2*WB_BUTTON_X_GAP))/2;
}

int calcTopButtonY(){
	return WB_TITLE_H + 2*WB_WIDGET_Y_GAP;
}

int calcSideButtonX(){
	return (SIDE_PANEL_W - WB_BUTTON_W)/2;
}

int calcSideButtonY(){
	return (SIDE_PANEL_H - (5*WB_BUTTON_H + 4*WB_WIDGET_Y_GAP))/2;
}

int calcWBtitleX(int title_W){
	return (WIN_W - WB_TITLE_W)/2;
}
