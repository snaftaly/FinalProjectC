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


//start functions:

/* maybe we don't need to pass initdata !!!!!!!!!!!!! */
void startGeneralMenu(GUIref gui, void * initData, char * imgPath, int titleImgX, int titleImgY, int titleWidth,
		int numButtons, int selectedButton, int firstButtonNumOpts, int value){
	/* initialize viewState */
	ViewStateref menuViewState = initGUIViewState();
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
	drawGui(gui->viewState);
}

void startMainMenu(GUIref gui, void* initData){
	initMenuModel(gui, initData);
	if(isError)
		return;
	char imgPath[] = "images/MainMenu_temp.bmp";
	MenuDataRef data = gui->model;
	int currentButton = data->mainMenuButton;
	/* start the main menu gui */
	startGeneralMenu(gui, initData, imgPath,
			MENU_BUTTON_W*2, 0, MENU_TITLE_W, MAIN_MENU_NUM_BUTTONS, currentButton, 1, 0);
}

void startChooseAnimal(GUIref gui, void* initData){
	initMenuModel(gui, initData);
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
			MENU_BUTTON_W*2, titleImgY, MENU_TITLE_W, COMMON_MENU_NUM_BUTTONS, currentButton, 1, 0);
}

void startAnimalSkill(GUIref gui, void* initData){
	initMenuModel(gui, initData);
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
			MENU_BUTTON_W*2, titleImgY, MENU_TITLE_W, COMMON_MENU_NUM_BUTTONS,currentButton, MAX_SKILL_VALUE, currentValue);
}


void startWorldMenu(GUIref gui, void* initData){
	initMenuModel(gui, initData);
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
			currentValue = data->editedWorld ? data->editedWorld: 1; //if edited world is 0 this will show 1
			titleImgY = 2*MENU_TITLE_H;
			break;
		default:
			break;
	}
	startGeneralMenu(gui, initData, imgPath,
			MENU_BUTTON_W*2, titleImgY, MENU_TITLE_W, COMMON_MENU_NUM_BUTTONS,currentButton, MAX_WORLD, currentValue);
}

void startWorldBuilder(GUIref gui, void* initData){
	initWorldBuilderModel(gui, initData);
	if(isError)
		return;
	WBDataRef wbModel = gui->model;

	// initialize viewState
	ViewStateref wbViewState = initGUIViewState();
	if (wbViewState == NULL){
		return;
	}
	gui->viewState = wbViewState;

	// create image surface for gui
	char imgPath[] = "images/worldBuilder_temp.bmp";
	SDL_Surface * wbImage = SDL_LoadBMP(imgPath);
	if (wbImage == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	wbViewState->image = wbImage;

	//create grid Items array
	createGridItemsImgArr(wbViewState);
	if (isError)
		return;

	//set the layout of world builder gui
	setThreePartLayout(wbViewState, wbModel->gameGridData);

	// create buttons array
	Widget ** buttons = (Widget **)malloc(WB_NUM_BUTTONS*sizeof(Widget *));
	if (buttons == NULL){
		perrorPrint("malloc");
		return;
	}
	wbViewState->menuButtons = buttons;

	// set up wb top label
	Widget *label = create_image(calcWBtitleX(WB_TITLE_W), PANEL_WIDGET_Y_GAP, WB_TITLE_W, WB_TITLE_H,
			wbImage, PANEL_BUTTON_W, wbModel->editedWorld*WB_TITLE_H);
	if (label == NULL){
		return;
	}
	ListRef labelNode = addChildNode(wbViewState->topPanelNode, label);
	if (labelNode == NULL){
		freeWidget(label);
		return;
	}

	//add buttons to top panel:
	int topButtonX = calcTopButtonX(), topButtonY = calcTopButtonY(), buttonImgX = 0, buttonImgY = 0;
	// Add buttons to buttons array and to UI tree
	for (int i = 0; i < WB_TOP_PANEL_NUM_BUTTONS; i++){
		buttons[i] = create_button(topButtonX, topButtonY, PANEL_BUTTON_W, PANEL_BUTTON_H,
				wbImage, buttonImgX, buttonImgY, buttonImgX, buttonImgY, 0);//write function for creating a non markable button
		if (buttons[i] == NULL){
			return;
		}
		ListRef newButtonNode = addChildNode(wbViewState->topPanelNode, buttons[i]);
		if (newButtonNode == NULL){
			freeWidget(buttons[i]);
			return;
		}
		buttonImgY += PANEL_BUTTON_H;
		topButtonX += PANEL_BUTTON_W + WB_BUTTON_X_GAP;
	}

	//add buttons to side panel:
	addButtonsToSidePanel(wbViewState, buttonImgX, buttonImgY, buttonImgX, buttonImgY, WB_TOP_PANEL_NUM_BUTTONS,
			WB_NUM_BUTTONS, 1);

	selectGridPos(wbViewState->gridPanel, wbViewState->gridItemsImgArr, wbModel->currPos);
	if (isError)
		return;
	// draw GUI according to UItree
	drawGui(gui->viewState);
}

/* Main Menu specific MVP functions */
void startErrMsg(GUIref gui, void* initData){
	initMenuModel(gui, initData);
	if(isError)
		return;
	MenuDataRef data = gui->model;

	char imgPath[] = "images/ErrMsg_temp.bmp";

	ViewStateref menuViewState = initGUIViewState();
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
	Widget ** buttons = (Widget **)malloc(ERR_MSG_NUM_BUTTONS*sizeof(Widget *));
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
	Widget *panel = create_panel(calcPanelX(ERR_MSG_TITLE_W), calcErrPanelY(),
			calcPanelWidth(ERR_MSG_TITLE_W),calcErrPanelHeight(),PANEL_RED,PANEL_GREEN,PANEL_BLUE);
	if (panel == NULL){
		return;
	}
	ListRef panel_node = addChildNode(win_node, panel);
	if (panel_node == NULL){
		freeWidget(panel);
		return;
	}
	Widget *label = create_image(MENU_TITLE_X_GAP, MENU_TITLE_Y_GAP, ERR_MSG_TITLE_W, MENU_TITLE_H,
			menuImage, 0, 0);
	if (label == NULL){
		return;
	}
	ListRef label_node = addChildNode(panel_node, label);
	if (label_node == NULL){
		freeWidget(label);
		return;
	}
	Widget * missingItemMsg = create_image(calcMisItemMsgX(MISSING_ITEM_MSG_W, ERR_MSG_TITLE_W), calcMisItemMsgY(),
			MISSING_ITEM_MSG_W, MENU_TITLE_H, menuImage, 0, data->missingItems*MENU_TITLE_H);
	if (missingItemMsg == NULL){
		return;
	}
	ListRef Msg_node = addChildNode(panel_node, missingItemMsg);
	if (Msg_node == NULL){
		freeWidget(label);
		return;
	}

	/* Add buttons to buttons array and to UI tree */
	int button_x = calcMenuButtonX(ERR_MSG_TITLE_W), button_y = calcErrMsgButtonY(), isSelected_x = ERR_MSG_TITLE_W, isSelected_y = 0;
	buttons[0] = create_button(button_x,button_y, MENU_BUTTON_W, MENU_BUTTON_H,
			menuImage, isSelected_x, isSelected_y, isSelected_x, isSelected_x, 0);
	if (buttons[0] == NULL){
		return;
	}
	ListRef newButtonNode = addChildNode(panel_node, buttons[0]);
	if (newButtonNode == NULL){
		freeWidget(buttons[0]);
		return;
	}
	setButtonSelected(menuViewState->menuButtons[0]);
	/* draw GUI according to UItree */
	drawGui(gui->viewState);
}

void startPlayGame(GUIref gui, void* initData){
	initPlayGameModel(gui, initData);
	if(isError)
		return;
	PGDataRef pgModel = gui->model;

	// initialize viewState
	ViewStateref pgViewState = initGUIViewState();
	if (pgViewState == NULL){
		return;
	}
	gui->viewState = pgViewState;

	//check if game is over
	pgModel->isGameOver = updateGameOver(pgModel);


	// create image surface for gui
	char imgPath[] = "images/PlayGame_temp.bmp";
	SDL_Surface * pgImage = SDL_LoadBMP(imgPath);
	if (pgImage == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	pgViewState->image = pgImage;

	//create grid items array
	createGridItemsImgArr(pgViewState);
	if (isError)
		return;

	//set the layout of play game gui
	setThreePartLayout(pgViewState, pgModel->gameGridData);
	if (isError)
		return;

	// create buttons array
	Widget ** buttons = (Widget **)malloc(PG_NUM_BUTTONS*sizeof(Widget *));
	if (buttons == NULL){
		perrorPrint("malloc");
		return;
	}
	pgViewState->menuButtons = buttons;

	// create labels array
	Widget ** labels = (Widget **)malloc(PG_NUM_LABELS*sizeof(Widget *));
	if (labels == NULL){
		perrorPrint("malloc");
		return;
	}
	pgViewState->labelArr = labels;

	//add buttons to side panel:
	int buttonImgX = 0, buttonImgY = 3*PAUSE_BUTTON_H;
	int buttonImgDisX = PANEL_BUTTON_W, buttonImgDisY = 3*PAUSE_BUTTON_H;
	addButtonsToSidePanel(pgViewState, buttonImgX, buttonImgY, buttonImgDisX, buttonImgDisY,
			PG_TOP_PANEL_NUM_BUTTONS, PG_NUM_BUTTONS, 1-pgModel->isGameOver);
	if (isError)//check if we need this
		return;

	//handle top panel presentation
	if (pgModel->isGameOver)
		setTopPanelGameOver(pgModel, pgViewState);
	else
		setTopPanelPlayGame(pgModel, pgViewState);
	if (isError)
		return;

	//set selected position to current player
	selectGridPos(pgViewState->gridPanel, pgViewState->gridItemsImgArr, *getCurrPlayerPos(pgModel));
	if (isError)
		return;

	// draw GUI according to UItree
	drawGui(gui->viewState);
}

//vte functions:
void* simpleMenuVTE(void* viewState, SDL_Event* event, int numOfButtons){
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	returnEvent->type = NO_EVENT;
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

void* mainMenuVTE(void* viewState, SDL_Event* event){
	logicalEventRef returnEvent = simpleMenuVTE(viewState, event, MAIN_MENU_NUM_BUTTONS);
	return returnEvent;
}

void* chooseAnimalVTE(void* viewState, SDL_Event* event){
	logicalEventRef returnEvent = simpleMenuVTE(viewState, event, COMMON_MENU_NUM_BUTTONS);
	return returnEvent;
}

void* worldBuilderVTE(void* viewState, SDL_Event* event){
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	returnEvent->type = NO_EVENT;
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
			else
				handleThreePartLayoutArrowKey(key, returnEvent);
			break;
		case (SDL_MOUSEBUTTONUP):
			handleThreePartLayoutMouseSelect(event, returnEvent, wbViewState->menuButtons, WB_NUM_BUTTONS);
			break;
		default:
			returnEvent->type = NO_EVENT;
	}
	return returnEvent;
}

void* errMsgVTE(void* viewState, SDL_Event* event){
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	ViewStateref menuViewState = viewState;
	Widget * currButton = menuViewState->menuButtons[0];
	returnEvent->type = NO_EVENT;

	switch (event->type) {
		case (SDL_KEYUP):
			if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_KP_ENTER){
				returnEvent->type = SELECT_CURR_BUTTON;
			}
			break;
		case (SDL_MOUSEBUTTONUP):
			if (isClickEventOnButton(event, currButton, REGULAR_BUTTON)){
				returnEvent->type = SELECT_BUTTON_NUM;
				returnEvent->buttonNum = 0;
			}
			break;
		default:
			returnEvent->type = NO_EVENT;
	}

	return returnEvent;
}

void* playGameVTE(void* viewState, SDL_Event * event){
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	returnEvent->type = NO_EVENT;
	ViewStateref pgViewState = viewState;
	SDLKey key;
	switch (event->type) {
		case (SDL_KEYUP):
			key = event->key.keysym.sym;
			if (key == SDLK_SPACE || key == SDLK_F1 || key == SDLK_F2 || key == SDLK_F3 || key == SDLK_F4 ||
					key == SDLK_ESCAPE){
				returnEvent->type = SELECT_BUTTON_NUM;
				returnEvent->buttonNum = getPGButtonNum(key);
			}
			else
				handleThreePartLayoutArrowKey(key, returnEvent);
			break;
		case (SDL_MOUSEBUTTONUP):
			handleThreePartLayoutMouseSelect(event, returnEvent, pgViewState->menuButtons, PG_NUM_BUTTONS);
			break;
		default:
			returnEvent->type = NO_EVENT;
	}
	return returnEvent;
}





// PHE functions:

StateId generalMenuPHE(void* model, void* viewState, void* logicalEvent, StateId states[], int numOfButtons,
		StateId stateId, int* currButton, int* currValue, int maxValue){
	StateId returnStateId = stateId;
	if (logicalEvent == NULL || viewState == NULL || model == NULL)
		return returnStateId;
	logicalEventRef menuEvent = logicalEvent;
	ViewStateref menuView = viewState;
	MenuDataRef menuModel = menuModel;
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
	if (returnStateId == CHOOSE_CAT){ //new game is pressed
		mainMenuModel->preChooseCat = MAIN_MENU;
		mainMenuModel->loadGameWorld = 1;
		mainMenuModel->loadFromFile = 1;
	}
	else if (returnStateId == WORLD_BUILDER){
		mainMenuModel->preWorldBuilder = MAIN_MENU;
		mainMenuModel->editedWorld = 0; // we can delete this line!!!
	}
	else if (returnStateId == LOAD_GAME)
		mainMenuModel->loadGameWorld = DEFAULT_WORLD;
	else if (returnStateId == EDIT_GAME)
		mainMenuModel->editedWorld = DEFAULT_WORLD;
	mainMenuModel->retStateId = returnStateId;
	return returnStateId;
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
	chooseCatModel->retStateId = returnStateId;
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
	chooseMouseModel->retStateId = returnStateId;
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
	catSkillModel->retStateId = returnStateId;
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
	mouseSkillModel->retStateId = returnStateId;
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
	if (returnStateId == CHOOSE_CAT){ //done was pressed
		loadGameModel->preChooseCat = LOAD_GAME;
		loadGameModel->loadFromFile = 1;
	}
	loadGameModel->retStateId = returnStateId;
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
	editGameModel->retStateId = returnStateId;
	return returnStateId;
}

StateId saveWorldPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = SAVE_WORLD;
	if (model == NULL)
		return returnStateId;
	MenuDataRef saveWorldModel = model;
	StateId saveWorldStates[COMMON_MENU_NUM_BUTTONS] = {SAVE_WORLD, WORLD_BUILDER, WORLD_BUILDER};
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, saveWorldStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &saveWorldModel->saveWorldButton, &saveWorldModel->currValueTemp, MAX_WORLD);
	if (returnStateId == WORLD_BUILDER){ //done or back were pressed
		saveWorldModel->preWorldBuilder = SAVE_WORLD; // do we need this?
		if (saveWorldModel->saveWorldButton == 1){ //done was pressed
			saveWorldModel->loadFromFile = 1;// tell wb to use file to load the grid
			saveWorldModel->editedWorld = saveWorldModel->currValueTemp;
			saveGridDataToFile(saveWorldModel->editedWorld, saveWorldModel->isCatFirst, saveWorldModel->gameGridData);
			//free grid data
			freeGridData(saveWorldModel->gameGridData);
			saveWorldModel->gameGridData = NULL;
		}
		else if (saveWorldModel->saveWorldButton == 2) //back was pressed
			saveWorldModel->loadFromFile = 0;// tell wb to use char ** to load the grid
	}
	saveWorldModel->retStateId = returnStateId;
	return returnStateId;
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
			if(returnStateId == SAVE_WORLD && isGridInvalid(wbModel))
				returnStateId = ERR_MSG;
			else if (returnStateId == WORLD_BUILDER){
				putGridItemInPos(wbModel, wbView->gridPanel, wbView->gridItemsImgArr, wbModel->currPos, wbEvent->buttonNum);
			}
			else if (returnStateId == QUIT){
				isQuit = 1;
			}
			break;
		case(SELECT_SQUARE):
			changeSelectedGridSquare(wbView->gridPanel, wbView->gridItemsImgArr,&wbModel->currPos, wbEvent->gridPos);
			break;
		case(GO_UP):
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImgArr, &wbModel->currPos, UP);
			break;
		case(GO_DOWN):
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImgArr, &wbModel->currPos, DOWN);
			break;
		case(GO_RIGHT):
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImgArr, &wbModel->currPos, RIGHT);
			break;
		case(GO_LEFT):
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImgArr, &wbModel->currPos, LEFT);
			break;
		case(NO_EVENT):
			break;
		default:
			break;

	}
	free(logicalEvent);
	wbModel->returnStateId = returnStateId;
	return returnStateId;
}


StateId playGamePHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = PLAY_GAME;
	if (logicalEvent == NULL || viewState == NULL || model == NULL)
		return returnStateId;
	logicalEventRef pgEvent = logicalEvent;
	ViewStateref pgViewState = viewState;
	PGDataRef pgModel = model;
	StateId states[PG_NUM_BUTTONS] = {PLAY_GAME, CHOOSE_MOUSE, CHOOSE_CAT, PLAY_GAME, MAIN_MENU, QUIT};
	/////// done forget to update pre choose cat and pre choose mouse in stop function!!!!!!!!
	switch(pgEvent->type){
		case(SELECT_BUTTON_NUM):
			if (pgModel->isGamePaused || pgModel->isGameOver){
				returnStateId = states[pgEvent->buttonNum];
				if (pgEvent->buttonNum == 3){ // restart the game
					restartGame(pgViewState, pgModel);
				}
				if (pgEvent->buttonNum == 0 && pgModel->isGamePaused){ //game is paused and space was pressed
					resumeGame(pgViewState, pgModel);
				}
			}
			else if (pgEvent->buttonNum == 0){ // game is running and space was pressed
				pauseGame(pgViewState, pgModel);
				//setPauseButton(pgModel, pgViewState);// how do we update the view????? (blit up to window)
			}
			break;
		case(SELECT_SQUARE):
			makeGameMoveIfLegal(pgViewState, pgModel, pgEvent->gridPos); // will call call change selected grid square if needed
			break;
		case(GO_UP):
			makeGameMoveByArrowIfLegal(pgViewState, pgModel, UP);
			break;
		case(GO_DOWN):
			makeGameMoveByArrowIfLegal(pgViewState, pgModel, DOWN);
			break;
		case(GO_RIGHT):
			makeGameMoveByArrowIfLegal(pgViewState, pgModel, RIGHT);
			break;
		case(GO_LEFT):
			makeGameMoveByArrowIfLegal(pgViewState, pgModel, LEFT);
			break;
		case(NO_EVENT):
			break;
		default:
			break;

	}
	free(logicalEvent);
	pgModel->returnStateId = returnStateId;
	return returnStateId;
}


StateId errMsgPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = ERR_MSG;
	if (model == NULL)
		return returnStateId;
	MenuDataRef errMsgModel = model;
	StateId errMsgStates[MAIN_MENU_NUM_BUTTONS] = {WORLD_BUILDER};
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, errMsgStates, ERR_MSG_NUM_BUTTONS,
			returnStateId, &errMsgModel->errMsgButton, NULL, 0);
	if (returnStateId == WORLD_BUILDER)
		errMsgModel->preWorldBuilder = ERR_MSG;
	errMsgModel->retStateId = returnStateId;
	return returnStateId;
}



// stop functions:
void* stopMenu(GUIref gui){ /* maybe this will be a general stop function */

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
	if (isError || returnData->retStateId == QUIT){
		freeMenuData(returnData); // we need to write a function for that!
		return NULL;
	}
	return returnData;
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
	//for save world & error message:
	returnData->gameGridData = wbData->gameGridData;
	returnData->editedWorld = wbData->editedWorld;
	returnData->isCatFirst = wbData->isCatFirst;
	returnData-> numTurns = wbData->numTurns;
	returnData->wbCurrPos = wbData->currPos;
	returnData->currValueTemp = wbData->editedWorld ? wbData->editedWorld : MIN_VALUE;
	returnData->missingItems = wbData->missingItems;

	freeViewState(guiViewState);

	if (wbData != NULL){
		free(wbData); // maybe we need a function for that???? we don't want to free the char **
	}
	if (isError || wbData->returnStateId == QUIT || wbData->returnStateId == MAIN_MENU){
		freeMenuData(returnData); // we need to write a function for that
		return NULL;
	}
	return returnData;
}



void* stopPlayGame(GUI * gui){

	ViewStateref guiViewState = gui->viewState;
	PGDataRef pgData = gui->model;
	gui->model = NULL;
	gui->viewState = NULL;

	MenuDataRef returnData = initMenuDataToDefault();
	if (isError)
		return NULL;
	//put things in return Data
	//for choose cat & choose mouse:
	returnData->gameGridData = pgData->gameGridData;
	returnData->loadGameWorld = pgData->loadGameWorld;
	returnData->isCatFirst = pgData->isCatCurrPlayer;
	returnData-> numTurns = pgData->numTurnsLeft;
	returnData->catSkill = pgData->catSkill;
	returnData->mouseSkill = pgData->mouseSkill;
	returnData->gameGridData = pgData->gameGridData;
	returnData->isCatHuman = pgData->isCatHuman;
	returnData->isMouseHuman = pgData->isMouseHuman;
	returnData->preChooseCat = PLAY_GAME;
	returnData->preChooseMouse = PLAY_GAME;

	freeViewState(guiViewState);

	if (pgData != NULL){
		free(pgData); // maybe we need a function for that???? we don't want to free char **
	}
	if (isError || pgData->returnStateId == QUIT || pgData->returnStateId == MAIN_MENU){
		freeMenuData(returnData); // we need to write a function for that!
		return NULL;
	}
	return returnData;
}
