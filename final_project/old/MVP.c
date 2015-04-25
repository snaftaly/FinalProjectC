/* includes */

#include "MVP_utils.h"
#include "../main/ErrorHandling.h"


/**** start functions ****/

/* startGeneralMenu is a function used for starting all the menu GUIs */
void startGeneralMenu(GUIref gui, char * imgPath, int titleImgX, int titleImgY, int titleWidth,
		int numButtons, int selectedButton, int firstButtonNumOpts, int value){
	/* initialize viewState */
	ViewStateref menuViewState = initGUIViewState();
	if (menuViewState == NULL){
		return;
	}
	gui->viewState = menuViewState;

	/* create image an bgImage surfaces */
	SDL_Surface * menuImage = SDL_LoadBMP(imgPath);
	if (menuImage == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	menuViewState->image = menuImage;

	SDL_Surface * bgImage = SDL_LoadBMP("images/background.bmp");
	if (bgImage == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	menuViewState->bgImage = bgImage;

	/* create buttons array */
	Widget ** buttons = (Widget **)malloc(numButtons*sizeof(Widget *));
	if (buttons == NULL){
		perrorPrint("malloc");
		return;
	}
	menuViewState->buttonsArr = buttons;

	/* create the UItree: */
	/* create the window */
	Widget *win = createWindow(WIN_W,WIN_H, 193, 201, 255);
	if (win == NULL){
		return;
	}
	ListRef win_node = newList(win);
	menuViewState->UITree = win_node;
	if (win_node == NULL){
		freeWidget(win);
		return;
	}
	/* create the background image */
	Widget * bgImageWidget = create_image(0, 0, WIN_W, WIN_H,bgImage, 0, 0);
	if (bgImageWidget == NULL){
		return;
	}
	ListRef bgImage_node = addChildNode(win_node, bgImageWidget);
	if (bgImage_node == NULL){
		freeWidget(bgImageWidget);
		return;
	}
	/* create the panel */
	Widget *panel = create_panel(calcMenuPanelX(titleWidth), calcMenuPanelY(numButtons),
			calcMenuPanelWidth(titleWidth),calcMenuPanelHeight(numButtons),PANEL_RED,PANEL_GREEN,PANEL_BLUE);
	if (panel == NULL){
		return;
	}
	ListRef panel_node = addChildNode(win_node, panel);
	if (panel_node == NULL){
		freeWidget(panel);
		return;
	}
	/* create the label */
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

	/* update the view buttons according to the appropriate data */
	/* update the values button if it is a value selection button */
	if(firstButtonNumOpts > 1){
		setValuesButtonFromInit(value, buttons[0]);
	}
	/* set the correct button as the button selected */
	menuViewState->currButton = selectedButton;
	setButtonSelected(menuViewState->buttonsArr[selectedButton]);

	/* draw GUI according to UItree */
	drawGui(menuViewState->UITree);
}

/* startMainMenu is a function used for starting the Main Menu GUI
 * It calls startGeneralMenu function */
void startMainMenu(GUIref gui, void* initData){
	/* init the main menu model according to init data */
	initMainMenuData(gui, initData);
	if(isError)
		return;
	/* set the information needed to start the GUI */
	char imgPath[] = "images/MainMenu.bmp";
	MenuDataRef data = gui->model;
	int currentButton = data->mainMenuButton;
	/* call startGeneralMenu to start the GUI */
	startGeneralMenu(gui, imgPath,
			MENU_BUTTON_W*2, 0, MENU_TITLE_W, MAIN_MENU_NUM_BUTTONS, currentButton, 1, 0);
}

/* startChooseAnimal is a function used for starting choose cat and choose mouse GUI
 * It calls startGeneralMenu function, with the data of the appropriate animal */
void startChooseAnimal(GUIref gui, void* initData){
	/* init the choose cat/mouse model according to init data */
	initMenuModel(gui, initData);
	if(isError)
		return;
	/* set the information needed to start the GUI according to the GUI's stateId*/
	char imgPath[] = "images/ChooseAnimal.bmp";
	MenuDataRef data = gui->model;
	int currentButton, titleImgY;
	switch(gui->stateId){
		case(CHOOSE_CAT):
			currentButton = data->chooseCatButton;
			titleImgY = 0; /* set the title location in the GUI's widgets' image */
			break;
		case(CHOOSE_MOUSE):
			currentButton = data->chooseMouseButton;
			titleImgY = MENU_TITLE_H; /* set the title location in the the GUI's widgets' image */
			break;
		default:
			break;
	}
	/* call startGeneralMenu to start the GUI */
	startGeneralMenu(gui, imgPath,
			MENU_BUTTON_W*2, titleImgY, MENU_TITLE_W, COMMON_MENU_NUM_BUTTONS, currentButton, 1, 0);
}


/* startAnimalAkill is a function used for starting cat/mouse skill GUI
 * It calls startGeneralMenu function, with the data of the appropriate animal */
void startAnimalSkill(GUIref gui, void* initData){
	/* init the cat/mouse skill model according to init data */
	initMenuModel(gui, initData);
	if(isError)
		return;
	/* set the information needed to start the GUI according to the GUI's stateId*/
	char imgPath[] = "images/AnimalSkill.bmp";
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
	/* call startGeneralMenu to start the GUI */
	startGeneralMenu(gui, imgPath,
			MENU_BUTTON_W*2, titleImgY, MENU_TITLE_W, COMMON_MENU_NUM_BUTTONS,currentButton, MAX_SKILL_VALUE, currentValue);
}

/* startWorldMenu is a function used for starting load game/edit game/save world GUI
 * It calls startGeneralMenu function, with the data of the appropriate gui  */
void startWorldMenu(GUIref gui, void* initData){
	/* init the world menu model according to init data */
	initMenuModel(gui, initData);
	if(isError)
		return;
	/* set the information needed to start the GUI according to the GUI's stateId*/
	char imgPath[] = "images/WorldMenu.bmp";
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
	/* call startGeneralMenu to start the GUI */
	startGeneralMenu(gui, imgPath,
			MENU_BUTTON_W*2, titleImgY, MENU_TITLE_W, COMMON_MENU_NUM_BUTTONS,currentButton, MAX_WORLD, currentValue);
}

/* startWorldBuilder is a function used for starting the world builder (wb) gui */
void startWorldBuilder(GUIref gui, void* initData){
	/* init world builder model */
	initWorldBuilderModel(gui, initData);
	if(isError)
		return;
	WBDataRef wbModel = gui->model;

	/* initialize viewState */
	ViewStateref wbViewState = initGUIViewState();
	if (wbViewState == NULL){
		return;
	}
	gui->viewState = wbViewState;

	/* create image surface for the wb GUI */
	char imgPath[] = "images/WorldBuilder.bmp";
	SDL_Surface * wbImage = SDL_LoadBMP(imgPath);
	if (wbImage == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	wbViewState->image = wbImage;

	/* create grid Items array */
	createGridItemsImgArr(wbViewState);
	if (isError)
		return;

	/* set the layout of world builder gui */
	setThreePartLayout(wbViewState, wbModel->gameGridData);
	if (isError)
		return;

	/* create buttons array */
	Widget ** buttons = (Widget **)malloc(WB_NUM_BUTTONS*sizeof(Widget *));
	if (buttons == NULL){
		perrorPrint("malloc");
		return;
	}
	wbViewState->buttonsArr = buttons;

	/* setup wb top label */
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

	/* add buttons to top panel: */
	int buttonImgX = 0, buttonImgY = 0;/* set the location of the button images in the image file */
	addButtonsToWBTopPanel(wbViewState, buttonImgX, buttonImgY, buttonImgX, buttonImgY, 0,
			WB_TOP_PANEL_NUM_BUTTONS, 1);
	if (isError)
		return;

	/* add buttons to side panel: */
	buttonImgY = PANEL_BUTTON_H*WB_TOP_PANEL_NUM_BUTTONS; /* update the location of the button images in the image file */
	addButtonsToSidePanel(wbViewState, buttonImgX, buttonImgY, buttonImgX, buttonImgY, WB_TOP_PANEL_NUM_BUTTONS,
			WB_NUM_BUTTONS, 1);
	if (isError)
		return;

	/* select the correct grid position */
	selectGridPos(wbViewState->gridPanel, wbViewState->gridItemsImgArr, wbModel->currPos);
	if (isError)
		return;
	// draw GUI according to UItree
	drawGui(wbViewState->UITree);
}

/* startErrMsg is used for starting the error message GUI */
void startErrMsg(GUIref gui, void* initData){
	/* init the model according to initData */
	initMenuModel(gui, initData);
	if(isError)
		return;
	MenuDataRef data = gui->model;

	char imgPath[] = "images/ErrMsg.bmp";
	/* init the view state */
	ViewStateref errViewState = initGUIViewState();
	if (errViewState == NULL){
		return;
	}
	gui->viewState = errViewState;

	/* create image and bgImage surfaces */
	SDL_Surface * menuImage = SDL_LoadBMP(imgPath);
	if (menuImage == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	errViewState->image = menuImage;
	SDL_Surface * bgImage = SDL_LoadBMP("images/background.bmp");
	if (bgImage == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	errViewState->bgImage = bgImage;

	/* create buttons array */
	Widget ** buttons = (Widget **)malloc(ERR_MSG_NUM_BUTTONS*sizeof(Widget *));
	if (buttons == NULL){
		perrorPrint("malloc");
		return;
	}
	errViewState->buttonsArr = buttons;

	/* create the UItree */
	/* create the window */
	Widget *win = createWindow(WIN_W,WIN_H, 0, 0, 0);
	if (win == NULL){
		return;
	}
	ListRef win_node = newList(win);
	errViewState->UITree = win_node;
	if (win_node == NULL){
		freeWidget(win);
		return;
	}
	/* create the background image */
	Widget * bgImageWidget = create_image(0, 0, WIN_W, WIN_H,bgImage, 0, 0);
	if (bgImageWidget == NULL){
		return;
	}
	ListRef bgImage_node = addChildNode(win_node, bgImageWidget);
	if (bgImage_node == NULL){
		freeWidget(bgImageWidget);
		return;
	}
	/* create the panel */
	Widget *panel = create_panel(calcMenuPanelX(ERR_MSG_TITLE_W), calcErrPanelY(),
			calcMenuPanelWidth(ERR_MSG_TITLE_W),calcErrPanelHeight(),PANEL_RED,PANEL_GREEN,PANEL_BLUE);
	if (panel == NULL){
		return;
	}
	ListRef panel_node = addChildNode(win_node, panel);
	if (panel_node == NULL){
		freeWidget(panel);
		return;
	}
	/* create the labels */
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

	/* Add button to buttons array and to UI tree */
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
	/* set the button selected */
	setButtonSelected(errViewState->buttonsArr[0]);
	/* draw GUI according to UItree */
	drawGui(errViewState->UITree);
}

/* start play game is a function used to start the play game GUI */
void startPlayGame(GUIref gui, void* initData){
	/* init play game model */
	initPlayGameModel(gui, initData);
	if(isError)
		return;
	PGDataRef pgModel = gui->model;

	/* initialize viewState */
	ViewStateref pgViewState = initGUIViewState();
	if (pgViewState == NULL){
		return;
	}
	gui->viewState = pgViewState;

	/* check if game is over and update the model accordingly */
	pgModel->isGameOver = checkAndupdateGameOverType(pgModel);


	/* create image surface for gui */
	char imgPath[] = "images/PlayGame.bmp";
	SDL_Surface * pgImage = SDL_LoadBMP(imgPath);
	if (pgImage == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	pgViewState->image = pgImage;

	/* create grid items array for the grid items images widgets */
	createGridItemsImgArr(pgViewState);
	if (isError)
		return;

	/* set the layout of play game gui */
	setThreePartLayout(pgViewState, pgModel->gameGridData);
	if (isError)
		return;

	/* create buttons array */
	Widget ** buttons = (Widget **)malloc(PG_NUM_BUTTONS*sizeof(Widget *));
	if (buttons == NULL){
		perrorPrint("malloc");
		return;
	}
	pgViewState->buttonsArr = buttons;

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
	if (isError)
		return;

	/* handle top panel presentation and select curr player if game is played */
	if (pgModel->isGameOver)
		setTopPanelGameOver(pgModel, pgViewState);
	else
		setPlayGame(pgViewState, pgModel);
	if (isError)
		return;

	/* enable side panel buttons if game is over */
	if (pgModel->isGameOver || pgModel->isGamePaused)
		enablePGSidePanelButtons(pgViewState);

	/* draw GUI according to UItree */
	drawGui(pgViewState->UITree);
}


/**** viewTranslateEvent (VTE) functions: ****/

/* simpleMenuVte is a function that is used as a VTE for the menus which have only
 * regular buttons (the menus that don't have value selection button */
void* simpleMenuVTE(void* viewState, SDL_Event* event, int numOfButtons){
	/* create the return event */
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	/* set type of return event to NO_EVENT as default */
	returnEvent->type = NO_EVENT;
	ViewStateref menuViewState = viewState;
	/* switch over event types */
	switch (event->type) {
		case (SDL_KEYUP): /* handle key up events */
			if (event->key.keysym.sym == SDLK_TAB){
				/* if tab was pressed, event is mark next button */
				returnEvent->type = MARK_NEXT_BUTTON;
			}
			if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_KP_ENTER){
				/* if return button pressed, even is select curr button */
				returnEvent->type = SELECT_CURR_BUTTON;
			}
			break;
		case (SDL_MOUSEBUTTONUP): /* handle mouse button up events */
			for (int i = 0; i< numOfButtons; i++){ /* go over each button and check if mouse clicked on it */
				Widget * currButton = menuViewState->buttonsArr[i];
				if (isClickEventOnButton(event, currButton, REGULAR_BUTTON)){ /* check if button was clicked*/
					returnEvent->type = SELECT_BUTTON_NUM; /* set event type to select button with
					 	 	 	 	 	 	 	 	 	 	  the number of the button*/
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

/* complexMenuVte is a function that is used as a VTE for the menus which have one value selection
 * button and two other regular buttons */
void* complexMenuVTE(void* viewState, SDL_Event* event){
	/* create the return event */
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	int numOfButtons = COMMON_MENU_NUM_BUTTONS;
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	ViewStateref menuViewState = viewState;
	/* set type of return event to NO_EVENT as default */
	returnEvent->type = NO_EVENT;
	/* switch over event types */
	switch (event->type) {
		case (SDL_KEYUP): /* handle key up events */
			/* if tab was pressed, event is mark next button */
			if (event->key.keysym.sym == SDLK_TAB){
				returnEvent->type = MARK_NEXT_BUTTON;
			}
			else if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_KP_ENTER){
				/* if return button pressed, event is select curr button */
				if (menuViewState->currButton != FIRST_BUTTON)
					returnEvent->type = SELECT_CURR_BUTTON;
			}
			else if (event->key.keysym.sym == SDLK_UP){
				if (menuViewState->currButton == FIRST_BUTTON)
					/* if the first button (value selection) is selected, event is increase value */
					returnEvent->type = INCREASE_VALUE;
			}
			else if (event->key.keysym.sym == SDLK_DOWN){
				if (menuViewState->currButton == FIRST_BUTTON)
					/* if the first button (value selection) is selected, event is decrease value */
					returnEvent->type = DECREASE_VALUE;
			}
			break;
		case (SDL_MOUSEBUTTONUP): /* handle mouse button up events */
			for (int i = 0; i< numOfButtons; i++){ /* go over each button and check if mouse clicked on it */
				Widget * currButton = menuViewState->buttonsArr[i];
				if (i == FIRST_BUTTON){
					/* for the value selection button first check if click is on arrow area or not */
					if(isClickEventOnButton(event, currButton, UP_ARROW_BUTTON)){
						returnEvent->type = INCREASE_VALUE; /* if up arrow area is clicked, event is increase value */
						break;
					}
					if(isClickEventOnButton(event, currButton, DOWN_ARROW_BUTTON)){
						returnEvent->type = DECREASE_VALUE;/* if down arrow area is clicked, event is decrease value */
						break;
					}
					if (isClickEventOnButton(event, currButton, REGULAR_BUTTON)){
						returnEvent->type = MARK_VALUES_BUTTON; /* if other parts are clicked event is mark values button */
						returnEvent->buttonNum = i;
						break;
					}
				}
				if (isClickEventOnButton(event, currButton, REGULAR_BUTTON)){
					/* for all other buttons (regular buttons) check if they were clicked */
					returnEvent->type = SELECT_BUTTON_NUM;
					returnEvent->buttonNum = i;
					break;
				}
			}
			break;
	}
	return returnEvent;
}

/* mainMenuVTE is the VTE function for the main menu gui
 * it calls simpleMenuVTE function */
void* mainMenuVTE(void* viewState, SDL_Event* event){
	logicalEventRef returnEvent = simpleMenuVTE(viewState, event, MAIN_MENU_NUM_BUTTONS);
	return returnEvent;
}

/* chooseAnimalVTE is the VTE function for the choose cat/mouse guis
 * it calls simpleMenuVTE function */
void* chooseAnimalVTE(void* viewState, SDL_Event* event){
	logicalEventRef returnEvent = simpleMenuVTE(viewState, event, COMMON_MENU_NUM_BUTTONS);
	return returnEvent;
}

/* worldBuilderVTE is the VTE function for the world builder gui */
void* worldBuilderVTE(void* viewState, SDL_Event* event){
	/* create the return event */
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	/* set type of return event to NO_EVENT as default */
	returnEvent->type = NO_EVENT;
	ViewStateref wbViewState = viewState;
	SDLKey key;
	switch (event->type) {
		case (SDL_KEYUP):/* handle key up events */
			key = event->key.keysym.sym;
			if (key == SDLK_s || key == SDLK_F1 || key == SDLK_ESCAPE ||
					key == SDLK_m || key == SDLK_c || key == SDLK_p || key == SDLK_w || key == SDLK_SPACE){
				/* handle case where a button key was pressed */
				returnEvent->type = SELECT_BUTTON_NUM;
				returnEvent->buttonNum = getWBButtonNum(key);
			}
			else
				/* check and handle arrow key events */
				handleThreePartLayoutArrowKey(key, returnEvent);
			break;
		case (SDL_MOUSEBUTTONUP): /* handle mouse button up events */
			handleThreePartLayoutMouseSelect(event, returnEvent, wbViewState->buttonsArr, WB_NUM_BUTTONS);
			break;
		default:
			returnEvent->type = NO_EVENT;
	}
	return returnEvent;
}

void* errMsgVTE(void* viewState, SDL_Event* event){
	/* create the return event */
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	/* set type of return event to NO_EVENT as default */
	returnEvent->type = NO_EVENT;
	ViewStateref menuViewState = viewState;
	Widget * currButton = menuViewState->buttonsArr[0]; /* there is only one button */
	switch (event->type) {
		case (SDL_KEYUP): /* handle key up events - for return click */
			if (event->key.keysym.sym == SDLK_RETURN || event->key.keysym.sym == SDLK_KP_ENTER){
				returnEvent->type = SELECT_CURR_BUTTON;
			}
			break;
		case (SDL_MOUSEBUTTONUP): /* handle mouse button up events on the back button */
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
	/* create the return event */
	logicalEventRef returnEvent = malloc(sizeof(logicalEvent));
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	/* set type of return event to NO_EVENT as default */
	returnEvent->type = NO_EVENT;
	ViewStateref pgViewState = viewState;
	SDLKey key;
	switch (event->type) {
		case (SDL_KEYUP):
			key = event->key.keysym.sym;
			if (key == SDLK_SPACE || key == SDLK_F1 || key == SDLK_F2 || key == SDLK_F3 || key == SDLK_F4 ||
					key == SDLK_ESCAPE){
				/* handle case where a button key was pressed */
				returnEvent->type = SELECT_BUTTON_NUM;
				returnEvent->buttonNum = getPGButtonNum(key);
			}
			else
				/* check and handle arrow key events */
				handleThreePartLayoutArrowKey(key, returnEvent);
			break;
		case (SDL_MOUSEBUTTONUP):/* handle mouse button up events */
			handleThreePartLayoutMouseSelect(event, returnEvent, pgViewState->buttonsArr, PG_NUM_BUTTONS);
			break;
		default:
			returnEvent->type = NO_EVENT;
	}
	return returnEvent;
}



/**** presenterHandleEvent functions ****/

/* generalMenuPHE is the PHE function used for all the menus of the program
 * (both simple and complex menus. it gets the menu model, view state, an array of states representing
 * the state the button number leads to, number of buttons in the menu, the stateId, the current button selected
 * the current value of the values button (if applicable), and the max value of the values button (if applicable) */
StateId generalMenuPHE(void* model, void* viewState, void* logicalEvent, StateId states[], int numOfButtons,
		StateId stateId, int* currButton, int* currValue, int maxValue){
	StateId returnStateId = stateId;
	if (logicalEvent == NULL || viewState == NULL || model == NULL)
		return returnStateId;
	/* get the logical event, viewState and menuModel */
	logicalEventRef menuEvent = logicalEvent;
	ViewStateref menuView = viewState;
	MenuDataRef menuModel = menuModel;
	/* switch over event types */
	switch(menuEvent->type){
		case(SELECT_CURR_BUTTON):
			returnStateId = states[*currButton]; /* set the correct return stateId according to the button and state arr*/
			menuView->currButton = *currButton; /* update the current button in the view */
			break;
		case(MARK_NEXT_BUTTON):
			changeSelectedButton(menuView->buttonsArr[*currButton],
					menuView->buttonsArr[(*currButton+1)%numOfButtons]); /* update the selected button in the view */
			*currButton = (*currButton + 1)%numOfButtons; /* update the button number */
			menuView->currButton = *currButton;/* update the current button in the view */
			break;
		case(SELECT_BUTTON_NUM):
			*currButton = menuEvent->buttonNum; /* get the number of button from the event */
			returnStateId = states[menuEvent->buttonNum]; /* set the correct return stateId according to the button and state arr*/
			menuView->currButton = *currButton; /* update the current button in the view */
			break;
		case(MARK_VALUES_BUTTON):
			changeSelectedButton(menuView->buttonsArr[*currButton],
					menuView->buttonsArr[FIRST_BUTTON]); /* update the selected button in the view */
			*currButton = FIRST_BUTTON; /* update the button number */
			menuView->currButton = *currButton; /* update the current button in the view */
			break;
		case(INCREASE_VALUE):
			increaseValuesButton(currValue, maxValue, menuView->buttonsArr[FIRST_BUTTON]); /* update the value */
			changeSelectedButton(menuView->buttonsArr[*currButton],
				menuView->buttonsArr[FIRST_BUTTON]); /* update the selected button in the view */
			*currButton = FIRST_BUTTON; /* update the button number */
			menuView->currButton = *currButton; /* update the current button in the view */
			break;
		case(DECREASE_VALUE):
			decreaseValuesButton(currValue, MIN_VALUE, menuView->buttonsArr[FIRST_BUTTON]); /* update the value */
			changeSelectedButton(menuView->buttonsArr[*currButton],
				menuView->buttonsArr[FIRST_BUTTON]); /* update the selected button in the view */
			*currButton = FIRST_BUTTON; /* update the button number */
			menuView->currButton = *currButton; /* update the current button in the view */
			break;
		case(NO_EVENT):
			break;
		default:
			break;
	}
	free(logicalEvent); /* free logical event */
	return returnStateId;
}

/* the phe function for main menu. calls generalMenuPHE */
StateId mainMenuPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = MAIN_MENU; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef mainMenuModel = model;
	/* create an array of return stateIds according to the buttons: */
	StateId mainMenuStates[MAIN_MENU_NUM_BUTTONS] = {CHOOSE_CAT, LOAD_GAME, WORLD_BUILDER, EDIT_GAME, QUIT};
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, mainMenuStates, MAIN_MENU_NUM_BUTTONS, returnStateId,
			&mainMenuModel->mainMenuButton, NULL, 0);
	if (returnStateId == WORLD_BUILDER) /* set edited world to 0 if we're going to world builder from main menu */
		mainMenuModel->editedWorld = 0;
	mainMenuModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for choose cat. Calls generalMenuPHE */
StateId chooseCatPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = CHOOSE_CAT; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef chooseCatModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId chooseCatStates[COMMON_MENU_NUM_BUTTONS] = {CHOOSE_MOUSE, CAT_SKILL, chooseCatModel->preChooseCat};
	if (chooseCatModel->preChooseCat == PLAY_GAME) /* set the first button to go to play game if we came from play game */
		chooseCatStates[0]=PLAY_GAME;
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, chooseCatStates, COMMON_MENU_NUM_BUTTONS,
			returnStateId, &chooseCatModel->chooseCatButton, NULL, 0);
	if (returnStateId == CHOOSE_MOUSE){ /* human was pressed */
		chooseCatModel->isCatHuman = 1;
		chooseCatModel->preChooseMouse = CHOOSE_CAT;
	}
	else if (returnStateId == PLAY_GAME && chooseCatModel->chooseCatButton == 0) /* human was pressed */
			chooseCatModel->isCatHuman = 1;
	else if (returnStateId == CAT_SKILL) /* update the current skill in a temp value */
		chooseCatModel->currValueTemp = chooseCatModel->catSkill;
	else if (returnStateId == LOAD_GAME) // back was pressed
		chooseCatModel->chooseCatButton = 0;
	chooseCatModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for choose mouse. Calls generalMenuPHE */
StateId chooseMousePHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = CHOOSE_MOUSE; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef chooseMouseModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId chooseMouseStates[COMMON_MENU_NUM_BUTTONS] = {PLAY_GAME, MOUSE_SKILL, chooseMouseModel->preChooseMouse};
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, chooseMouseStates, COMMON_MENU_NUM_BUTTONS,
			returnStateId, &chooseMouseModel->chooseMouseButton, NULL, 0);
	if (returnStateId == PLAY_GAME && chooseMouseModel->chooseMouseButton == 0) /* human was pressed */
		chooseMouseModel->isMouseHuman = 1;
	else if (returnStateId == MOUSE_SKILL) /* update the current skill in a temp value */
		chooseMouseModel->currValueTemp = chooseMouseModel->mouseSkill;
	else if (returnStateId == CHOOSE_CAT || returnStateId == CAT_SKILL) // back was pressed
		chooseMouseModel->chooseMouseButton = 0;
	chooseMouseModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for cat skill. Calls generalMenuPHE */
StateId catSkillPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = CAT_SKILL; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef catSkillModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId catSkillStates[COMMON_MENU_NUM_BUTTONS] = {CAT_SKILL, CHOOSE_MOUSE, CHOOSE_CAT};
	if (catSkillModel->preChooseCat == PLAY_GAME)
		catSkillStates[1]=PLAY_GAME; /* set the second button to go to play game if we came from play game */
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, catSkillStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &catSkillModel->catSkillButton, &catSkillModel->currValueTemp, MAX_SKILL_VALUE);
	if (returnStateId == PLAY_GAME || returnStateId == CHOOSE_MOUSE){ /* Done button was pressed */
		catSkillModel->isCatHuman = 0; /* cat us a machine */
		catSkillModel->catSkill = catSkillModel->currValueTemp; /* set cat skill once done is pressed */
		if (returnStateId == CHOOSE_MOUSE){ /* update pre choose mouse in model */
			catSkillModel->preChooseMouse = CAT_SKILL;
		}
	}
	if (returnStateId == CHOOSE_CAT) /* back was pressed and we go back to choose cat */
		catSkillModel->catSkillButton = 0;
	catSkillModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for mouse skill. Calls generalMenuPHE */
StateId mouseSkillPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = MOUSE_SKILL; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef mouseSkillModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId mouseSkillStates[COMMON_MENU_NUM_BUTTONS] = {MOUSE_SKILL, PLAY_GAME, CHOOSE_MOUSE};
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, mouseSkillStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &mouseSkillModel->mouseSkillButton, &mouseSkillModel->currValueTemp, MAX_SKILL_VALUE);
	if (returnStateId == PLAY_GAME){ /* Done button was pressed */
		mouseSkillModel->isMouseHuman = 0; /* set mouse as machine */
		mouseSkillModel->mouseSkill = mouseSkillModel->currValueTemp;
	}
	else if (returnStateId == CHOOSE_MOUSE){ /* back was pressed */
		mouseSkillModel->mouseSkillButton = 0; /* set button to 0 for when we get back */
	}
	mouseSkillModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for load game menu. calls generalMenuPHE */
StateId loadGamePHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = LOAD_GAME;/* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef loadGameModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId loadGameStates[COMMON_MENU_NUM_BUTTONS] = {LOAD_GAME, CHOOSE_CAT, MAIN_MENU};
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, loadGameStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &loadGameModel->loadGameButton, &loadGameModel->loadGameWorld, MAX_WORLD);
	if (returnStateId == CHOOSE_CAT){ /* done was pressed */
		loadGameModel->preChooseCat = LOAD_GAME; /* update the pre choose cat stateId */
		loadGameModel->loadFromFile = 1; /* update load from file to 1 */
	}
	loadGameModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for edit game menu. calls generalMenuPHE */
StateId editGamePHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = EDIT_GAME; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef editGameModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId editGameStates[COMMON_MENU_NUM_BUTTONS] = {EDIT_GAME, WORLD_BUILDER, MAIN_MENU};
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, editGameStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &editGameModel->editGameButton, &editGameModel->editedWorld, MAX_WORLD);
	if (returnStateId == WORLD_BUILDER){ /* done was pressed */
		editGameModel->preWorldBuilder = EDIT_GAME; /* update pre world builder */
		editGameModel->loadFromFile = 1; /* set load from file to 1 */
	}
	editGameModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for save world menu. calls generalMenuPHE */
StateId saveWorldPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = SAVE_WORLD; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef saveWorldModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId saveWorldStates[COMMON_MENU_NUM_BUTTONS] = {SAVE_WORLD, WORLD_BUILDER, WORLD_BUILDER};
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, saveWorldStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &saveWorldModel->saveWorldButton, &saveWorldModel->currValueTemp, MAX_WORLD);
	if (returnStateId == WORLD_BUILDER){ //done or back were pressed
		saveWorldModel->preWorldBuilder = SAVE_WORLD;
		if (saveWorldModel->saveWorldButton == 1){ /* done was pressed */
			saveWorldModel->loadFromFile = 1; /* tell wb to use file to load the grid */
			saveWorldModel->editedWorld = saveWorldModel->currValueTemp; /* set edited world to curr value */
			/* save the grid data to the correct file and free it */
			saveGameDataToFile(saveWorldModel->editedWorld, saveWorldModel->isCatFirst, saveWorldModel->gameGridData);
			freeGridData(saveWorldModel->gameGridData); /* free grid data */
			saveWorldModel->gameGridData = NULL;
		}
		else if (saveWorldModel->saveWorldButton == 2) //back was pressed
			saveWorldModel->loadFromFile = 0; /* tell wb to use char ** to load the grid */
	}
	saveWorldModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for error message. calls generalMenuPHE */
StateId errMsgPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = ERR_MSG; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef errMsgModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId errMsgStates[MAIN_MENU_NUM_BUTTONS] = {WORLD_BUILDER};
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, errMsgStates, ERR_MSG_NUM_BUTTONS,
			returnStateId, &errMsgModel->errMsgButton, NULL, 0);
	if (returnStateId == WORLD_BUILDER) /* back button pressed */
		errMsgModel->preWorldBuilder = ERR_MSG;
	errMsgModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for the world builder */
StateId worldBuilderPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = WORLD_BUILDER; /* default return stateId */
	if (logicalEvent == NULL || viewState == NULL || model == NULL)
		return returnStateId;
	logicalEventRef wbEvent = logicalEvent;
	ViewStateref wbView = viewState;
	WBDataRef wbModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId states[WB_NUM_BUTTONS] = {SAVE_WORLD, MAIN_MENU, QUIT, WORLD_BUILDER, WORLD_BUILDER, WORLD_BUILDER,
			WORLD_BUILDER,WORLD_BUILDER};
	/* switch over logical events type */
	switch(wbEvent->type){
		case(SELECT_BUTTON_NUM):
			returnStateId = states[wbEvent->buttonNum];
			if(returnStateId == SAVE_WORLD && isGridInvalid(wbModel)) /* save world was pressed and grid is invalid */
				returnStateId = ERR_MSG;
			else if (returnStateId == WORLD_BUILDER){ /* place mouse/cat/cheese/wall/empty space was pressed */
				putGridItemInPos(wbModel, wbView->gridPanel, wbView->gridItemsImgArr, wbModel->currPos, wbEvent->buttonNum);
			}
			break;
		case(SELECT_SQUARE): /* a grid square was pressed */
			changeSelectedGridSquare(wbView->gridPanel, wbView->gridItemsImgArr,&wbModel->currPos, wbEvent->gridPos);
			break;
		case(GO_UP): /* up arrow pressed */
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImgArr, &wbModel->currPos, UP);
			break;
		case(GO_DOWN): /* down arrow pressed */
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImgArr, &wbModel->currPos, DOWN);
			break;
		case(GO_RIGHT): /* right arrow pressed */
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImgArr, &wbModel->currPos, RIGHT);
			break;
		case(GO_LEFT): /* left arrow pressed */
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImgArr, &wbModel->currPos, LEFT);
			break;
		case(NO_EVENT):
			break;
		default:
			break;

	}
	free(logicalEvent);
	wbModel->returnStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}

/* The phe function for the play game */
StateId playGamePHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = PLAY_GAME; /* default return stateId */
	if (logicalEvent == NULL || viewState == NULL || model == NULL)
		return returnStateId;
	logicalEventRef pgEvent = logicalEvent;
	ViewStateref pgViewState = viewState;
	PGDataRef pgModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId states[PG_NUM_BUTTONS] = {PLAY_GAME, CHOOSE_MOUSE, CHOOSE_CAT, PLAY_GAME, MAIN_MENU, QUIT};
	/* switch over logical events type */
	switch(pgEvent->type){
		case(SELECT_BUTTON_NUM):
			if (pgModel->isGamePaused || pgModel->isGameOver){
				returnStateId = states[pgEvent->buttonNum];
				if (pgEvent->buttonNum == 3){ /* restart game was pressed */
					restartGame(pgViewState, pgModel);
				}
				if (pgEvent->buttonNum == 0 && pgModel->isGamePaused){ /* game is paused and space was pressed */
					resumeGame(pgViewState, pgModel);
				}
			}
			else if (pgEvent->buttonNum == 0){ /* game is running and space was pressed */
				pauseGame(pgViewState, pgModel);
			}
			break;
		case(SELECT_SQUARE): /* a square was selected */
			if (isCurrPlayerHuman(pgModel)) /* if current player is human make the move if possible */
				makeGameMoveIfLegal(pgViewState, pgModel, pgEvent->gridPos);
			break;
		case(GO_UP): /* arrow button pressed - make move if possible */
			if (isCurrPlayerHuman(pgModel))
				makeGameMoveByArrowIfLegal(pgViewState, pgModel, UP);
			break;
		case(GO_DOWN): /* arrow button pressed - make move if possible */
			if (isCurrPlayerHuman(pgModel))
				makeGameMoveByArrowIfLegal(pgViewState, pgModel, DOWN);
			break;
		case(GO_RIGHT): /* arrow button pressed - make move if possible */
			if (isCurrPlayerHuman(pgModel))
				makeGameMoveByArrowIfLegal(pgViewState, pgModel, RIGHT);
			break;
		case(GO_LEFT): /* arrow button pressed - make move if possible */
			if (isCurrPlayerHuman(pgModel))
				makeGameMoveByArrowIfLegal(pgViewState, pgModel, LEFT);
			break;
		case(NO_EVENT):
			break;
		default:
			break;

	}
	if (returnStateId == CHOOSE_CAT || returnStateId == CHOOSE_MOUSE)
		pgModel->loadFromFile = 0; /* set the load from file value to 0, so that when we get back the same grid will be shown */
	free(logicalEvent);
	pgModel->returnStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}


/**** stop functions: ****/

/* stop function for all menus and err msg */
void* stopMenu(GUIref gui){ /* maybe this will be a general stop function */
	/* get the model and view state */
	MenuDataRef returnData = gui->model; /* return data will be menu data */
	ViewStateref guiViewState = gui->viewState;
	/* reset the model and view to NULL */
	gui->model = NULL;
	gui->viewState = NULL;

	freeViewState(guiViewState); /* free viewstate */

	if (isError || returnData->retStateId == QUIT){ /* if we are in a final situation, free the return data */
		freeMenuData(returnData);
		return NULL;
	}
	return returnData;
}

/* stop function for world builder */
void* stopWorldBuilder(GUI * gui){
	/* get the model and view state */
	ViewStateref guiViewState = gui->viewState;
	WBDataRef wbData = gui->model;
	/* reset the model and view to NULL */
	gui->model = NULL;
	gui->viewState = NULL;

	freeViewState(guiViewState); /* free viewstate */

	/* if we are in a final situation, or we go back to Main Menu, free the wb data */
	if (isError || wbData->returnStateId == QUIT || wbData->returnStateId == MAIN_MENU){
		freeWBData(wbData, 1); /* free wbData with grid free */
		return NULL;
	}

	/* initialize return data as menu data default values */
	MenuDataRef returnData = initMenuDataToDefault();
	if (isError){
		freeWBData(wbData, 1); /* free wbData with grid free */
		return NULL;
	}
	/* Assign values in return Data for save world/error message: */
	returnData->gameGridData = wbData->gameGridData;
	returnData->editedWorld = wbData->editedWorld;
	returnData->isCatFirst = wbData->isCatFirst;
	returnData-> numTurns = wbData->numTurns;
	returnData->wbCurrPos = wbData->currPos;
	returnData->currValueTemp = wbData->editedWorld ? wbData->editedWorld : MIN_VALUE;
	returnData->missingItems = wbData->missingItems;

	freeWBData(wbData, 0); /* free wbData without grid free */

	return returnData;
}


/* stop function for play game */
void* stopPlayGame(GUI * gui){
	/* get the model and view state */
	ViewStateref guiViewState = gui->viewState;
	PGDataRef pgData = gui->model;
	/* reset the model and view to NULL */
	gui->model = NULL;
	gui->viewState = NULL;

	freeViewState(guiViewState); /* free viewstate */

	/* if we are in a final situation, or we go back to Main Menu, free the PG data */
	if (isError || pgData->returnStateId == QUIT || pgData->returnStateId == MAIN_MENU){
		freePGData(pgData, 1); /* free pgData with grid free */
		return NULL;
	}

	/* initialize return data as menu data default values */
	MenuDataRef returnData = initMenuDataToDefault();
	if (isError){
		freePGData(pgData, 1); /* free pgData with grid free */
		return NULL;
	}
	/*Assign values in return Data for choose cat & choose mouse: */
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
	returnData->chooseCatButton = pgData->isCatHuman ? 0 : 1;
	returnData->chooseMouseButton = pgData->isMouseHuman ? 0 : 1;
	returnData->isGamePaused = pgData->isGamePaused;
	returnData->loadFromFile = pgData->loadFromFile;

	freePGData(pgData, 0); /* free pgData without grid free */

	return returnData;
}
