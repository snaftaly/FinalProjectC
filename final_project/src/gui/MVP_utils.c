/* includes */

#include "MVP_utils.h"
#include "../main/ErrorHandling.h"

GUI createGUIForState(StateId stateId){
	GUI returnGUI;
	returnGUI.stateId = stateId;
	returnGUI.model = NULL;
	returnGUI.viewState = NULL;
	switch(stateId){
		case(MAIN_MENU):
			returnGUI.start = startMainMenu;
			returnGUI.viewTranslateEvent = mainMenuVTE;
			returnGUI.presenterHandleEvent = mainMenuPHE;
			returnGUI.stop = menuStop;
			break;
		case(CHOOSE_CAT):
			returnGUI.start = startChooseAnimal;
			returnGUI.viewTranslateEvent = chooseAnimalVTE;
			returnGUI.presenterHandleEvent = chooseCatPHE;
			returnGUI.stop = menuStop;
			break;
		case(CHOOSE_MOUSE):
			returnGUI.start = startChooseAnimal;
			returnGUI.viewTranslateEvent = chooseAnimalVTE;
			returnGUI.presenterHandleEvent = chooseMousePHE;
			returnGUI.stop = menuStop;
			break;
		case(CAT_SKILL):
			returnGUI.start = startAnimalSkill;
			returnGUI.viewTranslateEvent = complexMenuVTE;
			returnGUI.presenterHandleEvent = catSkillPHE;
			returnGUI.stop = menuStop;
			break;
		case(MOUSE_SKILL):
			returnGUI.start = startAnimalSkill;
			returnGUI.viewTranslateEvent = complexMenuVTE;
			returnGUI.presenterHandleEvent = mouseSkillPHE;
			returnGUI.stop = menuStop;
			break;
		case(LOAD_GAME):
			returnGUI.start = startWorldMenu;
			returnGUI.viewTranslateEvent = complexMenuVTE;
			returnGUI.presenterHandleEvent = loadGamePHE;
			returnGUI.stop = menuStop;
			break;
		case(WORLD_BUILDER):
			returnGUI.start = startWorldBuilder;
			returnGUI.viewTranslateEvent = worldBuilderVTE;
			returnGUI.presenterHandleEvent = worldBuilderPHE;
			returnGUI.stop = stopWorldBuilder;
			break;
		case(EDIT_GAME):
			returnGUI.start = startWorldMenu;
			returnGUI.viewTranslateEvent = complexMenuVTE;
			returnGUI.presenterHandleEvent = editGamePHE;
			returnGUI.stop = menuStop;
			break;
		case(SAVE_WORLD):
			returnGUI.start = startWorldMenu;
			returnGUI.viewTranslateEvent = complexMenuVTE;
			returnGUI.presenterHandleEvent = saveWorldPHE;
			returnGUI.stop = menuStop;
			break;
			/*
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

/* helper functions for MVP */
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
	viewState->currButton = 0;
	viewState->gridPanel = NULL;
	viewState->gridItemsImages = NULL;
	return viewState;
}

void startWorldBuilder(GUIref gui, void* initData){
	initializeWorldBuilderModel(gui, initData);
	if(isError)
		return;
	WBDataRef data = gui->model;
	char imgPath[] = "images/worldBuilder_temp.bmp";

	// initialize viewState
	ViewStateref wbViewState = initializeGUIViewState(); //maybe we will need a different function!
	if (wbViewState == NULL){
		return;
	}
	gui->viewState = wbViewState;

	// create image surface
	SDL_Surface * wbImage = SDL_LoadBMP(imgPath);
	if (wbImage == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	wbViewState->image = wbImage;

	// create buttons array
	Widget ** buttons = (Widget **)malloc(WB_NUM_BUTTONS*sizeof(Widget *));
	if (buttons == NULL){
		perrorPrint("malloc");
		return;
	}
	wbViewState->menuButtons = buttons;

	// create the UItree
	Widget *win = create_window(WIN_W,WIN_H, 0, 0, 0);
	if (win == NULL){
		return;
	}
	ListRef winNode = newList(win);
	wbViewState->UITree = winNode;
	if (winNode == NULL){
		freeWidget(win);
		return;
	}

	Widget *topPanel = create_panel(0, 0, WIN_W, TOP_PANEL_H,PANEL_RED,PANEL_GREEN,PANEL_BLUE);
	if (topPanel == NULL){
		return;
	}
	ListRef topPanelNode = addChildNode(winNode, topPanel);
	if (topPanelNode == NULL){
		freeWidget(topPanel);
		return;
	}
	Widget *sidePanel = create_panel(0, 210, SIDE_PANEL_W, SIDE_PANEL_H,PANEL_RED,PANEL_GREEN,PANEL_BLUE);
		if (sidePanel == NULL){
			return;
		}
	ListRef sidePanelNode = addChildNode(winNode, sidePanel);
	if (sidePanelNode == NULL){
		freeWidget(sidePanel);
		return;
	}
	Widget *gridPanel = create_panel(210, 210, GRID_SIZE, GRID_SIZE, PANEL_RED, PANEL_GREEN, PANEL_BLUE);
	if (gridPanel == NULL){
		return;
	}
	ListRef gridPanelNode = addChildNode(winNode, gridPanel);
	if (gridPanelNode == NULL){
		freeWidget(gridPanel);
		return;
	}
	Widget *label = create_image(calcWBtitleX(WB_TITLE_W), WB_WIDGET_Y_GAP, WB_TITLE_W, WB_TITLE_H,
			wbImage, WB_BUTTON_W, data->editedWorld*WB_TITLE_H);
	if (label == NULL){
		return;
	}
	ListRef labelNode = addChildNode(topPanelNode, label);
	if (labelNode == NULL){
		freeWidget(label);
		return;
	}

	//add buttons to top panel:
	int topButtonX = calcTopButtonX(), topButtonY = calcTopButtonY(), buttonImgX = 0, buttonImgY = 0;
	// Add buttons to buttons array and to UI tree
	for (int i = 0; i < WB_TOP_PANEL_NUM_BUTTONS; i++){
		buttons[i] = create_button(topButtonX, topButtonY, WB_BUTTON_W, WB_BUTTON_H,
				wbImage, buttonImgX, buttonImgY, buttonImgX, buttonImgY, 0);//write function for creating a non markable button
		if (buttons[i] == NULL){
			return;
		}
		ListRef newButtonNode = addChildNode(topPanelNode, buttons[i]);
		if (newButtonNode == NULL){
			freeWidget(buttons[i]);
			return;
		}
		buttonImgY += WB_BUTTON_H;
		topButtonX += WB_BUTTON_W + WB_BUTTON_X_GAP;
	}

	//add buttons to side panel:
	int sideButtonX = calcSideButtonX(), sideButtonY = calcSideButtonY();
	// Add buttons to buttons array and to UI tree
	for (int i = WB_TOP_PANEL_NUM_BUTTONS; i < WB_NUM_BUTTONS; i++){
		buttons[i] = create_button(sideButtonX, sideButtonY, WB_BUTTON_W, WB_BUTTON_H,
				wbImage, buttonImgX, buttonImgY, buttonImgX, buttonImgY, 0);//write function for creating a non markable button
		if (buttons[i] == NULL){
			return;
		}
		ListRef newButtonNode = addChildNode(sidePanelNode, buttons[i]);
		if (newButtonNode == NULL){
			freeWidget(buttons[i]);
			return;
		}
		buttonImgY += WB_BUTTON_H;
		sideButtonY += WB_BUTTON_H + WB_WIDGET_Y_GAP;
	}

	//initialize gridItemsImages Array:
	Widget ** gridItemImages = malloc(NUM_GRID_ITEMS*sizeof(Widget *));
	if (buttons == NULL){
		perrorPrint("malloc");
		return;
	}
	wbViewState->gridItemsImages = gridItemImages;
	int gridItemImgX = WB_BUTTON_W + WB_TITLE_W;
	int gridItemImgY = 0;
	for (int i = 0; i < NUM_GRID_ITEMS; i++){
		gridItemImages[i] = create_image(0, 0, GRID_SQUARE_SIZE, GRID_SQUARE_SIZE,
				wbImage, gridItemImgX, gridItemImgY);
		if (gridItemImages[i] == NULL){
			return;
		}
		gridItemImgY += GRID_SQUARE_SIZE;
	}
	setImageTransparent(gridItemImages[SELECT], TR_RED, TR_GREEN, TR_BLUE);//CREATE THIS FUNCTION!!!!!!!
	if (isError)
		return;
	setImageTransparent(gridItemImages[DESELECT], TR_RED, TR_GREEN, TR_BLUE);
	if (isError)
		return;
	createGridByData(gridPanel, data->gameGridData, gridItemImages);
	if (isError)
		return;
	// draw GUI according to UItree
	drawGui(gui);
}


void* stopWorldBuilder(GUI * gui){

	ViewStateref guiViewState = gui->viewState;
	WBDataRef wbData = gui->model;
	gui->model = NULL;
	gui->viewState = NULL;

	MenuDataRef returnData = initMenuDataToDefault();
	if (isError)
		return NULL;
	//put things in return Data
	//for main menu:
	returnData->mainMenuButton = wbData->mainMenuButton;
	//for save world:
	returnData->currWorld = wbData->gameGridData;
	returnData->editedWorld = wbData->editedWorld;
	returnData->isCatFirst = wbData->isCatFirst;
	returnData-> numTurns = wbData->numTurns;
	returnData->wbCurrPos = wbData->currPos;

	if (guiViewState != NULL){
		freeViewState(guiViewState);
	}
	if (wbData != NULL){
		free(wbData); // maybe we need a function for that????
	}
	if (isError || isQuit){
		free(returnData); // we need to write a function for that!
		return NULL;
	}

	return returnData;
}


void freeViewState(ViewStateref guiViewState){
	if (guiViewState->menuButtons != NULL)
		free(guiViewState->menuButtons);
	if (guiViewState->image != NULL)
		SDL_FreeSurface(guiViewState->image);
	if (guiViewState->UITree != NULL)
		freeTree(guiViewState->UITree, freeWidget);
	if (guiViewState->gridItemsImages != NULL)
		freeGridItems(guiViewState->gridItemsImages);
	free(guiViewState);
}


void freeGridItems(Widget ** gridItemsImages){
	for (int i = 0; NUM_GRID_ITEMS; i++)
		freeWidget(gridItemsImages[i]);
	free(gridItemsImages);
}



void createGridByData(Widget *gridPanel, char **gridData, Widget **gridItemImages){
	Widget *currItemImage = NULL;

	for (int i = 0; i < ROW_NUM; i++){
		for (int j = 0; j < COL_NUM; j++){
			switch(gridData[i][j]){
				case('C'):
					currItemImage = gridItemImages[CAT];
					break;
				case('M'):
					currItemImage = gridItemImages[MOUSE];
					break;
				case('P'):
					currItemImage = gridItemImages[CHEESE];
					break;
				case('W'):
					currItemImage = gridItemImages[WALL];
					break;
				case('#'):
					currItemImage = gridItemImages[EMPTY];
					break;
				default:
					break;
			}
			currItemImage->location_rect.x = j*(GRID_SQUARE_SIZE + GRID_GAP_SIZE);
			currItemImage->location_rect.y = i*(GRID_SQUARE_SIZE + GRID_GAP_SIZE);
			if (SDL_BlitSurface(currItemImage->surface, &currItemImage->img_rect,
				gridPanel->surface, &currItemImage->location_rect) != 0){
					sdlErrorPrint("failed to blit image");
					return;
			}
		}
	}
	gridItemPosition zeroPos = {0,0};
	selectGridPos(gridPanel, gridItemImages, zeroPos);
}

void selectGridPos(Widget *gridPanel, Widget ** gridSelectImages, gridItemPosition currPos){
	blitItemToGrid(gridPanel, gridSelectImages[SELECT], currPos.row, currPos.col);
}

//maybe??????
void deselectGridPos(Widget *gridPanel, Widget ** gridDeselectImages, gridItemPosition currPos){
	blitItemToGrid(gridPanel, gridDeselectImages[DESELECT], currPos.row, currPos.col);
}

void blitItemToGrid(Widget *gridPanel, Widget * itemImage, int row, int col){
	itemImage->location_rect.x = col*(GRID_SQUARE_SIZE + GRID_GAP_SIZE);
	itemImage->location_rect.y = row*(GRID_SQUARE_SIZE + GRID_GAP_SIZE);
	if (SDL_BlitSurface(itemImage->surface, &itemImage->img_rect,
		gridPanel->surface, &itemImage->location_rect) != 0){
			sdlErrorPrint("failed to blit image");
			return;
	}
}



/* general MVP functions - used by several GUIs */
void* simpleMenuVTE(void* viewState, SDL_Event* event, int numOfButtons){
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	ViewStateref menuViewState = viewState;
	switch (event->type) {
		case (SDL_KEYUP):
			if (event->key.keysym.sym == SDLK_TAB){
				returnEvent->type = MARK_NEXT_BUTTON;
			}
			if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_KP_ENTER){
				returnEvent->type = SELECT_CURR_BUTTON;
			}
			break;
		case (SDL_MOUSEBUTTONUP):
			for (int i = 0; i< numOfButtons; i++){
				Widget * currButton = menuViewState->menuButtons[i];
				if (isClickEventOnButton(event, currButton, REGULAR_BUTTON)){
					returnEvent->type = SELECT_BUTTON_NUM;
					returnEvent->buttonNum = i;
					break;
				}
			}
			break;
		default:
			returnEvent->type = NO_EVENT;
	}
	return returnEvent;
}

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
	return -1; //check !!!!
}

void* complexMenuVTE(void* viewState, SDL_Event* event){
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	int numOfButtons = COMMON_MENU_NUM_BUTTONS;
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	ViewStateref menuViewState = viewState;
	returnEvent->type = NO_EVENT;
	switch (event->type) {
		case (SDL_KEYUP):
			if (event->key.keysym.sym == SDLK_TAB){
				returnEvent->type = MARK_NEXT_BUTTON;
			}
			else if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_KP_ENTER){
				if (menuViewState->currButton != FIRST_BUTTON)
					returnEvent->type = SELECT_CURR_BUTTON;
			}
			else if (event->key.keysym.sym == SDLK_UP){
				if (menuViewState->currButton == FIRST_BUTTON)
					returnEvent->type = INCREASE_VALUE;
			}
			else if (event->key.keysym.sym == SDLK_DOWN){
				if (menuViewState->currButton == FIRST_BUTTON)
					returnEvent->type = DECREASE_VALUE;
			}
			break;
		case (SDL_MOUSEBUTTONUP):
			for (int i = 0; i< numOfButtons; i++){
				Widget * currButton = menuViewState->menuButtons[i];
				if (i == FIRST_BUTTON){
					if(isClickEventOnButton(event, currButton, UP_ARROW_BUTTON)){
						returnEvent->type = INCREASE_VALUE;
						break;
					}
					if(isClickEventOnButton(event, currButton, DOWN_ARROW_BUTTON)){
						returnEvent->type = DECREASE_VALUE;
						break;
					}
					if (isClickEventOnButton(event, currButton, REGULAR_BUTTON)){
						returnEvent->type = MARK_VALUES_BUTTON;
						returnEvent->buttonNum = i;
						break;
					}
				}
				if (isClickEventOnButton(event, currButton, REGULAR_BUTTON)){
					returnEvent->type = SELECT_BUTTON_NUM;
					returnEvent->buttonNum = i;
					break;
				}
			}
			break;
	}
	return returnEvent;
}

void* worldBuilderVTE(void* viewState, SDL_Event* event){
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	returnEvent->type = NO_EVENT;
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	ViewStateref wbViewState = viewState;
	SDLKey key;
	switch (event->type) {
		case (SDL_KEYUP):
			key = event->key.keysym.sym;
			if (key == SDLK_s || key == SDLK_F1 || key == SDLK_ESCAPE ||
					key == SDLK_m || key == SDLK_c || key == SDLK_p || key == SDLK_w || key == SDLK_SPACE){
				returnEvent->type = SELECT_BUTTON_NUM;
				returnEvent->buttonNum = getWBButtonNum(key);
			}
			else if (key == SDLK_UP)
				returnEvent->type = GO_UP;
			else if (key ==  SDLK_DOWN)
				returnEvent->type = GO_DOWN;
			else if (key ==  SDLK_RIGHT)
				returnEvent->type = GO_RIGHT;
			else if (key ==  SDLK_LEFT)
				returnEvent->type = GO_LEFT;
			break;
		case (SDL_MOUSEBUTTONUP):
			if (event->button.x < WIN_W - GRID_SIZE || event->button.y < WIN_H - GRID_SIZE){
				for (int i = 0; i< WB_NUM_BUTTONS; i++){
					Widget * currButton = wbViewState->menuButtons[i];
					if (isClickEventOnButton(event, currButton, REGULAR_BUTTON)){
						returnEvent->type = SELECT_BUTTON_NUM;
						returnEvent->buttonNum = i;
						break;
					}
				}
			}
			else{ // click is inside the grid
				returnEvent->type = SELECT_SQUARE;
				//MayBe write a function for that !!!!!
				returnEvent->gridPos.col = (event->button.x-(WIN_W - GRID_SIZE))/(GRID_SQUARE_SIZE+GRID_GAP_SIZE);
				returnEvent->gridPos.row = (event->button.y-(WIN_W - GRID_SIZE))/(GRID_SQUARE_SIZE+GRID_GAP_SIZE);
			}
			break;
		default:
			returnEvent->type = NO_EVENT;
	}
	return returnEvent;
}

StateId worldBuilderPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = WORLD_BUILDER;
	if (logicalEvent == NULL || viewState == NULL || model == NULL)
		return returnStateId;
	logicalEventRef wbEvent = logicalEvent;
	ViewStateref wbView = viewState;
	WBDataRef wbModel = model;
	StateId states[WB_NUM_BUTTONS] = {SAVE_WORLD, MAIN_MENU, QUIT, WORLD_BUILDER, WORLD_BUILDER, WORLD_BUILDER,
			WORLD_BUILDER,WORLD_BUILDER};
	switch(wbEvent->type){
		case(SELECT_BUTTON_NUM):
			returnStateId = states[wbEvent->buttonNum];
			if(returnStateId == SAVE_WORLD)
				returnStateId = isGridValid(wbModel);
			else if (returnStateId == WORLD_BUILDER){
				putGridItemInPos(wbModel, wbView->gridPanel, wbView->gridItemsImages, wbModel->currPos, wbEvent->buttonNum);
			}
			break;
		case(SELECT_SQUARE):
			changeSelectedGridSquare(wbView->gridPanel, wbView->gridItemsImages,&wbModel->currPos, wbEvent->gridPos);
			break;
		case(GO_UP):
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImages, &wbModel->currPos, GO_UP);  //Write this function!!!
			break;
		case(GO_DOWN):
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImages, &wbModel->currPos, GO_DOWN);
			break;
		case(GO_RIGHT):
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImages, &wbModel->currPos, GO_RIGHT);
			break;
		case(GO_LEFT):
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImages, &wbModel->currPos, GO_LEFT);
			break;
		case(NO_EVENT):
			break;
		default:
			break;
	}
	free(logicalEvent);
	return returnStateId;

}

int isGridValid(WBDataRef wbModel){
	gridItemPosition invalidPos = {-1,-1};
	if (isSamePos(wbModel->catPos, invalidPos) || isSamePos(wbModel->mousePos, invalidPos)
			|| isSamePos(wbModel->cheesePos, invalidPos) ){
		return 0;
	}
	return 1;
}

void putGridItemInPos(WBDataRef wbModel, Widget * gridPanel, Widget ** gridItemsImages,
		gridItemPosition currPos, int buttonNum){
	gridItem buttonsItems[5] = {MOUSE, CAT, CHEESE, WALL, EMPTY};
	gridItem itemType = buttonsItems[buttonNum-WB_TOP_PANEL_NUM_BUTTONS];
	if (itemType == WALL || itemType == EMPTY){
		addReusableItemToPos(itemType, wbModel->gameGridData, gridPanel, gridItemsImages, currPos);
	}
	else{ // item is MOUSE, CAT or CHEESE
		gridItemPosition * itemPosRef = NULL;
		if(itemType == MOUSE){
			itemPosRef = &wbModel->mousePos;
			if (wbModel-> editedWorld == 0){ //edited from create world!
				wbModel->isCatFirst = 0;
			}
		}
		else if (itemType == CAT){
			itemPosRef = &wbModel->catPos;
			if (wbModel-> editedWorld == 0){ //edited from create world!
				wbModel->isCatFirst = 1;
			}
		}
		else
			itemPosRef = &wbModel->cheesePos;
		moveItemToPos(itemType, gridItemsImages, gridPanel, wbModel->gameGridData, currPos, itemPosRef);
	}
	fixOverride(itemType, wbModel, currPos);
	selectGridPos(gridPanel, gridItemsImages, currPos);
}

void addReusableItemToPos(gridItem itemType, char ** gridData, Widget * gridPanel,
		Widget ** gridItemsImages, gridItemPosition currPos){
	Widget * itemImage = gridItemsImages[itemType];
	blitItemToGrid(gridPanel, itemImage, currPos.row, currPos.col);
	if (itemType == WALL)
		gridData[currPos.row][currPos.col] = 'W';
	else
		gridData[currPos.row][currPos.col] = '#';
}



void moveItemToPos(gridItem itemType, Widget ** gridItemsImages, Widget * gridPanel, char ** gridData,
		gridItemPosition currPos, gridItemPosition * prevItemPos){
	gridItemPosition noPos = {-1,-1};
	if (isSamePos(*prevItemPos, noPos) == 0){
		blitItemToGrid(gridPanel, gridItemsImages[EMPTY], prevItemPos->row, prevItemPos->col);
		if (isError)
			return;
		gridData[prevItemPos->row][prevItemPos->col] = getItemChar(EMPTY);
		prevItemPos->row = currPos.row;
		prevItemPos->col = currPos.col;
	}
	gridData[currPos.row][currPos.col] = getItemChar(itemType);
	blitItemToGrid(gridPanel, gridItemsImages[itemType], currPos.row, currPos.col);
}

char getItemChar(gridItem item){
	char itemsChars[5] = {'M', 'C', 'P', 'W', '#'};
	return itemsChars[item];
}


void fixOverride(gridItem itemType, WBDataRef wbModel, gridItemPosition currPos){
	gridItemPosition noPos = {-1,-1};
	if (itemType != CAT && isSamePos(currPos, wbModel->catPos))
		wbModel->catPos = noPos;
	if (itemType != MOUSE && isSamePos(currPos, wbModel->mousePos))
		wbModel->mousePos = noPos;
	if (itemType != CHEESE && isSamePos(currPos, wbModel->cheesePos))
		wbModel->cheesePos = noPos;
}

int isSamePos(gridItemPosition pos1, gridItemPosition pos2){
	if (pos1.row == pos2.row && pos1.col == pos2.col)
		return 1;
	return 0;
}

void changeSelectedPosByArrow(Widget * gridPanel, Widget ** gridItemsImages,
		gridItemPosition * currPos, logicalEventType direction){
	gridItemPosition newPos = {currPos->row, currPos->col};
	switch(direction){
		case(GO_UP):
			if (currPos->row > 0)
				newPos.row -= 1;
			else
				return;
			break;
		case(GO_DOWN):
			if (currPos->row < ROW_NUM)
				newPos.row += 1;
			else
				return;
			break;
		case(GO_LEFT):
			if (currPos->col > 0)
				newPos.col -= 1;
			else
				return;
			break;
		case(GO_RIGHT):
			if (currPos->col < COL_NUM)
				newPos.col += 1;
			else
				return;
			break;
		default:
			break;
	}
	changeSelectedGridSquare(gridPanel, gridItemsImages, currPos, newPos);
}

void changeSelectedGridSquare(Widget * gridPanel, Widget ** gridItemsImages, gridItemPosition * currPos,
		gridItemPosition newPos){
	deselectGridPos(gridPanel, gridItemsImages, *currPos);
	if (isError)
		return;
	selectGridPos(gridPanel, gridItemsImages, newPos);
	if (isError)
		return;
	currPos->row = newPos.row;
	currPos->col = newPos.col;
	blitUpToWindow(gridPanel);
}

StateId generalMenuPHE(void* model, void* viewState, void* logicalEvent, StateId states[], int numOfButtons,
		StateId stateId, int* currButton, int* currValue, int maxValue){
	StateId returnStateId = stateId;
	if (logicalEvent == NULL || viewState == NULL)
		return returnStateId;
	logicalEventRef menuEvent = logicalEvent;
	ViewStateref menuView = viewState;
	switch(menuEvent->type){
		case(SELECT_CURR_BUTTON):
			returnStateId = states[*currButton];
			menuView->currButton = *currButton;
			break;
		case(MARK_NEXT_BUTTON):
			changeSelectedButton(menuView->menuButtons[*currButton],
					menuView->menuButtons[(*currButton+1)%numOfButtons]);
			*currButton = (*currButton + 1)%numOfButtons;
			menuView->currButton = *currButton;
			break;
		case(SELECT_BUTTON_NUM):
			*currButton = menuEvent->buttonNum;
			returnStateId = states[menuEvent->buttonNum];
			menuView->currButton = *currButton;
			break;
		case(MARK_VALUES_BUTTON):
			changeSelectedButton(menuView->menuButtons[*currButton],
					menuView->menuButtons[FIRST_BUTTON]);
			*currButton = FIRST_BUTTON;
			menuView->currButton = *currButton;
			break;
		case(INCREASE_VALUE):
			increaseValuesButton(currValue, maxValue, menuView->menuButtons[FIRST_BUTTON]);
			changeSelectedButton(menuView->menuButtons[*currButton],
				menuView->menuButtons[FIRST_BUTTON]);
			*currButton = FIRST_BUTTON;
			menuView->currButton = *currButton;
			break;
		case(DECREASE_VALUE):
			decreaseValuesButton(currValue, MIN_VALUE, menuView->menuButtons[FIRST_BUTTON]);
			changeSelectedButton(menuView->menuButtons[*currButton],
				menuView->menuButtons[FIRST_BUTTON]);
			*currButton = FIRST_BUTTON;
			menuView->currButton = *currButton;
			break;
		case(NO_EVENT):
			break;
		default:
			break;
	}
	if (*currButton == numOfButtons-1 && returnStateId != stateId)
		*currButton = FIRST_BUTTON;
	free(logicalEvent);
	return returnStateId;

}


void* menuStop(GUIref gui){ /* maybe this will be a general stop function */
	MenuDataRef returnData = gui->model;
	ViewStateref guiViewState = gui->viewState;
	gui->model = NULL;
	gui->viewState = NULL;

	if (guiViewState != NULL){
		// we need to write a function for that
		if (guiViewState->menuButtons != NULL)
			free(guiViewState->menuButtons);
		if (guiViewState->image != NULL)
			SDL_FreeSurface(guiViewState->image);
		if (guiViewState->UITree != NULL)
			freeTree(guiViewState->UITree, freeWidget);
	}
	if (isError || isQuit){
		free(returnData); // we need to write a function for that!
		return NULL;
	}

	return returnData;
}

void drawGui(GUIref gui){
	ViewStateref viewState = gui->viewState;
	treeDFS(viewState->UITree, calcAbsWidgetXY, addChildWidgetsToParent);
	Widget * window = viewState->UITree->data;
	if (SDL_Flip(window->surface) != 0) {
		sdlErrorPrint("failed to flip buffer");
		return;
	}
}

/* maybe we don't need to pass initdata !!!!!!!!!!!!! */
void startGeneralMenu(GUIref gui, void * initData, char * imgPath, int titleImgX, int titleImgY, int titleWidth,
		int numButtons, int selectedButton, int firstButtonNumOpts, int value){
	/* initialize viewState */
	ViewStateref menuViewState = initializeGUIViewState();
	if (menuViewState == NULL){
		return;
	}
	gui->viewState = menuViewState;

	/* create image surface */
	SDL_Surface * menuImage = SDL_LoadBMP(imgPath);
	if (menuImage == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	menuViewState->image = menuImage;

	/* create buttons array */
	Widget ** buttons = (Widget **)malloc(numButtons*sizeof(Widget *));
	if (buttons == NULL){
		perrorPrint("malloc");
		return;
	}
	menuViewState->menuButtons = buttons;

	/* create the UItree */
	Widget *win = create_window(WIN_W,WIN_H, 0, 0, 0);
	if (win == NULL){
		return;
	}
	ListRef win_node = newList(win);
	menuViewState->UITree = win_node;
	if (win_node == NULL){
		freeWidget(win);
		return;
	}
	Widget *panel = create_panel(calcPanelX(titleWidth), calcPanelY(numButtons),
			calcPanelWidth(titleWidth),calcPanelHeight(numButtons),PANEL_RED,PANEL_GREEN,PANEL_BLUE);
	if (panel == NULL){
		return;
	}
	ListRef panel_node = addChildNode(win_node, panel);
	if (panel_node == NULL){
		freeWidget(panel);
		return;
	}
	Widget *label = create_image(MENU_TITLE_X_GAP, MENU_TITLE_Y_GAP, titleWidth, MENU_TITLE_H,
			menuImage, titleImgX, titleImgY);
	if (label == NULL){
		return;
	}
	ListRef label_node = addChildNode(panel_node, label);
	if (label_node == NULL){
		freeWidget(label);
		return;
	}
	/* Add buttons to buttons array and to UI tree */
	int button_x = calcMenuButtonX(titleWidth), button_y = calcMenuButtonY(), isSelected_x = MENU_BUTTON_W, isSelected_y = 0, isNselected_x = 0, isNselected_y=0;
	for (int i = 0; i < numButtons; i++){
		buttons[i] = create_button(button_x,button_y, MENU_BUTTON_W, MENU_BUTTON_H,
				menuImage, isSelected_x, isSelected_y, isNselected_x, isNselected_y, 0);
		if (buttons[i] == NULL){
			return;
		}
		ListRef newButtonNode = addChildNode(panel_node, buttons[i]);
		if (newButtonNode == NULL){
			freeWidget(buttons[i]);
			return;
		}
		if (i == FIRST_BUTTON){
			isSelected_y += firstButtonNumOpts*MENU_BUTTON_H;
			isNselected_y += firstButtonNumOpts*MENU_BUTTON_H;
		}
		else{
			isSelected_y += MENU_BUTTON_H;
			isNselected_y += MENU_BUTTON_H;
		}
		button_y += MENU_BUTTON_H+MENU_BUTTON_GAP;
	}

	/* update the view buttons */
	if(firstButtonNumOpts > 1){ /* update the values button */
		setValuesButtonFromInit(value, buttons[0]);
	}
	menuViewState->currButton = selectedButton;
	setButtonSelected(menuViewState->menuButtons[selectedButton]);
	/* draw GUI according to UItree */
	drawGui(gui);
}

void initializeMenuModel(GUIref gui, void* initData){
	if (initData == NULL){
		gui->model = initMenuDataToDefault(); /* write this function */
	}
	else{
		gui->model = initData;
	}
}

void initializeWorldBuilderModel(GUIref gui, void* initData){
	MenuDataRef menuData = initData;

	WBDataRef wbData = malloc(sizeof(WBData));
	if (wbData == NULL){
		perrorPrint("malloc");
	}
	gui->model = wbData;
	wbData->mainMenuButton = menuData->mainMenuButton;
	wbData->gameGridData = NULL;
	wbData->editedWorld = menuData->editedWorld;

	gridItemPosition catPos = {-1, -1};
	gridItemPosition mousePos = {-1, -1};
	gridItemPosition cheesePos = {-1, -1};
	gridItemPosition currPos = {0, 0};


	if (menuData->preWorldBuilder == SAVE_WORLD && menuData->loadFromFile == 0){ //we pressed back in SAVE_WORLD
		wbData->gameGridData = menuData->currWorld;
		currPos = menuData->wbCurrPos;
		wbData->numTurns = menuData->numTurns;
		wbData->isCatFirst = menuData->isCatFirst; //check this!!!
	}
	else{ // preWorldBuilder == MAIN_MENU || (preWorldBuilder == EDIT_GAME || preWorldBuilder = SAVE_WORLD) && loadFromFile=1
		wbData->gameGridData = initGameData(wbData->editedWorld, &wbData->numTurns, &wbData->isCatFirst);
	}

	updateItemsPostions(&mousePos,&catPos,&cheesePos, wbData->gameGridData);
	wbData->catPos = catPos;
	wbData->mousePos = mousePos;
	wbData->cheesePos = cheesePos;
	wbData->currPos = currPos;

	free(initData);
}

void updateItemsPostions(gridItemPosition * mousePos,gridItemPosition * catPos,gridItemPosition * cheesePos,
		char ** gameGridData){
	for (int i = 0 ; i < ROW_NUM ;i++){
		for (int j = 0; j < COL_NUM; j++){
			char currItemChar = gameGridData[i][j];
			if (currItemChar == 'M'){
				mousePos->row = i;
				mousePos->col = j;
			}
			else if (currItemChar == 'C'){
				catPos->row = i;
				catPos->col = j;
			}
			else if (currItemChar == 'P'){
				cheesePos->row = i;
				cheesePos->col = j;
			}
		}
	}

}

/* Main Menu specific MVP functions */
void startMainMenu(GUIref gui, void* initData){
	initializeMenuModel(gui, initData);
	if(isError)
		return;
	char imgPath[] = "images/MainMenu_temp.bmp";
	MenuDataRef data = gui->model;
	int currentButton = data->mainMenuButton;
	/* start the main menu gui */
	startGeneralMenu(gui, initData, imgPath,
			MENU_BUTTON_W*2, 0, 200, MAIN_MENU_NUM_BUTTONS, currentButton, 1, 0);
}


void startChooseAnimal(GUIref gui, void* initData){
	initializeMenuModel(gui, initData);
	if(isError)
		return;
	char imgPath[] = "images/chooseAnimal_temp.bmp";
	MenuDataRef data = gui->model;
	int currentButton,titleImgY;
	switch(gui->stateId){
		case(CHOOSE_CAT):
			currentButton = data->chooseCatButton;
			titleImgY = 0;
			break;
		case(CHOOSE_MOUSE):
			currentButton = data->chooseMouseButton;
			titleImgY = MENU_TITLE_H;
			break;
		default:
			break;
	}
	startGeneralMenu(gui, initData, imgPath,
			MENU_BUTTON_W*2, titleImgY, 200, COMMON_MENU_NUM_BUTTONS, currentButton, 1, 0);
}

void startAnimalSkill(GUIref gui, void* initData){
	initializeMenuModel(gui, initData);
	if(isError)
		return;
	char imgPath[] = "images/animalSkill_temp.bmp";
	MenuDataRef data = gui->model;
	int currentButton, currentValue, titleImgY;
	switch(gui->stateId){
		case(CAT_SKILL):
			currentButton = data->catSkillButton;
			currentValue = data->catSkill;
			titleImgY = 0;
			break;
		case(MOUSE_SKILL):
			currentButton = data->mouseSkillButton;
			currentValue = data->mouseSkill;
			titleImgY = MENU_TITLE_H;
			break;
		default:
			break;
	}
	startGeneralMenu(gui, initData, imgPath,
			MENU_BUTTON_W*2, titleImgY, 200, COMMON_MENU_NUM_BUTTONS,currentButton, MAX_SKILL_VALUE, currentValue);
}

void startWorldMenu(GUIref gui, void* initData){
	initializeMenuModel(gui, initData);
	if(isError)
		return;
	char imgPath[] = "images/worldMenu_temp.bmp";
	MenuDataRef data = gui->model;

	int currentButton, currentValue, titleImgY;
	switch(gui->stateId){
		case(EDIT_GAME):
			currentButton = data->editGameButton;
			currentValue = data->editedWorld;
			titleImgY = 0;
			break;
		case(LOAD_GAME):
			currentButton = data->loadGameButton;
			currentValue = data->loadGameWorld;
			titleImgY = MENU_TITLE_H;
			break;
		case(SAVE_WORLD):
			currentButton = data->saveWorldButton;
			currentValue = data->editedWorld ? data->editedWorld: MIN_VALUE; //if edited world is 0 this will show 1
			titleImgY = 2*MENU_TITLE_H;
			break;
		default:
			break;
	}
	startGeneralMenu(gui, initData, imgPath,
			MENU_BUTTON_W*2, titleImgY, 200, COMMON_MENU_NUM_BUTTONS,currentButton, MAX_WORLD, currentValue);
}


void* mainMenuVTE(void* viewState, SDL_Event* event){
	logicalEventRef returnEvent = simpleMenuVTE(viewState, event, MAIN_MENU_NUM_BUTTONS);
	return returnEvent;
}

StateId mainMenuPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = MAIN_MENU;
	if (model == NULL)
		return returnStateId;
	MenuDataRef mainMenuModel = model;
	StateId mainMenuStates[MAIN_MENU_NUM_BUTTONS] = {CHOOSE_CAT, LOAD_GAME, WORLD_BUILDER, EDIT_GAME, QUIT};
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, mainMenuStates, MAIN_MENU_NUM_BUTTONS, returnStateId,
			&mainMenuModel->mainMenuButton, NULL, 0);
	// maybe create a function the will reset all the data when going to main menu
	// and maybe it should be in start main menu!
	if (returnStateId == CHOOSE_CAT)
		mainMenuModel->preChooseCat = MAIN_MENU;
	else if (returnStateId == WORLD_BUILDER){
		mainMenuModel->preWorldBuilder = MAIN_MENU;
		mainMenuModel->editedWorld = 0; // we can delete this line!!!
	}
	else if (returnStateId == LOAD_GAME)
		mainMenuModel->loadGameWorld = DEFAULT_WORLD;
	else if (returnStateId == EDIT_GAME)
		mainMenuModel->editedWorld = DEFAULT_WORLD;
	return returnStateId;
}

StateId catSkillPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = CAT_SKILL;
	if (model == NULL)
		return returnStateId;
	MenuDataRef catSkillModel = model;
	StateId catSkillStates[COMMON_MENU_NUM_BUTTONS] = {CAT_SKILL, CHOOSE_MOUSE, CHOOSE_CAT};
	if (catSkillModel->preChooseCat == PLAY_GAME){
		catSkillStates[1]=PLAY_GAME;
	}
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, catSkillStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &catSkillModel->catSkillButton, &catSkillModel->currValueTemp, MAX_SKILL_VALUE);
	if (returnStateId == PLAY_GAME || returnStateId == CHOOSE_MOUSE){ // Done button was pressed
		catSkillModel->isCatHuman = 0;
		catSkillModel->catSkill = catSkillModel->currValueTemp;
		if (returnStateId == CHOOSE_MOUSE){
			catSkillModel->preChooseMouse = CAT_SKILL;
		}
	}
	if (returnStateId == CHOOSE_CAT && catSkillModel->preChooseCat == MAIN_MENU)
		catSkillModel->catSkill = DEFAULT_SKILL;
	return returnStateId;
}

StateId mouseSkillPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = MOUSE_SKILL;
	if (model == NULL)
		return returnStateId;
	MenuDataRef mouseSkillModel = model;
	StateId mouseSkillStates[COMMON_MENU_NUM_BUTTONS] = {MOUSE_SKILL, PLAY_GAME, CHOOSE_MOUSE};
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, mouseSkillStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &mouseSkillModel->mouseSkillButton, &mouseSkillModel->currValueTemp, MAX_SKILL_VALUE);
	if (returnStateId == PLAY_GAME){ // Done button was pressed
		mouseSkillModel->isMouseHuman = 0;
		mouseSkillModel->mouseSkill = mouseSkillModel->currValueTemp;
	}
	if (returnStateId == CHOOSE_MOUSE && mouseSkillModel->preChooseMouse == CAT_SKILL)
		mouseSkillModel->mouseSkill = DEFAULT_SKILL;
	return returnStateId;
}

StateId loadGamePHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = LOAD_GAME;
	if (model == NULL)
		return returnStateId;
	MenuDataRef loadGameModel = model;
	StateId loadGameStates[COMMON_MENU_NUM_BUTTONS] = {LOAD_GAME, CHOOSE_CAT, MAIN_MENU};
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, loadGameStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &loadGameModel->loadGameButton, &loadGameModel->loadGameWorld, MAX_WORLD);
	if (returnStateId == CHOOSE_CAT)
		loadGameModel->preChooseCat = LOAD_GAME;
	return returnStateId;
}

StateId editGamePHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = EDIT_GAME;
	if (model == NULL)
		return returnStateId;
	MenuDataRef editGameModel = model;
	StateId editGameStates[COMMON_MENU_NUM_BUTTONS] = {EDIT_GAME, WORLD_BUILDER, MAIN_MENU};
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, editGameStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &editGameModel->editGameButton, &editGameModel->editedWorld, MAX_WORLD);
	if (returnStateId == WORLD_BUILDER){
		editGameModel->preWorldBuilder = EDIT_GAME;
		editGameModel->loadFromFile = 1;
	}
	return returnStateId;
}

StateId saveWorldPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = SAVE_WORLD;
	if (model == NULL)
		return returnStateId;
	MenuDataRef saveWorldModel = model;
	StateId saveWorldStates[COMMON_MENU_NUM_BUTTONS] = {SAVE_WORLD, WORLD_BUILDER, WORLD_BUILDER};
	saveWorldModel->currValueTemp = saveWorldModel->editedWorld;
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, saveWorldStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &saveWorldModel->saveWorldButton, &saveWorldModel->currValueTemp, MAX_WORLD);
	if (returnStateId == WORLD_BUILDER){
		saveWorldModel->preWorldBuilder = SAVE_WORLD; // do we need this?
	}
	if (saveWorldModel->saveWorldButton == 1){ //done was pressed
		saveWorldModel->loadFromFile = 1;// tell wb to use file to load the grid
		saveWorldModel->editedWorld = saveWorldModel->currValueTemp;
		saveGridDataToFile(saveWorldModel->editedWorld, saveWorldModel->isCatFirst, saveWorldModel->currWorld);

		//free grid data
		freeGridData(saveWorldModel->currWorld);
		saveWorldModel->currWorld = NULL;
	}
	else if (saveWorldModel->saveWorldButton == 2) //back was pressed
		saveWorldModel->loadFromFile = 0;// tell wb to use char ** to load the grid
	return returnStateId;
}


/* Choose Cat/ Choose Mouse specific MVP functions */
void* chooseAnimalVTE(void* viewState, SDL_Event* event){
	logicalEventRef returnEvent = simpleMenuVTE(viewState, event, COMMON_MENU_NUM_BUTTONS);
	return returnEvent;
}


StateId chooseCatPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = CHOOSE_CAT;
	if (model == NULL)
		return returnStateId;
	MenuDataRef chooseCatModel = model;
	StateId chooseCatStates[COMMON_MENU_NUM_BUTTONS] = {CHOOSE_MOUSE, CAT_SKILL, chooseCatModel->preChooseCat};
	if (chooseCatModel->preChooseCat == PLAY_GAME){
		chooseCatStates[0]=PLAY_GAME;
	}
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, chooseCatStates, COMMON_MENU_NUM_BUTTONS,
			returnStateId, &chooseCatModel->chooseCatButton, NULL, 0);
	if (returnStateId == CHOOSE_MOUSE){
		chooseCatModel->isCatHuman = 1;
		chooseCatModel->preChooseMouse = CHOOSE_CAT;
	}
	else if (returnStateId == CAT_SKILL)
		chooseCatModel->currValueTemp = chooseCatModel->catSkill;
	return returnStateId;
}

StateId chooseMousePHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = CHOOSE_MOUSE;
	if (model == NULL)
		return returnStateId;
	MenuDataRef chooseMouseModel = model;
	StateId chooseMouseStates[COMMON_MENU_NUM_BUTTONS] = {PLAY_GAME, MOUSE_SKILL, chooseMouseModel->preChooseMouse};
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, chooseMouseStates, COMMON_MENU_NUM_BUTTONS,
			returnStateId, &chooseMouseModel->chooseMouseButton, NULL, 0);
	if (returnStateId == PLAY_GAME)
		chooseMouseModel->isMouseHuman = 1;
	else if (returnStateId == MOUSE_SKILL)
		chooseMouseModel->currValueTemp = chooseMouseModel->mouseSkill;
	return returnStateId;
}

void saveGridDataToFile(int worldNum, int isCatFirst, char ** gridData){
	char filename[WORLD_FILE_NAME_LEN];
	if (sprintf(filename, "%s%s%d.%s", WORLD_FILE_PATH, WORLD_FILE_NAME_PREFIX, worldNum, WORLD_FILE_NAME_TYPE) < 0){
		perrorPrint("sprintf");
		return;
	}
	FILE * worldFile = fopen(filename,"w");
	if (worldFile == NULL){
		perrorPrint("fopen");
		return;
	}

	//update numTurns
	if (fprintf(worldFile, "%d\n", DEFAULT_TURNS) < 0){
		perrorPrint("fprintf");
		return;
	}
	//update isCatFirst
	char firstAnimal[6];
	if (isCatFirst)
		strcpy(firstAnimal,"cat");
	else
		strcpy(firstAnimal,"mouse");
	if (fscanf(worldFile, "%s\n", firstAnimal) < 0){
		perrorPrint("fprintf");
		return;
	}
	//fill file by grid:
	char nextChar;
	for (int i = 0; i< ROW_NUM;i++){
		for (int j = 0; j< COL_NUM; j++){
			nextChar = gridData[i][j];
			if ((fprintf(worldFile, "%c" , nextChar)) < 0){
				perrorPrint("fprintf");
				return;
			}
			if (j == COL_NUM-1){
				if (fprintf(worldFile, "\n") < 0){
					perrorPrint("fprintf");
					return;
				}
			}
		}
	}
	//close the file
	fclose(worldFile);
}


char ** initGameData(int worldNum, int * numTurns, int * isCatFirst){
	char ** grid = initGrid();
	if (isError)
		return NULL;
	*numTurns = DEFAULT_TURNS;
	char firstAnimal[6];
	if (worldNum == 0){
		setEmptyGrid(grid);
	}// write this function!
	else{
		//open the file:
		char filename[WORLD_FILE_NAME_LEN];
		if (sprintf(filename, "%s%s%d.%s", WORLD_FILE_PATH, WORLD_FILE_NAME_PREFIX, worldNum, WORLD_FILE_NAME_TYPE) < 0){
			perrorPrint("sprintf");
			return NULL;
		}
		FILE * worldFile = fopen(filename,"r");
		if (worldFile == NULL){
			perrorPrint("fopen");
			return NULL;
		}
		//update numTurns
		if (fscanf(worldFile, "%d", numTurns) < 0){
			perrorPrint("fscanf");
			return NULL;
		}
		//update isCatFirst
		if (fscanf(worldFile, "%s", firstAnimal) < 0){
			perrorPrint("fscanf");
			return NULL;
		}
		if (strcmp(firstAnimal, "cat") == 0)
			*isCatFirst = 1;
		else
			*isCatFirst = 0;
		//fill grid by file:
		char nextChar;
		for (int i = 0; i< ROW_NUM;i++){
			for (int j = 0; j< COL_NUM; j++){
				if (j == 0){
					while (1){
						if ((fscanf(worldFile, "%c" , &nextChar)) < 0){
							perrorPrint("fscanf");
							return NULL;
						}
						if (nextChar != '\r' && nextChar != '\n')
							break;
					}
					grid[i][0] = nextChar;
				}
				else{
					if ((fscanf(worldFile, "%c" , &nextChar)) < 0){
						perrorPrint("fscanf");
						return NULL;
					}
					grid[i][j] = nextChar;
				}
			}
		}
		//close the file
		fclose(worldFile);
	}
	return grid;
}

void setEmptyGrid(char ** grid){
	for (int i = 0; i< ROW_NUM; i++){
		for (int j = 0; j< COL_NUM; j++)
			grid[i][j] = EMPTY_CELL_CHAR;
	}
}

char ** initGrid(){
	char ** grid = (char **)malloc(COL_NUM*sizeof(char *));
	if (grid == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	initColumns(ROW_NUM, COL_NUM, grid);
	if (isError)
		return NULL;
	return grid;
}

void initColumns(int rownum, int colnum, char ** grid){
    int i, j;
    for (i = 0; i<rownum ; i++){
    	grid[i] = (char *)malloc(colnum*sizeof(char));
    	if (grid[i] == NULL){
    		for (j = 0; j<i; j++)
    			free(grid[j]);
    		free(grid);
    		grid = NULL;
    		perrorPrint("malloc");
    		return;
    	}
    }
}


void freeGridData(char ** gridData){
	if (gridData != NULL){
		for (int i = 0; i< ROW_NUM; i++)
			free(gridData[i]);
	}
	free(gridData);
}
