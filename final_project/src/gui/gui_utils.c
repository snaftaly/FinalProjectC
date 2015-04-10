/* includes */
#include "gui_utils.h"

void drawGui(ViewStateref viewState){ //changes to gui view state from GUIref
	treeDFS(viewState->UITree, calcAbsWidgetXY, addChildWidgetsToParent);
	Widget * window = viewState->UITree->data;
	if (SDL_Flip(window->surface) != 0) {
		sdlErrorPrint("failed to flip buffer");
		return;
	}
}

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

//change return value to null!!
int blitChildToParentWidget(Widget * childWidget, Widget * parentWidget){
	if (parentWidget == NULL)
		return 0;
	switch (childWidget->type){
		case(PANEL):
			if (SDL_BlitSurface(childWidget->surface, NULL,
					parentWidget->surface, &childWidget->location_rect) != 0){
				sdlErrorPrint("failed to blit panel");
				return -1;
			}
			break;
		case(BUTTON):
			if (SDL_BlitSurface(childWidget->surface, &childWidget->img_rect,
					parentWidget->surface, &childWidget->location_rect) != 0){
				sdlErrorPrint("failed to blit button");
				return -1;
			}
			break;
		case(IMAGE):
			if (SDL_BlitSurface(childWidget->surface, &childWidget->img_rect,
					parentWidget->surface, &childWidget->location_rect) != 0){
				sdlErrorPrint("failed to blit image/label");
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
	blitUpToWindow(newButton->parentWidget);
	return 0;
}

void blitUpToWindow(void * widget){ // changed to void * !!!!!!!!!!!!!!!!!!////
	Widget * currWidget = widget;
	while(currWidget->parentWidget != NULL){
		if (blitChildToParentWidget(currWidget, currWidget->parentWidget) == -1)
			return;
		currWidget = currWidget->parentWidget;
	}
	if (SDL_Flip(currWidget->surface) != 0) { /* we are in the top level widget - the window */
		sdlErrorPrint("failed to flip buffer");
	}
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
	return 4*MENU_TITLE_Y_GAP + MENU_TITLE_H + numButtons*MENU_BUTTON_H + (numButtons-1)*MENU_BUTTON_GAP;
}

int calcMenuButtonX(int titleWidth){
	return (calcPanelWidth(titleWidth)-MENU_BUTTON_W)/2;
}

int calcMenuButtonY(){ /* maybe change to a constant */
	return 2*MENU_TITLE_Y_GAP+MENU_TITLE_H;
}

int calcTopButtonX(){
	return (WIN_W - (3*PANEL_BUTTON_W + 2*WB_BUTTON_X_GAP))/2;
}

int calcTopButtonY(){
	return WB_TITLE_H + 2*PANEL_WIDGET_Y_GAP;
}

int calcSideButtonX(){
	return (SIDE_PANEL_W - PANEL_BUTTON_W)/2;
}

int calcSideButtonY(){
	return (SIDE_PANEL_H - (5*PANEL_BUTTON_H + 4*PANEL_WIDGET_Y_GAP))/2;
}

int calcWBtitleX(int title_W){
	return (WIN_W - WB_TITLE_W)/2;
}

int calcErrPanelY(){
	return (WIN_H - calcErrPanelHeight())/2;
}

int calcErrPanelHeight(){
	return MENU_TITLE_H + MENU_TITLE_H + MENU_BUTTON_H + 4*MENU_TITLE_Y_GAP;
}

int calcMisItemMsgX(int messageWidth, int titleWidth){
	return (calcPanelWidth(titleWidth)-messageWidth)/2;
}

int calcMisItemMsgY(){
	return MENU_TITLE_H + 2*MENU_TITLE_Y_GAP;
}

int calcErrMsgButtonY(){
	return calcMisItemMsgY() + MENU_TITLE_Y_GAP + MENU_TITLE_H;
}

int calcGameOverLabelX(){
	return (WIN_W - GAME_OVER_LABEL_W)/2;
}

int calcGameOverLabelY(){
	return (TOP_PANEL_H - GAME_OVER_LABEL_H)/2;
}

int calcPauseButtonX(){
	return (WIN_W - PAUSE_BUTTON_W)/2;
}

int calcPauseButtonY(){
	return 3*PANEL_WIDGET_Y_GAP + 2*GAME_INFO_LABEL_H;
}

int calcMoveLabelX(int isCatCurrPlayer){
	if (isCatCurrPlayer)
		return (WIN_W-(GAME_INFO_LABEL_W+CAT_MOUSE_DIF+2*DIGIT_LABEL_W))/2;
	else
		return (WIN_W-(GAME_INFO_LABEL_W+2*DIGIT_LABEL_W))/2;
}

int calcTensLabelX(int isCatCurrPlayer){
	return calcMoveLabelX(isCatCurrPlayer)+GAME_INFO_LABEL_W;
}

int calcUnitsLabelX(int isCatCurrPlayer){
	return calcTensLabelX(isCatCurrPlayer)+DIGIT_LABEL_W;
}

int calcStateLabelX(){
	return (WIN_W-STATE_LABEL_W)/2;
}

int calcStateLabelY(){
	return GAME_INFO_LABEL_H+2*PANEL_WIDGET_Y_GAP;
}

