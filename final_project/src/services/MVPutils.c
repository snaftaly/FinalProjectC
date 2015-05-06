/* includes */
#include "MVPutils.h"

/**** helper functions for MVP ****/
/** data initialization functions: **/

/* initGUIViewState initializes the viewstate for all GUIs
 * not all guis uses all the parts of the view state */
ViewStateref initGUIViewState(){
	/* allocate memory for create viewState */
	ViewStateref viewState = (ViewStateref)malloc(sizeof(ViewState));
	if (viewState == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	/* initialize viewState fields */
	viewState->image = NULL;
	viewState->bgImage = NULL;
	viewState->buttonsArr = NULL;
	viewState->labelArr = NULL;
	viewState->currButton = 0;
	viewState->UITree = NULL;
	viewState->ViewExt = NULL;

	return viewState;
}

/* initGUIViewState initializes the viewstate for all GUIs
 * not all guis uses all the parts of the view state */
ThreePartViewExtRef initThreePartViewExt(){
	/* allocate memory */
	ThreePartViewExtRef viewStateExt = (ThreePartViewExtRef)malloc(sizeof(ThreePartViewExt));
	if (viewStateExt == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	/* initialize ThreePartViewExt fields */
	viewStateExt->gridItemImage = NULL;
	viewStateExt->gridItemsImgArr = NULL;
	viewStateExt->gridPanel = NULL;
	viewStateExt->sidePanelNode = NULL;
	viewStateExt->topPanelNode = NULL;

	return viewStateExt;
}

/* initialize menu data to its relevant default values */
MenuDataRef initMenuDataToDefault(){
	MenuDataRef menuData = malloc(sizeof(MenuData)); /* memory allocation */
	if (menuData == NULL){
		perrorPrint("calloc");
		return NULL;
	}
	menuData->catSkill = DEFAULT_SKILL;
	menuData->mouseSkill = DEFAULT_SKILL;

	menuData->mainMenuButton = 0;
	menuData->chooseCatButton = 0;
	menuData->chooseMouseButton = 0;
	menuData->catSkillButton = 0;
	menuData->mouseSkillButton = 0;
	menuData->loadGameButton = 0;
	menuData->editGameButton = 0;
	menuData->saveWorldButton = 0;
	menuData->errMsgButton = 0;

	menuData->preChooseCat = MAIN_MENU;
	menuData->preWorldBuilder = MAIN_MENU;

	menuData->currValueTemp = 0;
	menuData->loadGameWorld = DEFAULT_WORLD;
	menuData->editedWorld = DEFAULT_WORLD;

	menuData->loadFromFile = 1;
	menuData->isGamePaused = 0;
	menuData->isCatFirst = 0;

	menuData->gameGridData = NULL;


	return menuData;
}

/* initMenuModel is used for init menu data according to initData
 * where initData is assumed to be of type MenuDataRef */
void initMenuModel(GUIref gui, void* initData){
	if (initData == NULL){
		gui->model = initMenuDataToDefault();
	}
	else{
		gui->model = initData;
	}
}

/* initMainMenuData is a specific function for updating the menu data
 * if initData is null, it will set the model with the defualt menu data settings
 * else, it will take update the menu data bethe main menu button number in init data
 * and then free init data  */
void initMainMenuData(GUIref mainMenuGui, void * initData){
	mainMenuGui->model = initMenuDataToDefault();
	if (initData != NULL){
		MenuDataRef data = initData;
		MenuDataRef model = mainMenuGui->model;
		model->mainMenuButton = data->mainMenuButton;
		freeMenuData(initData); /* free init data (including the grid) */
	}
}
/* initWorldBuilderModel inits the world builder data according to initData
 * where initData is assumed to be of type MenuDataRef and not NULL */
void initWorldBuilderModel(GUIref gui, void* initData){
	MenuDataRef menuData = initData;

	WBDataRef wbData = malloc(sizeof(WBData)); /* allocate memory */
	if (wbData == NULL){
		perrorPrint("malloc");
		return;
	}
	/* update the data according to initData */
	gui->model = wbData;
	wbData->gameGridData = NULL;
	wbData->editedWorld = menuData->editedWorld;

	gridItemPosition currPos = {0, 0};
	/* update the data from file or from gridData, depending on where we came from */
	if ((menuData->preWorldBuilder == SAVE_WORLD && menuData->loadFromFile == 0) || menuData->preWorldBuilder == ERR_MSG){
		/* we pressed back in SAVE_WORLD or were in errMsg */
		wbData->gameGridData = menuData->gameGridData;
		wbData->currPos = menuData->wbCurrPos;
		wbData->numTurns = menuData->numTurns;
		wbData->isCatFirst = menuData->isCatFirst;
	}
	else{ /* preWorldBuilder == MAIN_MENU || (preWorldBuilder == EDIT_GAME || preWorldBuilder = SAVE_WORLD) && loadFromFile=1 */
		wbData->gameGridData = initGameDataByFile(wbData->editedWorld, &wbData->numTurns, &wbData->isCatFirst);
		wbData->currPos = currPos;
	}
	/* get the positions of the items in the grid, and update the relevant data in the model */
	updateItemsPositions(&wbData->mousePos,&wbData->catPos,&wbData->cheesePos, wbData->gameGridData);

	free(initData); /* free initData, without freeing the grid */
}

/* initPlayGameModel inits the world play game according to initData,
 * where initData is assumed to be of type MenuDataRef and not NULL */
void initPlayGameModel(GUIref gui, void* initData){
	MenuDataRef menuData = initData;

	PGDataRef pgData = malloc(sizeof(PGData)); 	/* allocate memory */
	if (pgData == NULL){
		perrorPrint("malloc");
		return;
	}
	gui->model = pgData;
	/* update the data from file or from gridData, depending on where we came from */
	if (menuData->loadFromFile){
		pgData->gameGridData = initGameDataByFile(menuData->loadGameWorld, &pgData->numTurnsLeft,
				&pgData->isCatCurrPlayer);
		if (isError)
			return;
	}
	else{
		pgData->gameGridData = menuData->gameGridData;
		pgData->isCatCurrPlayer = menuData->isCatFirst;
		pgData->numTurnsLeft = menuData->numTurns;
	}
	/* update the data according to initData */
	pgData->loadGameWorld = menuData->loadGameWorld;
	pgData->isCatHuman = menuData->isCatHuman;
	pgData->isMouseHuman = menuData->isMouseHuman;
	pgData->catSkill = menuData->catSkill;
	pgData->mouseSkill = menuData->mouseSkill;
	pgData->isGamePaused = menuData->isGamePaused;

	pgData->isGameOver = 0;
	/* get the positions of the items in the grid, and update the relevant data in the model */
	updateItemsPositions(&pgData->mousePos,&pgData->catPos,&pgData->cheesePos, pgData->gameGridData);

	free(initData); /* free initData, without freeing the grid */
}

/**** VTE helper functions ****/

/* handleThreePartLayoutMouseSelect updates the logical event of a mouse click in a three part layout GUI (world builder
 * and play game). If the click was on a button it will set the event to SELECT_BUTTON_NUM, and if the click was on
 * a grid square it will set the event to SELECT_SQUARE */
void handleThreePartLayoutMouseSelect(SDL_Event * event, logicalEventRef returnEvent,
		Widget ** buttons, int numButtons){
	/* first check if the click was on top panel or on side panel */
	if (event->button.x < WIN_W - GRID_SIZE || event->button.y < WIN_H - GRID_SIZE){
		/* check if the click is on a button */
		for (int i = 0; i< numButtons; i++){
			Widget * currButton = buttons[i];
			if (currButton == NULL) /* used for handling pause button when game is over */
				continue;
			if (isClickEventOnButton(event, currButton, REGULAR_BUTTON)){
				/* if click was on a button change the return event type */
				returnEvent->type = SELECT_BUTTON_NUM;
				returnEvent->buttonNum = i;
				return;
			}
		}
	}
	else{ /* click is inside the grid */
		returnEvent->type = SELECT_SQUARE; /* change the return event type */
		/* update the grid position clicked */
		returnEvent->gridPos.col = (event->button.x-(WIN_W - GRID_SIZE))/(GRID_SQUARE_SIZE+GRID_GAP_SIZE);
		returnEvent->gridPos.row = (event->button.y-(WIN_W - GRID_SIZE))/(GRID_SQUARE_SIZE+GRID_GAP_SIZE);
	}
}

/* handleThreePartLayoutArrowKey updates the logical event according to arrow key strokes */
void handleThreePartLayoutArrowKey(SDLKey key, logicalEventRef returnEvent){
	/* check the if the key stroke was on one of the arrows and update
	 * the logical event accordingly */
	if (key == SDLK_UP)
		returnEvent->type = GO_UP;
	else if (key ==  SDLK_DOWN)
		returnEvent->type = GO_DOWN;
	else if (key ==  SDLK_RIGHT)
		returnEvent->type = GO_RIGHT;
	else if (key ==  SDLK_LEFT)
		returnEvent->type = GO_LEFT;
}

/* check if a given button has an SDL click event on its absolute location */
int isClickEventOnButton(SDL_Event* event, Widget * button, int buttonType){
	int isClicked = 0;
	switch (buttonType){
		case (UP_ARROW_BUTTON): /* for up arrow button - check only the location of the up arrow in the button */
			isClicked = isClickEventOnButtonSpecificArea(event, button, ARROW_BUTTON_X, ARROW_BUTTON_Y, 0);
			break;
		case (DOWN_ARROW_BUTTON): /* for down arrow button - check only the location of the down arrow in the button*/
			isClicked = isClickEventOnButtonSpecificArea(event, button, ARROW_BUTTON_X, 0, ARROW_BUTTON_Y);
			break;
		case (REGULAR_BUTTON): /* for a regular button - check the entire button */
			isClicked = isClickEventOnButtonSpecificArea(event, button, 0, 0, 0);
			break;
		default:
			isClicked = 0;
	}
	return isClicked;
}

/* check if a given button area has an SDL click event on it
 * return 1 if so, 0 otherwise */
int isClickEventOnButtonSpecificArea(SDL_Event* event, Widget * button, int Xdelta, int YdeltaUp, int YdeltaDown){
	if ((event->button.x > button->absX + Xdelta) &&
			(event->button.x < button->absX + button->locationRect.w) &&
			(event->button.y > button->absY + YdeltaDown) &&
			(event->button.y < button->absY + button->locationRect.h - YdeltaUp))
		return 1;
	return 0;
}

/**** PHE helper functions ****/
/** buttons update related functions **/

/* changeSelectedButton changes the selected button from one to antoher (used in menu guis)
 * assumes the buttons are in the same surface */
void changeSelectedButton(Widget * oldButton, Widget * newButton){
	setButtonNotSelected(oldButton); /* set the old button to not selected */
	setButtonSelected(newButton); /* set the new button to selected */
	/* blit the two buttons to the parent widget */
	blitChildToParentWidget(oldButton, oldButton->parentWidget);
	if (isError)
		return;
	blitChildToParentWidget(newButton, newButton->parentWidget);
	if (isError)
		return;
	blitUpToWindow(newButton->parentWidget); /* update display */
}

/* set the a value button to its initializes value */
void setValuesButtonFromInit(int value, Widget * valuesButton){
	/* update the values selected rect and non-selected rect*/
	valuesButton->buttonSelectedRect.y += (value-1)*MENU_BUTTON_H;
	valuesButton->buttonNonSelectedRect.y += (value-1)*MENU_BUTTON_H;
	/* updatethe img rect to the button non-selected rect */
	valuesButton->imgRect = valuesButton->buttonNonSelectedRect;
}

/* increase value in a values button, and update the value */
void increaseValuesButton(int* currValue, int maxValue, Widget* valuesButton){
	if (*currValue < maxValue){
		/* update the values selected rect and non-selected rect*/
		valuesButton->buttonSelectedRect.y += MENU_BUTTON_H;
		valuesButton->buttonNonSelectedRect.y += MENU_BUTTON_H;
		/* update the value */
		*currValue += 1;
	}
}

void decreaseValuesButton(int * currValue, int maxValue, Widget * valuesButton){
	if (*currValue > MIN_VALUE){
		/* update the values selected rect and non-selected rect*/
		valuesButton->buttonSelectedRect.y -= MENU_BUTTON_H;
		valuesButton->buttonNonSelectedRect.y -= MENU_BUTTON_H;
		/* update the value */
		*currValue -= 1;
	}
}

/**** grid related functions ****/

/* createGridItemsImgArr creates an array of the images of the different items in the array
 * applicable for world builder and play game GUIs */
void createGridItemsImgArr(ViewStateref viewState){
	/* create a reference to the ThreePartLayoutViewExt */
	ThreePartViewExtRef threePartView = viewState->ViewExt;

	SDL_Surface * gridItemsImg = SDL_LoadBMP("images/GridItems.bmp"); /* load the image file of the grid items */
	if (gridItemsImg == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	threePartView->gridItemImage = gridItemsImg;
	Widget ** gridItemImagesArr = malloc(NUM_GRID_ITEMS*sizeof(Widget *)); /* allocate memory for the array */
	if (gridItemImagesArr == NULL){
		perrorPrint("malloc");
		return;
	}
	threePartView->gridItemsImgArr = gridItemImagesArr; /* update the viewstate of current gui */
	/* add the images to the array */
	int gridItemImgX = 0, gridItemImgY = 0;
	for (int i = 0; i < NUM_GRID_ITEMS; i++){
		gridItemImagesArr[i] = create_image(0, 0, GRID_SQUARE_SIZE, GRID_SQUARE_SIZE,
				gridItemsImg, gridItemImgX, gridItemImgY);
		/* if there was an error, free all the data we have so far in the array, and the array itself */
		if (gridItemImagesArr[i] == NULL){
			for (int j = 0; j < i; j++){
				freeWidget(gridItemImagesArr[i]);
			}
			free(gridItemImagesArr);
			gridItemImagesArr = NULL;
			return;
		}
		gridItemImgY += GRID_SQUARE_SIZE;
	}
	/* set the select, deselect and warn items transparent */
	setImageTransparent(gridItemImagesArr[SELECT], TR_RED, TR_GREEN, TR_BLUE);
	if (isError)
		return;
	setImageTransparent(gridItemImagesArr[DESELECT], TR_RED, TR_GREEN, TR_BLUE);
	if (isError)
		return;
	setImageTransparent(gridItemImagesArr[WARN], TR_RED, TR_GREEN, TR_BLUE);
	if (isError)
		return;
}

/* createGridByData puts the grid images on the grid panel according to the grid data */
void createGridByData(Widget * gridPanel, char ** gridData, Widget ** gridItemImages){
	Widget * currItemImage = NULL;
	/* go over each grid position and add the appropriate image to the appropriate position in the panel */
	for (int i = 0; i < ROW_NUM; i++){
		for (int j = 0; j < COL_NUM; j++){
			switch(gridData[i][j]){
				case(CAT_CHAR):
					currItemImage = gridItemImages[CAT];
					break;
				case(MOUSE_CHAR):
					currItemImage = gridItemImages[MOUSE];
					break;
				case(CHEESE_CHAR):
					currItemImage = gridItemImages[CHEESE];
					break;
				case(WALL_CHAR):
					currItemImage = gridItemImages[WALL];
					break;
				case(EMPTY_CELL_CHAR):
					currItemImage = gridItemImages[EMPTY];
					break;
				default:
					break;
			}
			/* blit the current item to its place on the grid, according to i and j */
			blitItemToGrid(gridPanel, currItemImage, i, j);
			if (isError)
				return;
		}
	}

}

/* put the selection on a specific grid position */
void selectGridPos(Widget *gridPanel, Widget ** gridItemstImagesArr, gridItemPosition currPos){
	blitItemToGrid(gridPanel, gridItemstImagesArr[SELECT], currPos.row, currPos.col);
}

/* remove the selection on a specific grid position */
void deselectGridPos(Widget *gridPanel, Widget ** gridItemstImagesArr, gridItemPosition currPos){
	blitItemToGrid(gridPanel, gridItemstImagesArr[DESELECT], currPos.row, currPos.col);
}

/* changeSelectedGridSquare is used by world builder and play game to change the selected position on the grid */
void changeSelectedGridSquare(Widget * gridPanel, Widget ** gridItemsImages, gridItemPosition * currPos,
		gridItemPosition newPos){
	deselectGridPos(gridPanel, gridItemsImages, *currPos); /* deselect the current pos */
	if (isError)
		return;
	selectGridPos(gridPanel, gridItemsImages, newPos); /* select the new pos */
	if (isError)
		return;
	/* change the curr position data to the new position */
	currPos->row = newPos.row;
	currPos->col = newPos.col;
	/* update the view */
	blitUpToWindow(gridPanel);
}

/* blitItemToGrid gets the location of the current grid image on the panel, according to grid position
 * and blits it on the grid */
void blitItemToGrid(Widget *gridPanel, Widget * itemImage, int row, int col){
	itemImage->locationRect.x = col*(GRID_SQUARE_SIZE + GRID_GAP_SIZE);
	itemImage->locationRect.y = row*(GRID_SQUARE_SIZE + GRID_GAP_SIZE);
	if (SDL_BlitSurface(itemImage->surface, &itemImage->imgRect,
		gridPanel->surface, &itemImage->locationRect) != 0){
			sdlErrorPrint("failed to blit image");
			return;
	}
}


/**** layout functions - used for setting/ updating the layout of the view ****/

 /* setThreePartLayout will set the layout of play game and world builder,
  * including the window, the top panel, the side panel and the grid  */
 void setThreePartLayout(ViewStateref viewState, char ** gameGridData){
	/* create a reference to the ThreePartLayoutViewExt */
	ThreePartViewExtRef threePartView = viewState->ViewExt;

	 /* create the window and add it to UI tree */
 	Widget *win = createWindow(WIN_W,WIN_H, WINDOW_RED, WINDOW_GREEN, WINDOW_BLUE);
 	if (win == NULL){
 		return;
 	}
 	ListRef winNode = newList(win);
 	viewState->UITree = winNode;
 	if (winNode == NULL){
 		freeWidget(win);
 		return;
 	}
 	/* create the top panel and add it the the UI tree */
 	Widget *topPanel = create_panel(0, 0, WIN_W, TOP_PANEL_H,PANEL_RED,PANEL_GREEN,PANEL_BLUE);
 	if (topPanel == NULL){
 		return;
 	}
 	ListRef topPanelNode = addChildNode(winNode, topPanel);
 	if (topPanelNode == NULL){
 		freeWidget(topPanel);
 		return;
 	}
 	threePartView->topPanelNode = topPanelNode;
 	/* create the side panel and add it the the UI tree */
 	Widget *sidePanel = create_panel(0, 210, SIDE_PANEL_W, SIDE_PANEL_H,PANEL_RED,PANEL_GREEN,PANEL_BLUE);
 	if (sidePanel == NULL){
 		return;
 	}
 	ListRef sidePanelNode = addChildNode(winNode, sidePanel);
 	if (sidePanelNode == NULL){
 		freeWidget(sidePanel);
 		return;
 	}
 	threePartView->sidePanelNode = sidePanelNode;
 	/* create the grid panel and add it the the UI tree */
 	Widget *gridPanel = create_panel(210, 210, GRID_SIZE, GRID_SIZE, PANEL_RED, PANEL_GREEN, PANEL_BLUE);
 	if (gridPanel == NULL){
 		return;
 	}
 	threePartView->gridPanel = gridPanel;
 	ListRef gridPanelNode = addChildNode(winNode, gridPanel);
 	if (gridPanelNode == NULL){
 		freeWidget(gridPanel);
 		return;
 	}
 	/* update the grid by data the grid data*/
 	createGridByData(gridPanel, gameGridData, threePartView->gridItemsImgArr);
 }

 /* addButtonsToSidePanel sets the side panel buttons of play game and world builder
  * and adds them to the view state */
void addButtonsToSidePanel(ViewStateref viewState, int buttonImgX, int buttonImgY,
		int buttonImgDisX, int buttonImgDisY, int fromButtonNum, int toButtonNum, int isDisabled){
	/* create a reference to the ThreePartLayoutViewExt */
	ThreePartViewExtRef threePartView = viewState->ViewExt;
	/* get the x and y initial values of the buttons, relative to the side panel */
	int sideButtonX = calcSideButtonX(), sideButtonY = calcSideButtonY();
	/* Add buttons to buttons array and to UI tree */
	for (int i = fromButtonNum; i < toButtonNum; i++){
		viewState->buttonsArr[i] = create_button(sideButtonX, sideButtonY, PANEL_BUTTON_W, PANEL_BUTTON_H,
				viewState->image, buttonImgDisX, buttonImgDisY, buttonImgX, buttonImgY, isDisabled);
		if (viewState->buttonsArr[i] == NULL){
			return;
		}
		ListRef newButtonNode = addChildNode(threePartView->sidePanelNode, viewState->buttonsArr[i]);
		if (newButtonNode == NULL){
			freeWidget(viewState->buttonsArr[i]);
			return;
		}
		buttonImgY += PANEL_BUTTON_H; /* update the location of the next button (not disable state) in the  image file */
		buttonImgDisY += PANEL_BUTTON_H; /* update the location of the next button (disabled state) in the  image file */
		sideButtonY += PANEL_BUTTON_H + PANEL_WIDGET_Y_GAP; /* update the location of nect button relative to top panel*/
	}
}

/* addButtonsToWBTopPanel sets the top panel buttons of world builder
 * and adds them to the view state */
 void addButtonsToWBTopPanel(ViewStateref viewState, int buttonImgX, int buttonImgY,
		int buttonImgDisX, int buttonImgDisY, int fromButtonNum, int toButtonNum, int isDisabled){
	/* create a reference to the ThreePartLayoutViewExt */
	ThreePartViewExtRef threePartView = viewState->ViewExt;
	/* get the x and y initial values of the buttons, relative to the top panel */
	int topButtonX = calcTopButtonX(), topButtonY = calcTopButtonY();
	/* go over each button and add it to the button array and to UI tree */
	for (int i = 0; i < WB_TOP_PANEL_NUM_BUTTONS; i++){
		viewState->buttonsArr[i] = create_button(topButtonX, topButtonY, PANEL_BUTTON_W, PANEL_BUTTON_H,
				viewState->image, buttonImgX, buttonImgY, buttonImgX, buttonImgY, 0);
		if (viewState->buttonsArr[i] == NULL){
			return;
		}
		ListRef newButtonNode = addChildNode(threePartView->topPanelNode, viewState->buttonsArr[i]);
		if (newButtonNode == NULL){
			freeWidget(viewState->buttonsArr[i]);
			return;
		}
		buttonImgY += PANEL_BUTTON_H; /* update the location of the next button in the  image file */
		topButtonX += PANEL_BUTTON_W + WB_BUTTON_X_GAP; /* update the x location of the next button relative to top panel */
	}
}


 /**** MVP free data functions ****/

 /* free view state */
 void freeViewState(ViewStateref guiViewState){
 	if (guiViewState != NULL){
 		if (guiViewState->buttonsArr != NULL)
 			free(guiViewState->buttonsArr); /* free menu buttons array - the buttons will be freed from UI tree */
 		if (guiViewState->labelArr != NULL)
 			free(guiViewState->labelArr); /* free menu labels array (for play game) - the labels will be freed from UI tree */
 		/* free gui's image and bgImage surfaces */
 		if (guiViewState->image != NULL)
 			SDL_FreeSurface(guiViewState->image);
 		if (guiViewState->bgImage != NULL)
 			SDL_FreeSurface(guiViewState->bgImage);
 		if (guiViewState->UITree != NULL)
 			freeTree(guiViewState->UITree, freeWidget); /* free all widgets by freeing the UItree */
 		if (guiViewState->ViewExt != NULL) /* free the view extentsion */
 			free(guiViewState->ViewExt);
 		free(guiViewState);
 	}
 }

 /* Free the grid data in the three part extension of the view state.
  * The rest of the data (gridPnael, top/side panel nodes) will be freed when freeing the UITree */
void freeThreePartExtViewState(ThreePartViewExtRef threePartView){
	if (threePartView->gridItemImage != NULL)
		SDL_FreeSurface(threePartView->gridItemImage); /* free gridItemImage if needed (play game and world builder */
	if (threePartView->gridItemsImgArr != NULL)
		freeGridItemsArray(threePartView->gridItemsImgArr); /* free gridItemsImgArr, including the items (they are not in the tree)*/
}



 /* free the array of grid items images, including the images widgets */
 void freeGridItemsArray(Widget ** gridItemsImages){
 	for (int i = 0; i < NUM_GRID_ITEMS; i++)
 		freeWidget(gridItemsImages[i]);
 	free(gridItemsImages);
 }

 /* free Menu Data */
 void freeMenuData(MenuDataRef menuData){
 	if (menuData != NULL){
 		freeGridData(menuData->gameGridData);
 		free(menuData);
 	}
 }

 /* free world builder data - with or without the grid */
 void freeWBData(WBDataRef wbData, int doFreeGrid){
 	if (wbData != NULL){
 		if (doFreeGrid)
 			freeGridData(wbData->gameGridData);
 		free(wbData);
 	}
 }

 /* free play game data - with or without the grid */
 void freePGData(PGDataRef pgData, int doFreeGrid){
 	if (pgData != NULL){
 		if (doFreeGrid)
 			freeGridData(pgData->gameGridData);
 		free(pgData);
 	}
 }


/**** play game specific functions ****/

/*** play game layout functions ***/


/* setGameOver will update the model that the game is over and
* will update the view of the top panel and side panel accordingly
* will also deselcet the current player, so no player is selected */
void setGameOver(PGDataRef pgModel, ViewStateref pgViewState){
	/* create a reference to the ThreePartLayoutViewExt */
	ThreePartViewExtRef threePartView = pgViewState->ViewExt;
	/* update model and view */
	pgModel->isGameOver = 1;
	setTopPanelGameOver(pgModel, pgViewState);
	if (isError) { return; }
	enablePGSidePanelButtons(pgViewState);
	if (isError) { return; }
	deselectGridPos(threePartView->gridPanel, threePartView->gridItemsImgArr, *getCurrPlayerPos(pgModel));
	if (isError) { return; }
	blitUpToWindow(threePartView->sidePanelNode->data);
	if (isError) { return; }
	blitUpToWindow(threePartView->topPanelNode->data);
	if (isError) { return; }
	blitUpToWindow(threePartView->gridPanel);
}


/* setTopPanelGameOver is used to set the top panel label for once the game is over
 * and updating the view */
void setTopPanelGameOver(PGDataRef pgModel, ViewStateref pgViewState){
	/* create a reference to the ThreePartLayoutViewExt */
	ThreePartViewExtRef threePartView = pgViewState->ViewExt;
	/* free all nodes in UI tree that are under topPanelNode) */
	freeDescendants(threePartView->topPanelNode, freeWidget);
	pgViewState->buttonsArr[0] = NULL;
	clearPanel(threePartView->topPanelNode->data); /* clear the panel from previous items */
	if(isError)
		return;
	/* create the game over label, and add  it to UI tree */
	Widget * gameOverLabel = create_image(calcGameOverLabelX(), calcGameOverLabelY(), GAME_OVER_LABEL_W, GAME_OVER_LABEL_H,
			pgViewState->image, 2*PANEL_BUTTON_W+2*DIGIT_LABEL_W, 4*STATE_LABEL_H+pgModel->gameOverType*GAME_OVER_LABEL_H);
	ListRef gameOverLabelNode = addChildNode(threePartView->topPanelNode, gameOverLabel);
	if (gameOverLabelNode == NULL){
		freeWidget(gameOverLabel);
		return;
	}
	/* run DFS on UI tree starting from topPanelNode */
	treeDFS(threePartView->topPanelNode, calcAbsWidgetXY, addChildWidgetsToParent);
}

/* set the view and model for playing the game when starting/restarting the game */
void setPlayGame(ViewStateref pgViewState, PGDataRef pgModel){
	/* create a reference to the ThreePartLayoutViewExt */
	ThreePartViewExtRef threePartView = pgViewState->ViewExt;

	pgModel->isGameOver = 0; /* update model */
	setTopPanelPlayGame(pgModel, pgViewState);
	if (isError) { return; }
	disablePGSidePanelButtons(pgViewState);
	if (isError) { return; }
	selectGridPos(threePartView->gridPanel, threePartView->gridItemsImgArr, *getCurrPlayerPos(pgModel));
}

/* updatePlayGame will update the play game view and model, and will update the current player selected
 * when the game is played */
void updatePlayGame(ViewStateref pgViewState, PGDataRef pgModel, gridItemPosition * eventPos){
	/* create a reference to the ThreePartLayoutViewExt */
	ThreePartViewExtRef threePartView = pgViewState->ViewExt;

	pgModel->isCatCurrPlayer = 1-pgModel->isCatCurrPlayer; /* change current player */
	/* update top panel */
	updateTopPanelPlayGame(pgViewState, pgModel);
	blitUpToWindow(threePartView->topPanelNode->data);
	if (isError) { return; }
	/* update the grid position selected and update the view */
	changeSelectedGridSquare(threePartView->gridPanel, threePartView->gridItemsImgArr,
			eventPos, *getCurrPlayerPos(pgModel));
}


/* setTopPanelPlayGame is used to set the top panel labels and button once
 * we enter a play game state and updates the view */
void setTopPanelPlayGame(PGDataRef pgModel, ViewStateref pgViewState){
	/* create a reference to the ThreePartLayoutViewExt */
	ThreePartViewExtRef threePartView = pgViewState->ViewExt;

	freeDescendants(threePartView->topPanelNode, freeWidget); /* free all nodes in UI tree that are under topPanelNode) */
	clearPanel(threePartView->topPanelNode->data); /* clear the panel from previous items */
	if(isError)
		return;
	/* set first row labels - which player and num turns left: */
	/* set player label and add to UI tree */
	Widget * playerMoveLabel = create_image(calcMoveLabelX(pgModel->isCatCurrPlayer), PANEL_WIDGET_Y_GAP, GAME_INFO_LABEL_W,
			GAME_INFO_LABEL_H, pgViewState->image, 2*PANEL_BUTTON_W+2*DIGIT_LABEL_W, 4*STATE_LABEL_H+3*GAME_OVER_LABEL_H);
	if (playerMoveLabel == NULL)
		return;
	pgViewState->labelArr[0] = playerMoveLabel;
	ListRef moveLabelNode = addChildNode(threePartView->topPanelNode, playerMoveLabel);
	if (moveLabelNode == NULL){
		freeWidget(playerMoveLabel);
		return;
	}
	/* set num turns tens label and add to UI tree */
	Widget * turnsTensLabel = create_image(calcTensLabelX(pgModel->isCatCurrPlayer), PANEL_WIDGET_Y_GAP,
			DIGIT_LABEL_W,GAME_INFO_LABEL_H, pgViewState->image, 2*PANEL_BUTTON_W, 3*PAUSE_BUTTON_H);
	if (turnsTensLabel == NULL)
		return;
	pgViewState->labelArr[1] = turnsTensLabel;
	ListRef turnsTensLabelNode = addChildNode(threePartView->topPanelNode, turnsTensLabel);
	if (turnsTensLabelNode == NULL){
		freeWidget(turnsTensLabel);
		return;
	}
	/* set num turns units label and add to UI tree */
	Widget * turnsUnitsLabel = create_image(calcUnitsLabelX(pgModel->isCatCurrPlayer), PANEL_WIDGET_Y_GAP, DIGIT_LABEL_W,
			GAME_INFO_LABEL_H, pgViewState->image, 2*PANEL_BUTTON_W+DIGIT_LABEL_W, 3*PAUSE_BUTTON_H);
	if (turnsUnitsLabel == NULL)
		return;
	pgViewState->labelArr[2] = turnsUnitsLabel;
	ListRef turnsUnitsLabelNode = addChildNode(threePartView->topPanelNode, turnsUnitsLabel);
	if (turnsUnitsLabelNode == NULL){
		freeWidget(turnsUnitsLabel);
		return;
	}
	/* set the second row label and add to UI tree*/
	Widget * playerStateLabel = create_image(calcStateLabelX(), calcStateLabelY(), STATE_LABEL_W,
			STATE_LABEL_H, pgViewState->image, PAUSE_BUTTON_W, 0);
	if (playerStateLabel == NULL)
		return;
	pgViewState->labelArr[3] = playerStateLabel;
	ListRef stateLabelNode = addChildNode(threePartView->topPanelNode, playerStateLabel);
	if (stateLabelNode == NULL){
		freeWidget(playerStateLabel);
		return;
	}
	/* set the pause button and add to UI tree*/
	Widget * pauseButton = create_button(calcPauseButtonX(), calcPauseButtonY(), PAUSE_BUTTON_W,
			PAUSE_BUTTON_H, pgViewState->image, 0, 0, 0, 0, 0);
	if (pauseButton == NULL)
		return;
	pgViewState->buttonsArr[0] = pauseButton;
	ListRef pauseButtonNode = addChildNode(threePartView->topPanelNode, pauseButton);
	if (pauseButtonNode == NULL){
		freeWidget(pauseButton);
		return;
	}
	/* get the correct image location for each widget and run DFS on UI tree starting from topPanelNode */
	updateTopPanelPlayGame(pgViewState, pgModel);
}

/* updateTopPanelPlayGame is used after a player turn, when the game is not over, for updating the top panel widgets
 * image location in the image file according the data in the model, and running
 *  DFS on UI tree starting from topPanelNode for updating the view*/
void updateTopPanelPlayGame(ViewStateref pgViewState, PGDataRef pgModel){
	/* create a reference to the ThreePartLayoutViewExt */
	ThreePartViewExtRef threePartView = pgViewState->ViewExt;

	/* update the labels and buttons */
	setPlayerMoveLabel(pgModel, pgViewState);
	setNumTurnsLabels(pgModel, pgViewState);
	setPlayerStateLabel(pgModel, pgViewState);
	setPauseButton(pgModel, pgViewState);
	treeDFS(threePartView->topPanelNode, calcAbsWidgetXY, addChildWidgetsToParent);
}

/* setPauseButton is used for updating the pause button image
 * according to the game state */
void setPauseButton(PGDataRef pgModel, ViewStateref pgViewState){
	Widget * pauseButton = pgViewState->buttonsArr[0];
	if (pgModel->isGamePaused){ /* will set "Resume Game (Space)" button text */
		pauseButton->buttonNonSelectedRect.y = 2*PAUSE_BUTTON_H;
		pauseButton->buttonSelectedRect.y = 2*PAUSE_BUTTON_H;
	}
	else{
		if (isCurrPlayerHuman(pgModel)){ /* will set "Pause (Space)" button text */
			pauseButton->buttonNonSelectedRect.y = PAUSE_BUTTON_H;
			pauseButton->buttonSelectedRect.y = PAUSE_BUTTON_H;
		}
		else{
			pauseButton->buttonNonSelectedRect.y = 0; /* will set "Pause Before Next Move (Space)" button text */
			pauseButton->buttonSelectedRect.y = 0;
		}
	}
	pauseButton->imgRect = pauseButton->buttonNonSelectedRect; /* update img_rect of button */
}

/* setNumTurnsLabels is used for updating the number of turns labels images
 * according to the game state */
void setNumTurnsLabels(PGDataRef pgModel, ViewStateref pgViewState){
	/* get the tens and the units labels */
	Widget * turnsTensLabel = pgViewState->labelArr[1];
	Widget * turnsUnitsLabel = pgViewState->labelArr[2];
	int turnsTensDigit = pgModel->numTurnsLeft/10; /* get the tens digit out of numturnsleft */
	int turnsUnitsDigit = pgModel->numTurnsLeft%10; /* get the units digit out of numturnsleft */
	int yStartPoint = 3*PAUSE_BUTTON_H;/*y start point of the location of the numbers in the image file */
	/* update the img_rect of each digit */
	turnsUnitsLabel->imgRect.y = yStartPoint + turnsUnitsDigit*GAME_INFO_LABEL_H;
	turnsTensLabel->imgRect.y = yStartPoint + turnsTensDigit*GAME_INFO_LABEL_H;
}

/* setPlayerMoveLabel is used for updating the player's move label
 * according to the game state */
void setPlayerMoveLabel(PGDataRef pgModel, ViewStateref pgViewState){
	Widget * moveLabel = pgViewState->labelArr[0];
	int yStartPoint = 4*STATE_LABEL_H+3*GAME_OVER_LABEL_H; /*y value of the location of the players' labels in the image file */
	if (pgModel->isCatCurrPlayer)
		moveLabel->imgRect.y = yStartPoint; /* will set "cat move" text */
	else
		moveLabel->imgRect.y = yStartPoint + GAME_INFO_LABEL_H; /* will set "mouse move" text */
}

/* setPlayerStateLabel is used for updating the player's state label
 * according to the game state */
void setPlayerStateLabel(PGDataRef pgModel, ViewStateref pgViewState){
	int yStartPoint = 0;
	Widget * label = pgViewState->labelArr[3];
	if (isCurrPlayerHuman(pgModel)){ /* curr player is human*/
		if (pgModel->isGamePaused) /* game is paused */
			label->imgRect.y = yStartPoint + STATE_LABEL_H; /* will set "Human – paused" text */
		else /* game is not paused */
			label->imgRect.y = yStartPoint; /* will set "Human – waiting for next move" text */
	}
	else{ /* current player is machine */
		if (pgModel->isGamePaused)
			label->imgRect.y = yStartPoint + 3*STATE_LABEL_H;/* will set "Machine – paused" text */
		else
			label->imgRect.y = yStartPoint + 2*STATE_LABEL_H;/* will set "Machine – computing" text */
	}
}

/*** play game get data functions ***/

/* getPGButtonNum goes over the different key strokes available in Play game
 * and returns the button number that is related to them */
int getPGButtonNum(SDLKey key){
	switch(key){
		case(SDLK_SPACE):
			return 0;
		case(SDLK_F1):
			return 1;
		case(SDLK_F2):
			return 2;
		case(SDLK_F3):
			return 3;
		case(SDLK_F4):
			return 4;
		case(SDLK_ESCAPE):
			return 5;
		default:
			return -1;
	}
}

/* get the position of the current player */
gridItemPosition * getCurrPlayerPos(PGDataRef pgModel){
	return pgModel->isCatCurrPlayer ? &pgModel->catPos : &pgModel->mousePos;
}

/* checkAndupdateGameOverType checks if the games is over, and updates the type of the game over state.
 * return 1 if the game is over, 0 otherwise */
int checkAndupdateGameOverType(PGDataRef pgModel){
	/* first check the game over type */
	gameOverType type = checkGameOverType(pgModel->catPos, pgModel->mousePos, pgModel->cheesePos, pgModel->numTurnsLeft);
	pgModel->gameOverType = type; /* update the model with the game over type */
	if (type == GAME_NOT_OVER) /* if the type is GAME_NOT_OVER return 0, otherwise return 1 */
		return 0;
	return 1;
}

/* check if current player is human, if so return 1, otherwise return 0 */
int isCurrPlayerHuman(PGDataRef pgModel){
	if ((pgModel->isCatCurrPlayer && pgModel->isCatHuman) ||
			(!pgModel->isCatCurrPlayer && pgModel->isMouseHuman))
		return 1;
	return 0;
}

/*** play game make move functions ***/

/* makeGameMoveIfLegal will check if the game is currently played, and if the new move position in the grid is valid
 * if it's valid it will make the move, and then update the top panel */
void makeGameMoveIfLegal(ViewStateref pgViewState, PGDataRef pgModel, gridItemPosition eventPos){
	/* create a reference to the ThreePartLayoutViewExt */
	ThreePartViewExtRef threePartView = pgViewState->ViewExt;

	if (!pgModel->isGamePaused && !pgModel->isGameOver){  /* if the game is not paused or over */
		gridItemPosition * currPlayerPos = getCurrPlayerPos(pgModel); /* get the position of the current player */
		gridItem currPlayerType = pgModel->isCatCurrPlayer ? CAT : MOUSE; /* get the current player type */
		if (isMoveValid(pgModel->gameGridData, *currPlayerPos, eventPos)){ /* check if the move is valid */
			/* make the move: */
			moveItemToPos(currPlayerType, threePartView->gridItemsImgArr, threePartView->gridPanel,
				pgModel->gameGridData, eventPos, currPlayerPos);
			if (isError)
				return;
			/* update num turns left: */
			pgModel->numTurnsLeft -= 1;
			/* check if game is over: */
			if (checkAndupdateGameOverType(pgModel)) /* game is over */
				setGameOver(pgModel, pgViewState); /* update model and view state for game over */
			else /* game is not over */
				updatePlayGame(pgViewState, pgModel, &eventPos); /* change current player and update the view accordingly */
		}
		else{ /* move is illegal */
			warnIllegalMove(pgViewState, eventPos, *currPlayerPos); /* put a warning on the screen that the move is illegal */
		}
	}
}

/* get move direction by arrow direction, and call make move if legal */
void makeGameMoveByArrowIfLegal(ViewStateref pgView, PGDataRef pgModel, direction direction){
	gridItemPosition currPlayerPos = pgModel->isCatCurrPlayer ? pgModel->catPos : pgModel->mousePos; /* get curr player pos */
	gridItemPosition eventPos = getPosByDirection(currPlayerPos, direction); /* get move position by direction */
	makeGameMoveIfLegal(pgView, pgModel, eventPos); /* make the move if legal */
}

/* warnIllegalMove puts a warning on the screen that the move (eventPos) is illegal */
void warnIllegalMove(ViewStateref pgViewState, gridItemPosition eventPos, gridItemPosition currPlayerPos){
	/* create a reference to the ThreePartLayoutViewExt */
	ThreePartViewExtRef threePartView = pgViewState->ViewExt;

	/* first put a warning on the problematic area */
	blitItemToGrid(threePartView->gridPanel, threePartView->gridItemsImgArr[WARN], eventPos.row, eventPos.col);
	blitUpToWindow(threePartView->gridPanel);
	SDL_Delay(WARN_DELAY); /* wait */
	if (isSamePos(eventPos, currPlayerPos)) /* if warning is on curr player position, restore selection */
		blitItemToGrid(threePartView->gridPanel, threePartView->gridItemsImgArr[SELECT], eventPos.row, eventPos.col);
	else /* warning is not on curr player positions, remove warning */
		blitItemToGrid(threePartView->gridPanel, threePartView->gridItemsImgArr[DESELECT], eventPos.row, eventPos.col);
	blitUpToWindow(threePartView->gridPanel); /* update view */
}

/* updateMachineMoveIfNeeded will make a machine move, if it is a machine move turn
 * will be called only when current gui's stateId is PLAY_GAME*/
void updateMachineMoveIfNeeded(GUI pgGUI){
	PGDataRef pgModel = pgGUI.model; /* get the play game gui model */
	if (isCurrPlayerHuman(pgModel) || pgModel->isGameOver || pgModel->isGamePaused) /* not a machine turn */
		return;
	ViewStateref pgViewState = pgGUI.viewState; /* get the view state */
	GameStateRef currState = (GameStateRef)malloc(sizeof(GameState)); /* allocate memory for currState  */
	if (currState == NULL){ /* malloc failed */
		perrorPrint("malloc");
		return;
	}
	/* update curr state by play game model data  */
	int currPlayerSkill = pgModel->isCatCurrPlayer ? pgModel->catSkill : pgModel->mouseSkill;
	currState->gridData = copyGrid(pgModel->gameGridData);
	currState->catPos = pgModel->catPos;
	currState->mousePos = pgModel->mousePos;
	currState->cheesePos = pgModel->cheesePos;
	currState->isCatCurrPlayer = pgModel->isCatCurrPlayer;
	currState->numTurnsLeft = pgModel->numTurnsLeft;
	currState->isMaxPlayer = 1; /* will always call the function with max player */
	/* get the move by suggest move function (which uses MiniMax algorithm */
	gridItemPosition errorPos = {-1, -1};
	gridItemPosition movePos = suggestMove(currState, currPlayerSkill);
	if (isSamePos(movePos, errorPos)){ /* there was an error while getting the suggested move */
		freeState(currState); /* free memory */
		return;
	}
	SDL_Delay(1000); /* wait before making the move */
	makeGameMoveIfLegal(pgViewState, pgModel,movePos);
	freeState(currState); /* free memory */
}

/*** play game change state functions ***/

/* restartGame restarts the game */
void restartGame(ViewStateref pgViewState, PGDataRef pgModel){
	/* create a reference to the ThreePartLayoutViewExt */
	ThreePartViewExtRef threePartView = pgViewState->ViewExt;

	/* free the grid data of previous game and get the current grud data, curr player and num turns left */
	freeGridData(pgModel->gameGridData);
	pgModel->gameGridData = initGameDataByFile(pgModel->loadGameWorld, &pgModel->numTurnsLeft,
			&pgModel->isCatCurrPlayer);
	if (pgModel->gameGridData == NULL)
		return;
	/* update cat, mouse and cheese position in model */
	updateItemsPositions(&pgModel->mousePos,&pgModel->catPos,&pgModel->cheesePos, pgModel->gameGridData);
	/* update the new grid in the view */
	createGridByData(threePartView->gridPanel, pgModel->gameGridData, threePartView->gridItemsImgArr);
	if (isError)
		return;
	if (checkAndupdateGameOverType(pgModel)){ /* game is over */
		setGameOver(pgModel, pgViewState);
	}
	else{ /* game is not over */
		pgModel->isGamePaused=0;
		setPlayGame(pgViewState, pgModel);
	}
	/* redraw entire gui */
	drawGui(pgViewState->UITree);
}

/* set the game in paused state, update top panel and enable side panel buttons */
void pauseGame(ViewStateref pgViewState, PGDataRef pgModel){
	/* create a reference to the ThreePartLayoutViewExt */
	ThreePartViewExtRef threePartView = pgViewState->ViewExt;

	pgModel->isGamePaused = 1; /* update the model */
	updateTopPanelPlayGame(pgViewState, pgModel);
	if (isError) { return; }
	enablePGSidePanelButtons(pgViewState); /* enable side panel buttons */
	if (isError) { return; }
	/* blit the changed to the window */
	blitUpToWindow(threePartView->topPanelNode->data);
	if (isError) { return; }
	blitUpToWindow(threePartView->sidePanelNode->data);
}

/* resume the game, update top panel and disable side panel buttons */
void resumeGame(ViewStateref pgViewState, PGDataRef pgModel){
	/* create a reference to the ThreePartLayoutViewExt */
	ThreePartViewExtRef threePartView = pgViewState->ViewExt;

	pgModel->isGamePaused = 0; /* update the model */
	updateTopPanelPlayGame(pgViewState, pgModel);
	if (isError) { return; }
	disablePGSidePanelButtons(pgViewState); /* disable side panel buttons */
	if (isError) { return; }
	/* blit the changed to the window */
	blitUpToWindow(threePartView->topPanelNode->data);
	if (isError) { return; }
	blitUpToWindow(threePartView->sidePanelNode->data);
}

/* set the buttons of the play game side panel to be disabled */
void disablePGSidePanelButtons(ViewStateref pgViewState){
	for (int i = PG_TOP_PANEL_NUM_BUTTONS; i< PG_NUM_BUTTONS; i++){
		setButtonDisabled(pgViewState->buttonsArr[i]);
		blitChildToParentWidget(pgViewState->buttonsArr[i], pgViewState->buttonsArr[i]->parentWidget);
	}
}

/* set the buttons of the play game side panel to be enabled */
void enablePGSidePanelButtons(ViewStateref pgViewState){
	for (int i = PG_TOP_PANEL_NUM_BUTTONS; i< PG_NUM_BUTTONS; i++){
		setButtonEnabled(pgViewState->buttonsArr[i]);
		blitChildToParentWidget(pgViewState->buttonsArr[i], pgViewState->buttonsArr[i]->parentWidget);
	}
}


/**** world builder specific helper functions ****/

/*** world builder get data functions ***/

/* getWBButtonNum goes over the different key strokes available in world builder
 * and returns the button number that is related to them */
int getWBButtonNum(SDLKey key){
	switch(key){
		case(SDLK_s):
			return 0;
		case(SDLK_F1):
			return 1;
		case(SDLK_ESCAPE):
			return 2;
		case(SDLK_m):
			return 3;
		case(SDLK_c):
			return 4;
		case(SDLK_p):
			return 5;
		case(SDLK_w):
			return 6;
		case(SDLK_SPACE):
			return 7;
		default:
			return -1;
	}
}


/* isGridInvalid is used by world builder to detect which item/s (cat/mouse/cheese) is missing from the grid
 * and updates the model accordingly. returns 1 if the grid is invalid, 0 otherwise */
int isGridInvalid(WBDataRef wbModel){
	gridItemPosition invalidPos = {-1,-1}; /* the position of an item that is missing from the grid */
	int missingItems = 0, mouse = 1, cat = 2, cheese = 4;
	int isInvalid = 0;
	if (isSamePos(wbModel->catPos, invalidPos)){ /* cat is missing */
		isInvalid = 1;
		missingItems |= cat;
	}
	if (isSamePos(wbModel->mousePos, invalidPos)){ /* mouse is missing */
		isInvalid = 1;
		missingItems |= mouse;
	}
	if (isSamePos(wbModel->cheesePos, invalidPos)){ /* cheese is missing */
		isInvalid = 1;
		missingItems |= cheese;
	}
	wbModel->missingItems = missingItems; /* update the model with the missing items */
	return isInvalid;
}

/*** world builder grid functions ***/

/* putGridItemInPos is used by world builder to put a grid  item (cat, mouse, cheese, wall, empty space)
 * in a certain grid position, according to the world builder side button pressed */
void putGridItemInPos(WBDataRef wbModel, Widget * gridPanel, Widget ** gridItemsImages,
		gridItemPosition currPos, int buttonNum){
	gridItem buttonsItems[5] = {MOUSE, CAT, CHEESE, WALL, EMPTY}; /* an array of the types of the items we can put */
	gridItem itemType = buttonsItems[buttonNum-WB_TOP_PANEL_NUM_BUTTONS]; /* get the item type by the button number */

	if (itemType == WALL || itemType == EMPTY){ /* the item is a reusable item (can be put more than once on the grid */
		addReusableItemToPos(itemType, wbModel->gameGridData, gridPanel, gridItemsImages, currPos);
	}
	else{ /* item is MOUSE, CAT or CHEESE - need to move the item from one position to the other*/
		gridItemPosition * itemPosRef = NULL;
		if(itemType == MOUSE){
			itemPosRef = &wbModel->mousePos;
			if (wbModel-> editedWorld == 0){
				/* if we edited a new world, update who will be the first player, by the last player added */
				wbModel->isCatFirst = 0;
			}
		}
		else if (itemType == CAT){
			itemPosRef = &wbModel->catPos;
			if (wbModel-> editedWorld == 0){
				/* if we edited a new world, update who will be the first player, by the last player added */
				wbModel->isCatFirst = 1;
			}
		}
		else /* itemType == CHEESE */
			itemPosRef = &wbModel->cheesePos;
		/* move the non reusable item from the old position to its new position */
		moveItemToPos(itemType, gridItemsImages, gridPanel, wbModel->gameGridData, currPos, itemPosRef);
	}
	fixOverride(itemType, wbModel, currPos); /* fix items positions, in case cat/mouse/cheese were overriden by the new item */
	selectGridPos(gridPanel, gridItemsImages, currPos); /* re-add the selection image on the position */
	blitUpToWindow(gridPanel); /* update the view */
}


/* addReusableItemToPos is used by world builder for adding reusable grid items to the grid
 * (wall or empty space) */
void addReusableItemToPos(gridItem itemType, char ** gridData, Widget * gridPanel,
		Widget ** gridItemsImages, gridItemPosition currPos){
	Widget * itemImage = gridItemsImages[itemType];
	blitItemToGrid(gridPanel, itemImage, currPos.row, currPos.col);
	if (itemType == WALL)
		gridData[currPos.row][currPos.col] = WALL_CHAR;
	else
		gridData[currPos.row][currPos.col] = EMPTY_CELL_CHAR;
}

/* moveItemToPos moves a non-reusable (cat/mouse/cheese) item from its old position to its new position */
void moveItemToPos(gridItem itemType, Widget ** gridItemsImages, Widget * gridPanel, char ** gridData,
		gridItemPosition currPos, gridItemPosition * prevItemPos){
	gridItemPosition noPos = {-1,-1};
	if (!isSamePos(*prevItemPos, noPos)){/* if the item was on the grid before */
		blitItemToGrid(gridPanel, gridItemsImages[EMPTY], prevItemPos->row, prevItemPos->col); /* add item to the grid */
		if (isError)
			return;
		gridData[prevItemPos->row][prevItemPos->col] = getItemChar(EMPTY); /* set the grid data to be empty for the prev pos */
	}
	/* update the item position to it's new position */
	prevItemPos->row = currPos.row;
	prevItemPos->col = currPos.col;
	gridData[currPos.row][currPos.col] = getItemChar(itemType);/* set the grid data to the correct item char for the new pos */
	blitItemToGrid(gridPanel, gridItemsImages[itemType], currPos.row, currPos.col); /* blit the item to the grid in the new pos*/
}

/* fixOverride is used by world builder fixes cat/mouse/cheese items positions to be out of the grid
 * in case cat/mouse/cheese were overriden by the new item
 * item type is the item that was now put on the grid */
void fixOverride(gridItem itemType, WBDataRef wbModel, gridItemPosition currPos){
	gridItemPosition noPos = {-1,-1};
	if (itemType != CAT && isSamePos(currPos, wbModel->catPos)) /* if item type is not cat, but was put on the cat pos */
		wbModel->catPos = noPos;
	if (itemType != MOUSE && isSamePos(currPos, wbModel->mousePos)) /* if item type is not mouse, but was put on the mouse pos */
		wbModel->mousePos = noPos;
	if (itemType != CHEESE && isSamePos(currPos, wbModel->cheesePos))  /* if item type is not cheese, but was put on the cheese pos */
		wbModel->cheesePos = noPos;
}

/* changeSelectedPosByArrow is used by world builder to change the selected position for an arrow strike */
void changeSelectedPosByArrow(Widget * gridPanel, Widget ** gridItemsImages,
		gridItemPosition * currPos, direction direction){
	gridItemPosition newPos = getPosByDirection(*currPos, direction); /* get the new position */
	changeSelectedGridSquare(gridPanel, gridItemsImages, currPos, newPos); /* change the current selected grid square */
}



