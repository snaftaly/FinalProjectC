/* includes */
#include "gui_utils.h"

/**** functions used for displaying the gui ****/


/* drawGui does DFS traversing through the UI tree which builds the widgets one on top of the other.
 * Then it flips the window surface for updating the display */
void drawGui(ListRef UITree){
	treeDFS(UITree, calcAbsWidgetXY, addChildWidgetsToParent);
//	int dfsRes = treeDFS(UITree, calcAbsWidgetXY, addChildWidgetsToParent);
//	if (dfsRes == -1)
//		return;
	Widget * window = UITree->data;
	if (SDL_Flip(window->surface) != 0) {
		sdlErrorPrint("failed to flip buffer");
		return;
	}
}

/* blit up to window is used for blitting data from a widget up to the
 * window surface, in order for it to be updated ont the window */
void blitUpToWindow(void * widget){
	Widget * currWidget = widget;
	while(currWidget->parentWidget != NULL){ /* while we are not in the window widget */
		blitChildToParentWidget(currWidget, currWidget->parentWidget);
		if (isError)
			return;
		currWidget = currWidget->parentWidget;
	}
	if (SDL_Flip(currWidget->surface) != 0) { /* now we are in the window widget - flip for updating diaplay */
		sdlErrorPrint("failed to flip buffer");
	}
}

/* blitChildToParentWidget is used to blit a widget to its parent, as set in the parent property of the widget */
void blitChildToParentWidget(Widget * childWidget, Widget * parentWidget){
	if (parentWidget == NULL)
		return;
	switch (childWidget->type){
		case(PANEL):
			/* if widget is a panel, blit the entire panel to the window, in a position of the location rect */
			if (SDL_BlitSurface(childWidget->surface, NULL,
					parentWidget->surface, &childWidget->locationRect) != 0){
				sdlErrorPrint("failed to blit panel");
				return;
			}
			break;
		case(BUTTON):
			/* if widget is a button, blit button according to the img_rect, in a position of the location rect */
			if (SDL_BlitSurface(childWidget->surface, &childWidget->imgRect,
					parentWidget->surface, &childWidget->locationRect) != 0){
				sdlErrorPrint("failed to blit button");
				return;
			}
			break;
		case(IMAGE):
			/* if widget is a image, blit button according to the img_rect, in a position of the location rect */
			if (SDL_BlitSurface(childWidget->surface, &childWidget->imgRect,
					parentWidget->surface, &childWidget->locationRect) != 0){
				sdlErrorPrint("failed to blit image/label");
				return;
			}
			break;
		default:
			break;
	}
}


/** functions used when traversing the UI tree **/
/* when trversint the UI tree with DFS, blit all parent's children to the parent
 * and update the parent of the children to be the parent */
int addChildWidgetsToParent(ListRef parent){
	ListRef currChild = parent->child;
	if (currChild==NULL){
		return 0;
	}
	Widget * parentWidget = parent->data;
	while(currChild != NULL){ /* go on all children */
		Widget * childWidget = currChild->data;
		childWidget->parentWidget = parentWidget; /* update the parent of the children to be the parent */
		blitChildToParentWidget(childWidget, parentWidget); /*  blit child to the parent widget surface */
		if (isError)
			return -1;
		currChild = currChild->next; /* get next child */
	}
	return 0;
}

/* when traversing the UI tree with DFS, update each node widget absolute X and Y (relative to the window)
 * according to its parent absolute location, and the the child relative location in the parent */
int calcAbsWidgetXY(ListRef node){
	Widget * nodeWidget = node->data;
	if (nodeWidget->type == WINDOW){ /* if we are at the top level absX=absY=0;*/
		nodeWidget->absX = 0;
		nodeWidget->absY = 0;
	}
	else { /* node has to have a parent node! */
		Widget * parentWidget = node->parent->data; /* get parent widget */
		/* update the child abs location by its parent abs location and the child relative location in the parent*/
		nodeWidget->absX = parentWidget->absX + nodeWidget->locationRect.x;
		nodeWidget->absY = parentWidget->absY + nodeWidget->locationRect.y;
	}
	return 0;
}




/*** size and location calculation functions ***/

/** menu functions **/

/* calculate the width of a menu panel */
int calcMenuPanelWidth(int titleWidth){
	return 2*MENU_TITLE_X_GAP + titleWidth;
}

/* calculate the height of a menu panel */
int calcMenuPanelHeight(int numButtons){
	return 4*MENU_TITLE_Y_GAP + MENU_TITLE_H + numButtons*MENU_BUTTON_H + (numButtons-1)*MENU_BUTTON_GAP;
}

/* calculate X location of a menu panel relative to the window */
int calcMenuPanelX(int titleWidth){
	return (WIN_W - calcMenuPanelWidth(titleWidth))/2;
}

/* calculate Y location of a menu panel relative to the window */
int calcMenuPanelY(int numButtons){
	return (WIN_H - calcMenuPanelHeight(numButtons))/2;
}

/* calculate X location of a menu button relative to the panel */
int calcMenuButtonX(int titleWidth){
	return (calcMenuPanelWidth(titleWidth)-MENU_BUTTON_W)/2;
}
/* calculate Y location of the menu first button relative to the panel */
int calcMenuButtonY(){ /* maybe change to a constant */
	return 2*MENU_TITLE_Y_GAP+MENU_TITLE_H;
}

/** top panel functions */
/* calculate X location of the first world builder button relative to the top panel */
int calcTopButtonX(){
	return (WIN_W - (3*PANEL_BUTTON_W + 2*WB_BUTTON_X_GAP))/2;
}

/* calculate Y location of a world builder button relative to the top panel */
int calcTopButtonY(){
	return WB_TITLE_H + 2*PANEL_WIDGET_Y_GAP;
}
/** side panel functions **/
/* calculate X location of a side panel button relative to the side panel */
int calcSideButtonX(){
	return (SIDE_PANEL_W - PANEL_BUTTON_W)/2;
}
/* calculate Y location of the first side panel button relative to the side panel */
int calcSideButtonY(){
	return (SIDE_PANEL_H - (5*PANEL_BUTTON_H + 4*PANEL_WIDGET_Y_GAP))/2;
}

/** wb functions **/
/* calc X location of world builder title relative to top panel */
int calcWBtitleX(int title_W){
	return (WIN_W - WB_TITLE_W)/2;
}

/** error message functions **/
/* calc Y location of world builder title relative to top panel */
int calcErrPanelY(){
	return (WIN_H - calcErrPanelHeight())/2;
}

/* calc height of the error message panel*/
int calcErrPanelHeight(){
	return MENU_TITLE_H + MENU_TITLE_H + MENU_BUTTON_H + 4*MENU_TITLE_Y_GAP;
}

/* calc X location of error message missing item message, relative to panel */
int calcMisItemMsgX(int messageWidth, int titleWidth){
	return (calcMenuPanelWidth(titleWidth)-messageWidth)/2;
}

/* calc Y location of error message missing item message, relative to panel */
int calcMisItemMsgY(){
	return MENU_TITLE_H + 2*MENU_TITLE_Y_GAP;
}

/* calc Y location of error message button, relative to panel */
int calcErrMsgButtonY(){
	return calcMisItemMsgY() + MENU_TITLE_Y_GAP + MENU_TITLE_H;
}

/** play game functions **/
/* X location of game over label, relative to top panel */
int calcGameOverLabelX(){
	return (WIN_W - GAME_OVER_LABEL_W)/2;
}

/* Y location of game over label, relative to top panel */
int calcGameOverLabelY(){
	return (TOP_PANEL_H - GAME_OVER_LABEL_H)/2;
}

/* X location of game whos player move is it label, relative to top panel */
int calcMoveLabelX(int isCatCurrPlayer){
	if (isCatCurrPlayer)
		return (WIN_W-(GAME_INFO_LABEL_W+CAT_MOUSE_DIF+2*DIGIT_LABEL_W))/2;
	else
		return (WIN_W-(GAME_INFO_LABEL_W+2*DIGIT_LABEL_W))/2;
}

/* X location of game num turns tens digit, relative to top panel */
int calcTensLabelX(int isCatCurrPlayer){
	return calcMoveLabelX(isCatCurrPlayer)+GAME_INFO_LABEL_W;
}

/* X location of game num turns units digit, relative to top panel */
int calcUnitsLabelX(int isCatCurrPlayer){
	return calcTensLabelX(isCatCurrPlayer)+DIGIT_LABEL_W;
}

/* X location of game state label, relative to top panel */
int calcStateLabelX(){
	return (WIN_W-STATE_LABEL_W)/2;
}

/* Y location of game state label, relative to top panel */
int calcStateLabelY(){
	return GAME_INFO_LABEL_H+2*PANEL_WIDGET_Y_GAP;
}

/* X location of pause button, relative to top panel */
int calcPauseButtonX(){
	return (WIN_W - PAUSE_BUTTON_W)/2;
}

/* Y location of pause button, relative to top panel */
int calcPauseButtonY(){
	return 3*PANEL_WIDGET_Y_GAP + 2*GAME_INFO_LABEL_H;
}

