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
			returnGUI.stop = stopMenu;
			break;
		case(CHOOSE_CAT):
			returnGUI.start = startChooseAnimal;
			returnGUI.viewTranslateEvent = chooseAnimalVTE;
			returnGUI.presenterHandleEvent = chooseCatPHE;
			returnGUI.stop = stopMenu;
			break;
		case(CHOOSE_MOUSE):
			returnGUI.start = startChooseAnimal;
			returnGUI.viewTranslateEvent = chooseAnimalVTE;
			returnGUI.presenterHandleEvent = chooseMousePHE;
			returnGUI.stop = stopMenu;
			break;
		case(CAT_SKILL):
			returnGUI.start = startAnimalSkill;
			returnGUI.viewTranslateEvent = complexMenuVTE;
			returnGUI.presenterHandleEvent = catSkillPHE;
			returnGUI.stop = stopMenu;
			break;
		case(MOUSE_SKILL):
			returnGUI.start = startAnimalSkill;
			returnGUI.viewTranslateEvent = complexMenuVTE;
			returnGUI.presenterHandleEvent = mouseSkillPHE;
			returnGUI.stop = stopMenu;
			break;
		case(LOAD_GAME):
			returnGUI.start = startWorldMenu;
			returnGUI.viewTranslateEvent = complexMenuVTE;
			returnGUI.presenterHandleEvent = loadGamePHE;
			returnGUI.stop = stopMenu;
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
			returnGUI.stop = stopMenu;
			break;
		case(SAVE_WORLD):
			returnGUI.start = startWorldMenu;
			returnGUI.viewTranslateEvent = complexMenuVTE;
			returnGUI.presenterHandleEvent = saveWorldPHE;
			returnGUI.stop = stopMenu;
			break;
		case(PLAY_GAME):
			returnGUI.start = startPlayGame;
			returnGUI.viewTranslateEvent = playGameVTE;
			returnGUI.presenterHandleEvent = playGamePHE;
			returnGUI.stop = stopPlayGame;
			break;
		case(ERR_MSG):
			returnGUI.start = startErrMsg;
			returnGUI.viewTranslateEvent = errMsgVTE;
			returnGUI.presenterHandleEvent = errMsgPHE;
			returnGUI.stop = stopMenu;
			break;
		default:
			break;
	}
	return returnGUI;
}
/* helper functions for MVP */

// init data functions:
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
	viewState->gridItemImage = NULL;
	viewState->menuButtons = NULL;
	viewState->UITree = NULL;
	viewState->currButton = 0;
	viewState->gridPanel = NULL;
	viewState->gridItemsImgArr = NULL;
	viewState->labelArr = NULL;
	viewState->topPanelNode = NULL;
	viewState->sidePanelNode = NULL;
	return viewState;
}

void initMenuModel(GUIref gui, void* initData){
	if (initData == NULL){
		gui->model = initMenuDataToDefault();
	}
	else{
		gui->model = initData;
	}
}

void initWorldBuilderModel(GUIref gui, void* initData){
	MenuDataRef menuData = initData;

	WBDataRef wbData = malloc(sizeof(WBData));
	if (wbData == NULL){
		perrorPrint("malloc");
	}
	gui->model = wbData;
	wbData->gameGridData = NULL;
	wbData->editedWorld = menuData->editedWorld;
	wbData->currValueTemp = menuData->currValueTemp; // maybe?????

	gridItemPosition currPos = {0, 0};

	if ((menuData->preWorldBuilder == SAVE_WORLD && menuData->loadFromFile == 0) || menuData->preWorldBuilder == ERR_MSG){ //we pressed back in SAVE_WORLD or were in errMsg
		wbData->gameGridData = menuData->gameGridData;
		wbData->currPos = menuData->wbCurrPos;
		wbData->numTurns = menuData->numTurns;
		wbData->isCatFirst = menuData->isCatFirst; //check this!!!
	}
	else{ // preWorldBuilder == MAIN_MENU || (preWorldBuilder == EDIT_GAME || preWorldBuilder = SAVE_WORLD) && loadFromFile=1
		wbData->gameGridData = initGameDataByFile(wbData->editedWorld, &wbData->numTurns, &wbData->isCatFirst);
		wbData->currPos = currPos;
	}

	updateItemsPositions(&wbData->mousePos,&wbData->catPos,&wbData->cheesePos, wbData->gameGridData);

	free(initData);
}

void initPlayGameModel(GUIref gui, void* initData){
	MenuDataRef menuData = initData;

	PGDataRef pgData = malloc(sizeof(PGData));
	if (pgData == NULL){
		perrorPrint("malloc");
	}
	gui->model = pgData;
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

	pgData->loadGameWorld = menuData->loadGameWorld; // maybe we don't need to know this in pg!!!!
	pgData->isCatHuman = menuData->isCatHuman;
	pgData->isMouseHuman = menuData->isMouseHuman;
	pgData->catSkill = menuData->catSkill;
	pgData->mouseSkill = menuData->mouseSkill;
	pgData->isGamePaused = menuData->isGamePaused;

	pgData->isGameOver = 0;
	pgData->doRestartGame = 0;


	updateItemsPositions(&pgData->mousePos,&pgData->catPos,&pgData->cheesePos, pgData->gameGridData);

	free(initData);
}

void setTopPanelGameOver(PGDataRef pgModel, ViewStateref pgViewState){
	freeDecendents(pgViewState->topPanelNode, freeWidget);
	clearPanel(pgViewState->topPanelNode->data);
	if(isError)
		return;
	Widget * gameOverLabel = create_image(calcGameOverLabelX(), calcGameOverLabelY(), GAME_OVER_LABEL_W, GAME_OVER_LABEL_H,
			pgViewState->image, 2*PANEL_BUTTON_W+2*DIGIT_LABEL_W, 4*STATE_LABEL_H+pgModel->gameOverType*GAME_OVER_LABEL_H);
	ListRef gameOverLabelNode = addChildNode(pgViewState->topPanelNode, gameOverLabel);
	if (gameOverLabelNode == NULL){
		freeWidget(gameOverLabel);
		return;
	}
	treeDFS(pgViewState->topPanelNode, calcAbsWidgetXY, addChildWidgetsToParent);
}

void setTopPanelPlayGame(PGDataRef pgModel, ViewStateref pgViewState){
	freeDecendents(pgViewState->topPanelNode, freeWidget);
	clearPanel(pgViewState->topPanelNode->data);
	//set first row labels - player and num turns:
	Widget * playerMoveLabel = create_image(calcMoveLabelX(pgModel->isCatCurrPlayer), PANEL_WIDGET_Y_GAP, GAME_INFO_LABEL_W,
			GAME_INFO_LABEL_H, pgViewState->image, 2*PANEL_BUTTON_W+2*DIGIT_LABEL_W, 4*STATE_LABEL_H+3*GAME_OVER_LABEL_H);
	if (playerMoveLabel == NULL)
		return;
	pgViewState->labelArr[0] = playerMoveLabel;
	ListRef moveLabelNode = addChildNode(pgViewState->topPanelNode, playerMoveLabel);
	if (moveLabelNode == NULL){
		freeWidget(playerMoveLabel);
		return;
	}
	Widget * turnsTensLabel = create_image(calcTensLabelX(pgModel->isCatCurrPlayer), PANEL_WIDGET_Y_GAP,
			DIGIT_LABEL_W,GAME_INFO_LABEL_H, pgViewState->image, 2*PANEL_BUTTON_W, 3*PAUSE_BUTTON_H);
	if (turnsTensLabel == NULL)
		return;
	pgViewState->labelArr[1] = turnsTensLabel;
	ListRef turnsTensLabelNode = addChildNode(pgViewState->topPanelNode, turnsTensLabel);
	if (turnsTensLabelNode == NULL){
		freeWidget(turnsTensLabel);
		return;
	}
	Widget * turnsUnitsLabel = create_image(calcUnitsLabelX(pgModel->isCatCurrPlayer), PANEL_WIDGET_Y_GAP, DIGIT_LABEL_W,
			GAME_INFO_LABEL_H, pgViewState->image, 2*PANEL_BUTTON_W+DIGIT_LABEL_W, 3*PAUSE_BUTTON_H);
	if (turnsUnitsLabel == NULL)
		return;
	pgViewState->labelArr[2] = turnsUnitsLabel;
	ListRef turnsUnitsLabelNode = addChildNode(pgViewState->topPanelNode, turnsUnitsLabel);
	if (turnsUnitsLabelNode == NULL){
		freeWidget(turnsUnitsLabel);
		return;
	}

	//set the second row label
	Widget * playerStateLabel = create_image(calcStateLabelX(), calcStateLabelY(), STATE_LABEL_W,
			STATE_LABEL_H, pgViewState->image, PAUSE_BUTTON_W, 0);
	if (playerStateLabel == NULL)
		return;
	pgViewState->labelArr[3] = playerStateLabel;
	ListRef stateLabelNode = addChildNode(pgViewState->topPanelNode, playerStateLabel);
	if (stateLabelNode == NULL){
		freeWidget(playerStateLabel);
		return;
	}

	//set the pause button
	Widget * pauseButton = create_button(calcPauseButtonX(), calcPauseButtonY(), PAUSE_BUTTON_W,
			PAUSE_BUTTON_H, pgViewState->image, 0, 0, 0, 0, 0);
	if (pauseButton == NULL)
		return;
	pgViewState->menuButtons[0] = pauseButton;
	ListRef pauseButtonNode = addChildNode(pgViewState->topPanelNode, pauseButton);
	if (pauseButtonNode == NULL){
		freeWidget(pauseButton);
		return;
	}
	updateTopPanelPlayGame(pgViewState, pgModel);
}

void updateTopPanelPlayGame(ViewStateref pgViewState, PGDataRef pgModel){
	setPlayerMoveLabel(pgModel, pgViewState);
	setNumTurnsLabels(pgModel, pgViewState);
	setPlayerStateLabel(pgModel, pgViewState);
	setPauseButton(pgModel, pgViewState);
	treeDFS(pgViewState->topPanelNode, calcAbsWidgetXY, addChildWidgetsToParent);
}


void setPauseButton(PGDataRef pgModel, ViewStateref pgViewState){
	Widget * pauseButton = pgViewState->menuButtons[0];
	if (pgModel->isGamePaused){
		pauseButton->button_non_selected_rect.y = 2*PAUSE_BUTTON_H;
		pauseButton->button_selected_rect.y = 2*PAUSE_BUTTON_H;
	}
	else{
		if (isCurrPlayerHuman(pgModel)){
			pauseButton->button_non_selected_rect.y = PAUSE_BUTTON_H;
			pauseButton->button_selected_rect.y = PAUSE_BUTTON_H;
		}
		else{
			pauseButton->button_non_selected_rect.y = 0;
			pauseButton->button_selected_rect.y = 0;
		}
	}
	pauseButton->img_rect = pauseButton->button_non_selected_rect;
}


void setNumTurnsLabels(PGDataRef pgModel, ViewStateref pgViewState){
	Widget * turnsTensLabel = pgViewState->labelArr[1];
	Widget * turnsUnitsLabel = pgViewState->labelArr[2];
	int turnsTensDigit = pgModel->numTurnsLeft/10;
	int turnsUnitsDigit = pgModel->numTurnsLeft%10;
	int yStartPoint = 3*PAUSE_BUTTON_H;
	turnsUnitsLabel->img_rect.y = yStartPoint + turnsUnitsDigit*GAME_INFO_LABEL_H;
	turnsTensLabel->img_rect.y = yStartPoint + turnsTensDigit*GAME_INFO_LABEL_H;
}

void setPlayerMoveLabel(PGDataRef pgModel, ViewStateref pgViewState){
	Widget * moveLabel = pgViewState->labelArr[0];
	int yStartPoint = 4*STATE_LABEL_H+3*GAME_OVER_LABEL_H;
	if (pgModel->isCatCurrPlayer)
		moveLabel->img_rect.y = yStartPoint;
	else
		moveLabel->img_rect.y = 4*STATE_LABEL_H+3*GAME_OVER_LABEL_H + GAME_INFO_LABEL_H;
}


void setPlayerStateLabel(PGDataRef pgModel, ViewStateref pgViewState){
	int yStartPoint = 0;
	Widget * label = pgViewState->labelArr[3];
	if (isCurrPlayerHuman(pgModel)){
		if (pgModel->isGamePaused)
			label->img_rect.y = yStartPoint + STATE_LABEL_H;
		else
			label->img_rect.y = yStartPoint;
	}
	else{ //cuurent player is machine
		if (pgModel->isGamePaused)
			label->img_rect.y = yStartPoint + 3*STATE_LABEL_H;
		else
			label->img_rect.y = yStartPoint + 2*STATE_LABEL_H;
	}
}


int isCurrPlayerHuman(PGDataRef pgModel){
	if ((pgModel->isCatCurrPlayer && pgModel->isCatHuman) ||
			(!pgModel->isCatCurrPlayer && pgModel->isMouseHuman))
		return 1;
	return 0;
}

void addButtonsToWBTopPanel(ViewStateref viewState, int buttonImgX, int buttonImgY,
		int buttonImgDisX, int buttonImgDisY, int fromButtonNum, int toButtonNum, int isDisabled){
	int topButtonX = calcTopButtonX(), topButtonY = calcTopButtonY();

	for (int i = 0; i < WB_TOP_PANEL_NUM_BUTTONS; i++){
		viewState->menuButtons[i] = create_button(topButtonX, topButtonY, PANEL_BUTTON_W, PANEL_BUTTON_H,
				viewState->image, buttonImgX, buttonImgY, buttonImgX, buttonImgY, 0);
		if (viewState->menuButtons[i] == NULL){
			return;
		}
		ListRef newButtonNode = addChildNode(viewState->topPanelNode, viewState->menuButtons[i]);
		if (newButtonNode == NULL){
			freeWidget(viewState->menuButtons[i]);
			return;
		}
		buttonImgY += PANEL_BUTTON_H;
		topButtonX += PANEL_BUTTON_W + WB_BUTTON_X_GAP;
	}
}

void addButtonsToSidePanel(ViewStateref viewState, int buttonImgX, int buttonImgY,
		int buttonImgDisX, int buttonImgDisY, int fromButtonNum, int toButtonNum, int isDisabled){
	int sideButtonX = calcSideButtonX(), sideButtonY = calcSideButtonY();
	// Add buttons to buttons array and to UI tree
	for (int i = fromButtonNum; i < toButtonNum; i++){
		viewState->menuButtons[i] = create_button(sideButtonX, sideButtonY, PANEL_BUTTON_W, PANEL_BUTTON_H,
				viewState->image, buttonImgDisX, buttonImgDisY, buttonImgX, buttonImgY, isDisabled);//write function for creating a non markable button
		if (viewState->menuButtons[i] == NULL){
			return;
		}
		ListRef newButtonNode = addChildNode(viewState->sidePanelNode, viewState->menuButtons[i]);
		if (newButtonNode == NULL){
			freeWidget(viewState->menuButtons[i]);
			return;
		}
		buttonImgY += PANEL_BUTTON_H;
		buttonImgDisY += PANEL_BUTTON_H;
		sideButtonY += PANEL_BUTTON_H + PANEL_WIDGET_Y_GAP;
	}
}


void createGridItemsImgArr(ViewStateref viewState){
	SDL_Surface * gridItemsImg = SDL_LoadBMP("images/GridItems.bmp");
	if (gridItemsImg == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	Widget ** gridItemImagesArr = malloc(NUM_GRID_ITEMS*sizeof(Widget *));
	if (gridItemImagesArr == NULL){
		perrorPrint("malloc");
		return;
	}
	viewState->gridItemsImgArr = gridItemImagesArr;
	int gridItemImgX = 0;
	int gridItemImgY = 0;
	for (int i = 0; i < NUM_GRID_ITEMS; i++){
		gridItemImagesArr[i] = create_image(0, 0, GRID_SQUARE_SIZE, GRID_SQUARE_SIZE,
				gridItemsImg, gridItemImgX, gridItemImgY);
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

void setThreePartLayout(ViewStateref viewState, char ** gameGridData){
	Widget *win = create_window(WIN_W,WIN_H, 193, 201, 255);
	if (win == NULL){
		return;
	}
	ListRef winNode = newList(win);
	viewState->UITree = winNode;
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
	viewState->topPanelNode = topPanelNode;

	Widget *sidePanel = create_panel(0, 210, SIDE_PANEL_W, SIDE_PANEL_H,PANEL_RED,PANEL_GREEN,PANEL_BLUE);
	if (sidePanel == NULL){
		return;
	}
	ListRef sidePanelNode = addChildNode(winNode, sidePanel);
	if (sidePanelNode == NULL){
		freeWidget(sidePanel);
		return;
	}
	viewState->sidePanelNode = sidePanelNode;
	Widget *gridPanel = create_panel(210, 210, GRID_SIZE, GRID_SIZE, PANEL_RED, PANEL_GREEN, PANEL_BLUE);
	if (gridPanel == NULL){
		return;
	}
	viewState->gridPanel = gridPanel;//put panel in viewstate
	ListRef gridPanelNode = addChildNode(winNode, gridPanel);
	if (gridPanelNode == NULL){
		freeWidget(gridPanel);
		return;
	}
	createGridByData(gridPanel, gameGridData, viewState->gridItemsImgArr);
}


int updateGameOver(PGDataRef pgModel){
	gameOverType type = checkGameOverType(pgModel->catPos, pgModel->mousePos, pgModel->cheesePos, pgModel->numTurnsLeft);
	pgModel->gameOverType = type;
	if (type == GAME_NOT_OVER)
		return 0;
	return 1;
}




//void printPos(gridItemPosition pos){
//	printf("col: %d", pos.col);
//	printf("row: %d \n", pos.row);
//}



void freeViewState(ViewStateref guiViewState){
	if (guiViewState != NULL){
		if (guiViewState->menuButtons != NULL)
			free(guiViewState->menuButtons);
		if (guiViewState->image != NULL)
			SDL_FreeSurface(guiViewState->image);
		if (guiViewState->bgImage != NULL)
			SDL_FreeSurface(guiViewState->bgImage);
		if (guiViewState->gridItemImage != NULL)
			SDL_FreeSurface(guiViewState->gridItemImage);
		if (guiViewState->UITree != NULL)
			freeTree(guiViewState->UITree, freeWidget);
		if (guiViewState->gridItemsImgArr != NULL)
			freeGridItems(guiViewState->gridItemsImgArr);
		if (guiViewState->labelArr != NULL)
			free(guiViewState->labelArr);
		free(guiViewState);
	}
}

void freeWBData(WBDataRef wbData, int doFreeGrid){
	if (wbData != NULL){
		if (doFreeGrid)
			freeGridData(wbData->gameGridData);
		free(wbData);
	}
}

void freePGData(PGDataRef pgData, int doFreeGrid){
	if (pgData != NULL){
		if (doFreeGrid)
			freeGridData(pgData->gameGridData);
		free(pgData);
	}
}


void freeGridItems(Widget ** gridItemsImages){
	for (int i = 0; i < NUM_GRID_ITEMS; i++)
		freeWidget(gridItemsImages[i]);
	free(gridItemsImages);
}



void createGridByData(Widget *gridPanel, char **gridData, Widget **gridItemImages){
	Widget *currItemImage = NULL;

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
			currItemImage->location_rect.x = j*(GRID_SQUARE_SIZE + GRID_GAP_SIZE);
			currItemImage->location_rect.y = i*(GRID_SQUARE_SIZE + GRID_GAP_SIZE);
			if (SDL_BlitSurface(currItemImage->surface, &currItemImage->img_rect,
				gridPanel->surface, &currItemImage->location_rect) != 0){
					sdlErrorPrint("failed to blit image");
					return;
			}
		}
	}

}

void selectGridPos(Widget *gridPanel, Widget ** gridItemstImagesArr, gridItemPosition currPos){
	blitItemToGrid(gridPanel, gridItemstImagesArr[SELECT], currPos.row, currPos.col);
}

void deselectGridPos(Widget *gridPanel, Widget ** gridItemstImagesArr, gridItemPosition currPos){
	blitItemToGrid(gridPanel, gridItemstImagesArr[DESELECT], currPos.row, currPos.col);
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
			return -1; //check!!
	}
}


int isGridInvalid(WBDataRef wbModel){
	gridItemPosition invalidPos = {-1,-1};
	int missingItems = 0, mouse = 1, cat = 2, cheese = 4;
	int isInvalid = 0;
	if (isSamePos(wbModel->catPos, invalidPos)){
		isInvalid = 1;
		missingItems |= cat;
	}
	if (isSamePos(wbModel->mousePos, invalidPos)){
		isInvalid = 1;
		missingItems |= mouse;
	}
	if (isSamePos(wbModel->cheesePos, invalidPos)){
		isInvalid = 1;
		missingItems |= cheese;
	}
	wbModel->missingItems = missingItems;
	return isInvalid;
}

void addReusableItemToPos(gridItem itemType, char ** gridData, Widget * gridPanel,
		Widget ** gridItemsImages, gridItemPosition currPos){
	Widget * itemImage = gridItemsImages[itemType];
	blitItemToGrid(gridPanel, itemImage, currPos.row, currPos.col);
	if (itemType == WALL)
		gridData[currPos.row][currPos.col] = WALL_CHAR;
	else
		gridData[currPos.row][currPos.col] = EMPTY_CELL_CHAR;
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
	blitUpToWindow(gridPanel);
}


void moveItemToPos(gridItem itemType, Widget ** gridItemsImages, Widget * gridPanel, char ** gridData,
		gridItemPosition currPos, gridItemPosition * prevItemPos){
	gridItemPosition noPos = {-1,-1};
	if (isSamePos(*prevItemPos, noPos) == 0){
		blitItemToGrid(gridPanel, gridItemsImages[EMPTY], prevItemPos->row, prevItemPos->col);
		if (isError)
			return;
		gridData[prevItemPos->row][prevItemPos->col] = getItemChar(EMPTY);
	}
	prevItemPos->row = currPos.row;
	prevItemPos->col = currPos.col;
	gridData[currPos.row][currPos.col] = getItemChar(itemType);
	blitItemToGrid(gridPanel, gridItemsImages[itemType], currPos.row, currPos.col);
}

char getItemChar(gridItem item){
	char itemsChars[5] = {CAT_CHAR, MOUSE_CHAR, CHEESE_CHAR, WALL_CHAR, EMPTY_CELL_CHAR};
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



void changeSelectedPosByArrow(Widget * gridPanel, Widget ** gridItemsImages,
		gridItemPosition * currPos, direction direction){
	gridItemPosition newPos = getPosByDirection(*currPos, direction);
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








void saveGridDataToFile(int worldNum, int isCatFirst, char ** gridData){
	char filename[WORLD_FILE_NAME_LEN];
	worldNum = worldNum ? worldNum : 1;
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
	if (fprintf(worldFile, "%s\n", firstAnimal) < 0){
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
			if (j == COL_NUM-1 && i < ROW_NUM-1){
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
	menuData->errMsgButton = 0;


	menuData->catSkill = DEFAULT_SKILL;
	menuData->mouseSkill = DEFAULT_SKILL;

	menuData->editedWorld = DEFAULT_WORLD;
	menuData->loadGameWorld = DEFAULT_WORLD;
	//menuData->saveOnWorld = MIN_VALUE; maybe we can delete this!

	menuData->currValueTemp = 0;

	menuData->gameGridData = NULL;
	menuData->isCatFirst = 0;

	menuData->preWorldBuilder = MAIN_MENU;
	menuData->preChooseCat = MAIN_MENU;
	menuData->loadFromFile = 1;
	menuData->isGamePaused = 0;

	/* what else ???? */

	return menuData;
}







void freeMenuData(MenuDataRef menuData){
	if (menuData != NULL){
		freeGridData(menuData->gameGridData);
		free(menuData);
	}
}

void makeGameMoveIfLegal(ViewStateref pgViewState, PGDataRef pgModel, gridItemPosition eventPos){
	if (!pgModel->isGamePaused && !pgModel->isGameOver){  // isCurrPlayerHuman(pgModel)
		gridItemPosition * currPlayerPos = getCurrPlayerPos(pgModel);
		gridItem currPlayerType = pgModel->isCatCurrPlayer ? CAT : MOUSE;
		if (isMoveValid(pgModel->gameGridData, *currPlayerPos, eventPos)){
			moveItemToPos(currPlayerType, pgViewState->gridItemsImgArr, pgViewState->gridPanel,
				pgModel->gameGridData, eventPos, currPlayerPos);
			//update curr player and num turns left:
			pgModel->numTurnsLeft -= 1; // go to next turn;
			//check if game is over:
			if (updateGameOver(pgModel)){ // write a function for that!!!
				pgModel->isGameOver = 1;
				setTopPanelGameOver(pgModel, pgViewState);
				enablePGSidePanelButtons(pgViewState);
				blitUpToWindow(pgViewState->sidePanelNode->data);
				blitUpToWindow(pgViewState->topPanelNode->data);
			}
			else{
				pgModel->isCatCurrPlayer = 1-pgModel->isCatCurrPlayer; //change current player
				setTopPanelPlayGame(pgModel, pgViewState);
				blitUpToWindow(pgViewState->topPanelNode->data);
			}
			changeSelectedGridSquare(pgViewState->gridPanel, pgViewState->gridItemsImgArr,
					&eventPos, *getCurrPlayerPos(pgModel));
		}
		else{ //move is illegal
			warnIllegalMove(pgViewState, eventPos, *currPlayerPos);
		}
	}
}

void makeGameMoveByArrowIfLegal(ViewStateref pgView, PGDataRef pgModel, direction direction){
	gridItemPosition currPlayerPos = pgModel->isCatCurrPlayer ? pgModel->catPos : pgModel->mousePos;
	gridItemPosition eventPos = getPosByDirection(currPlayerPos, direction);
	makeGameMoveIfLegal(pgView, pgModel, eventPos);
}

gridItemPosition * getCurrPlayerPos(PGDataRef pgModel){
	return pgModel->isCatCurrPlayer ? &pgModel->catPos : &pgModel->mousePos;
}





void warnIllegalMove(ViewStateref pgViewState, gridItemPosition eventPos, gridItemPosition currPlayerPos){
	blitItemToGrid(pgViewState->gridPanel, pgViewState->gridItemsImgArr[WARN], eventPos.row, eventPos.col);
	blitUpToWindow(pgViewState->gridPanel);
	SDL_Delay(WARN_DELAY);
	if (isSamePos(eventPos, currPlayerPos))
		blitItemToGrid(pgViewState->gridPanel, pgViewState->gridItemsImgArr[SELECT], eventPos.row, eventPos.col);
	else
		blitItemToGrid(pgViewState->gridPanel, pgViewState->gridItemsImgArr[DESELECT], eventPos.row, eventPos.col);
	blitUpToWindow(pgViewState->gridPanel);
}

void restartGame(ViewStateref pgViewState, PGDataRef pgModel){
	freeGridData(pgModel->gameGridData);
	pgModel->gameGridData = initGameDataByFile(pgModel->loadGameWorld, &pgModel->numTurnsLeft,
			&pgModel->isCatCurrPlayer);
	updateItemsPositions(&pgModel->mousePos,&pgModel->catPos,&pgModel->cheesePos, pgModel->gameGridData);
	createGridByData(pgViewState->gridPanel, pgModel->gameGridData, pgViewState->gridItemsImgArr);
	selectGridPos(pgViewState->gridPanel, pgViewState->gridItemsImgArr, *getCurrPlayerPos(pgModel));
	if (updateGameOver(pgModel)){
		pgModel->isGameOver = 1;
		setTopPanelGameOver(pgModel, pgViewState);
		enablePGSidePanelButtons(pgViewState);
	}
	else{
		pgModel->isGameOver = 0;
		pgModel->isGamePaused = 0;
		setTopPanelPlayGame(pgModel, pgViewState);
		disablePGSidePanelButtons(pgViewState);
	}
	drawGui(pgViewState->UITree);
}

void resumeGame(ViewStateref pgViewState, PGDataRef pgModel){
	pgModel->isGamePaused = 0;
	updateTopPanelPlayGame(pgViewState, pgModel);
	disablePGSidePanelButtons(pgViewState);
	blitUpToWindow(pgViewState->topPanelNode->data);
	blitUpToWindow(pgViewState->sidePanelNode->data);
}


void pauseGame(ViewStateref pgViewState, PGDataRef pgModel){
	pgModel->isGamePaused = 1;
	updateTopPanelPlayGame(pgViewState, pgModel);
	enablePGSidePanelButtons(pgViewState);
	blitUpToWindow(pgViewState->topPanelNode->data);
	blitUpToWindow(pgViewState->sidePanelNode->data);
}

void disablePGSidePanelButtons(ViewStateref pgViewState){
	for (int i = PG_TOP_PANEL_NUM_BUTTONS; i< PG_NUM_BUTTONS; i++){
		setButtonDisabled(pgViewState->menuButtons[i]);
		blitChildToParentWidget(pgViewState->menuButtons[i], pgViewState->menuButtons[i]->parentWidget);
	}
}

void enablePGSidePanelButtons(ViewStateref pgViewState){
	for (int i = PG_TOP_PANEL_NUM_BUTTONS; i< PG_NUM_BUTTONS; i++){
		setButtonEnabled(pgViewState->menuButtons[i]);
		blitChildToParentWidget(pgViewState->menuButtons[i], pgViewState->menuButtons[i]->parentWidget);
	}
}



void handleThreePartLayoutMouseSelect(SDL_Event * event, logicalEventRef returnEvent, Widget ** buttons, int numButtons){
	if (event->button.x < WIN_W - GRID_SIZE || event->button.y < WIN_H - GRID_SIZE){
		for (int i = 0; i< numButtons; i++){ //maybe create (or we have) a general function that does that!!!!
			Widget * currButton = buttons[i];
			if (isClickEventOnButton(event, currButton, REGULAR_BUTTON)){
				returnEvent->type = SELECT_BUTTON_NUM;
				returnEvent->buttonNum = i;
				return;
			}
		}
	}
	else{ // click is inside the grid
		returnEvent->type = SELECT_SQUARE;
		//MayBe write a function for that !!!!!
		returnEvent->gridPos.col = (event->button.x-(WIN_W - GRID_SIZE))/(GRID_SQUARE_SIZE+GRID_GAP_SIZE);
		returnEvent->gridPos.row = (event->button.y-(WIN_W - GRID_SIZE))/(GRID_SQUARE_SIZE+GRID_GAP_SIZE);
	}
}

void handleThreePartLayoutArrowKey(SDLKey key, logicalEventRef returnEvent){
	if (key == SDLK_UP)
		returnEvent->type = GO_UP;
	else if (key ==  SDLK_DOWN)
		returnEvent->type = GO_DOWN;
	else if (key ==  SDLK_RIGHT)
		returnEvent->type = GO_RIGHT;
	else if (key ==  SDLK_LEFT)
		returnEvent->type = GO_LEFT;
}

void updateMachineMoveIfNeeded(GUI pgGUI){
	PGDataRef pgModel = pgGUI.model;
	if (isCurrPlayerHuman(pgModel) || pgModel->isGameOver || pgModel->isGamePaused)
		return;
	ViewStateref pgViewState = pgGUI.viewState;
	GameStateRef currState = (GameStateRef)malloc(sizeof(GameState)); /* allocate memory for currState  */
	if (currState == NULL){ /* malloc failed */
		perrorPrint("malloc");
		return;
	}
	int currPlayerSkill = pgModel->isCatCurrPlayer ? pgModel->catSkill : pgModel->mouseSkill;
	currState->gridData = copyGrid(pgModel->gameGridData);
	currState->catPos = pgModel->catPos;
	currState->mousePos = pgModel->mousePos;
	currState->cheesePos = pgModel->cheesePos;
	currState->isCatCurrPlayer = pgModel->isCatCurrPlayer;
	currState->numTurnsLeft = pgModel->numTurnsLeft;
	gridItemPosition errorPos = {-1, -1};
	gridItemPosition movePos = suggestMove(currState, currPlayerSkill);
	if (isSamePos(movePos, errorPos)){ // there was an error while getting the suggested move
		freeState(currState); /* free memory */
		return;
	}
	SDL_Delay(1000);
	makeGameMoveIfLegal(pgViewState, pgModel,movePos);
	freeState(currState); /* free memory */
}


void initMainMenuData(GUIref mainMenuGui, void * initData){
	mainMenuGui->model = initMenuDataToDefault();
	if (initData != NULL){
		MenuDataRef data = initData;
		MenuDataRef model = mainMenuGui->model;
		model->mainMenuButton = data->mainMenuButton;
		freeMenuData(initData);
	}
}

