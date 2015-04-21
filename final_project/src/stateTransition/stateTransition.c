#include "stateTransition.h"

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
	menuViewState->menuButtons = buttons;

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
	setButtonSelected(menuViewState->menuButtons[selectedButton]);

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
			currentValue = data->editedWorld ? data->editedWorld: 1; /* if edited world is 0 this will show world 1 */
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
	wbViewState->menuButtons = buttons;

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
	/* draw GUI according to UItree */
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
	errViewState->menuButtons = buttons;

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
	setButtonSelected(errViewState->menuButtons[0]);
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
	pgViewState->menuButtons = buttons;

	/* create labels array */
	Widget ** labels = (Widget **)malloc(PG_NUM_LABELS*sizeof(Widget *));
	if (labels == NULL){
		perrorPrint("malloc");
		return;
	}
	pgViewState->labelArr = labels;

	/* add buttons to side panel: */
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
	if (isError)
		return;

	/* draw GUI according to UItree */
	drawGui(pgViewState->UITree);
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
	if (pgData->returnStateId == CAT_SKILL)
		returnData->currValueTemp = pgData->catSkill;
	if (pgData->returnStateId == MOUSE_SKILL)
		returnData->currValueTemp = pgData->mouseSkill;

	freePGData(pgData, 0); /* free pgData without grid free */

	return returnData;
}

