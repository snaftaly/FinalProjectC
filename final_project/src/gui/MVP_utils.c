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
			returnGUI.start = startChooseCat;
			returnGUI.viewTranslateEvent = chooseAnimalVTE;
			returnGUI.presenterHandleEvent = chooseCatPHE;
			returnGUI.stop = menuStop;
			break;
		case(CHOOSE_MOUSE):
			returnGUI.start = startChooseMouse;
			returnGUI.viewTranslateEvent = chooseAnimalVTE;
			returnGUI.presenterHandleEvent = chooseMousePHE;
			returnGUI.stop = menuStop;
			break;

		/*
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
	return viewState;
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
					returnEvent->type = MARK_AND_SELECT_BUTTON;
					returnEvent->buttonNum = i;
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
					returnEvent->type = MARK_AND_SELECT_BUTTON;
					returnEvent->buttonNum = i;
					break;
				}
			}
			break;
	}
	return returnEvent;
}


StateId generalMenuPHE(void* model, void* viewState, void* logicalEvent, StateId states[], int numOfButtons,
		StateId stateId, int* currButton, int* currValue, int maxValue){
	StateId returnStateId = stateId;
	if (logicalEvent == NULL || viewState == NULL)
		return returnStateId;
	logicalEventRef menuEvent = logicalEvent;
	GameDataRef menuModel = model;
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
		case(MARK_AND_SELECT_BUTTON):
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
	}
	free(logicalEvent);
	return returnStateId;

}


void* menuStop(GUIref gui){ /* maybe this will be a general stop function */
	GameDataRef returnData = gui->model;
	ViewStateref guiViewState = gui->viewState;
	if (guiViewState != NULL){
		/* we need to write a function for that */
		if (guiViewState->menuButtons != NULL)
			free(guiViewState->menuButtons);
		if (guiViewState->image != NULL)
			SDL_FreeSurface(guiViewState->image);
	}
	if (isError || isQuit){
		free(returnData); /* we need to write a function for that! */
		return NULL;
	}
	return returnData;
}

void drawMenuGui(GUIref gui){
	ViewStateref menuViewState = gui->viewState;
	treeDFS(menuViewState->UITree, calcAbsWidgetXY, addChildWidgetsToParent);
	Widget * window = menuViewState->UITree->data;
	if (SDL_Flip(window->surface) != 0) {
		sdlErrorPrint("failed to flip buffer");
		return;
}

/* maybe we don't need to pass initdata !!!!!!!!!!!!! */
void startGeneralMenu(GUIref gui, void * initData, , char * imgPath, int titleImgX, int titleImgY, int titleWidth,
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
	Widget *label = create_image(MENU_TITLE_X, MENU_TITLE_Y, titleWidth, MENU_TITLE_H,
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
	int button_x = calcMenuButtonX(titleWidth), button_y = calcMenuButtonY(), isSelected_x = BUTTON_W, isSelected_y = 0, isNselected_x = 0, isNselected_y=0;
	for (int i = 0; i < numButtons; i++){
		buttons[i] = create_button(button_x,button_y, BUTTON_W, BUTTON_H,
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
			isSelected_y += firstButtonNumOpts*BUTTON_H;
			isNselected_y += firstButtonNumOpts*BUTTON_H;
		}
		else{
			isSelected_y += BUTTON_H;
			isNselected_y += BUTTON_H;
		}
		button_y += BUTTON_H+MENU_BUTTON_GAP;
	}

	/* update the view buttons */
	if(firstButtonNumOpts > 1) /* update the values button */
		setValuesButtonFromInit(value, buttons[0]);
	menuViewState->currButton = selectedButton;
	setButtonSelected(menuViewState->menuButtons[selectedButton]);
	/* draw GUI according to UItree */
	drawMenuGui(gui);
}

void initializeMenuModel(GUIref gui, void* initData){
	if (initData == NULL){ /* memory leak!!! */
		gui->model = initGameDataToDefault(); /* write this function */
	}
	else{
		gui->model = initData;
	}
}

/* Main Menu specific MVP functions */
void startMainMenu(GUIref gui, void* initData){
	initializeMenuModel(gui, initData);
	if(isError)
		return;
	char imgPath[] = "images/MainMenu_temp.bmp";
	GameDataRef data = gui->model;
	int currentButton = data->mainMenuButton;
	/* start the main menu gui */
	startGeneralMenu(gui, initData, imgPath,
			BUTTON_W*2, 0, 200, MAIN_MENU_NUM_BUTTONS, currentButton, 1, 0);
}


void startChooseAnimal(GUIref gui, void* initData, StateId state){
	initializeMenuModel(gui, initData);
	if(isError)
		return;
	char imgPath[] = "images/animalSelectionButtons.bmp";
	GameDataRef data = gui->model;
	int currentButton;
	switch(state){
		case(CHOOSE_CAT):
			currentButton = data->chooseCatButton;
			break;
		case(CHOOSE_MOUSE):
			currentButton = data->chooseMouseButton;
			break;
	}
	startGeneralMenu(gui, initData, imgPath,
				BUTTON_W*2, 0, 200, COMMON_MENU_NUM_BUTTONS, currentButton, 1, 0);
}

void startChooseCat(GUIref gui, void* initData){
	startChooseAnimal(gui, initData, IS_CAT);
}

void startChooseMouse(GUIref gui, void* initData){
	startChooseAnimal(gui, initData, IS_MOUSE);
}

void startAnimalSkill(GUIref gui, void* initData, StateId state){
	initializeMenuModel(gui, initData);
	if(isError)
		return;
	char imgPath[] = "images/animalSkillButtons.bmp";
	GameDataRef data = gui->model;
	int currentButton, currentValue, titleImgY;
	switch(state){
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
	}
	startGeneralMenu(gui, initData, imgPath,
				BUTTON_W*2, titleImgY, 200, COMMON_MENU_NUM_BUTTONS,currentButton, 1, MAX_SKILL_VALUE);
}

void startCatSkill(GUIref gui, void* initData){
	startChooseAnimal(gui, initData, gui->stateId);
}

void startMouseSkill(GUIref gui, void* initData){
	startChooseAnimal(gui, initData, gui->stateId);
}

void startWorldMenu(GUIref gui, void* initData, StateId state){
	initializeMenuModel(gui, initData);
	if(isError)
		return;
	char imgPath[] = "images/worldMenuButtons.bmp";
	GameDataRef data = gui->model;
	int currentButton, currentValue, titleImgY;
	switch(state){
		case(EDIT_GAME):
			currentButton = data->catSkillButton;
			currentValue = data->catSkill;
			titleImgY = 0;
			break;
		case(LOAD_GAME):
			currentButton = data->catSkillButton;
			currentValue = data->catSkill;
			titleImgY = MENU_TITLE_H;
			break;
		case(SAVE_WORLD):
			currentButton = data->catSkillButton;
			currentValue = data->catSkill;
			titleImgY = 2*MENU_TITLE_H;
			break;
	}
	startGeneralMenu(gui, initData, imgPath,
				BUTTON_W*2, titleImgY, 200, COMMON_MENU_NUM_BUTTONS,currentButton, 1, MAX_WORLD);
}

void startEditGame(GUIref gui, void* initData){
	startWorldMenu(gui, initData, gui->stateId);
}

void startLoadGame(GUIref gui, void* initData){
	startWorldMenu(gui, initData, gui->stateId);
}

void startSaveWorld(GUIref gui, void* initData){
	startWorldMenu(gui, initData, gui->stateId);
}

void* mainMenuVTE(void* viewState, SDL_Event* event){
	logicalEventRef returnEvent = simpleMenuVTE(viewState, event, MAIN_MENU_NUM_BUTTONS);
	return returnEvent;
}

StateId mainMenuPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = MAIN_MENU;
	if (model == NULL)
		return returnStateId;
	GameDataRef mainMenuModel = model;
	StateId mainMenuStates[MAIN_MENU_NUM_BUTTONS] = {CHOOSE_CAT, LOAD_GAME, WORLD_BUILDER, EDIT_GAME, QUIT};
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, mainMenuStates, MAIN_MENU_NUM_BUTTONS, returnStateId,
			&mainMenuModel->mainMenuButton);
	if (returnStateId == CHOOSE_CAT)
		mainMenuModel->preChooseCat = MAIN_MENU;
	return returnStateId;
}

StateId catSkillPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = CAT_SKILL;
	if (model == NULL)
		return returnStateId;
	GameDataRef catSkillModel = model;
	StateId catSkillStates[COMMON_MENU_NUM_BUTTONS] = {CAT_SKILL, CHOOSE_MOUSE, CHOOSE_CAT};
	if (catSkillModel->preChooseCat == PLAY_GAME){
		catSkillStates[1]=PLAY_GAME;
	}
	returnStateId = generalMenuPHE(model, ViewState, logicalEvent, catSkillStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &catSkillModel->catSkillButton, &catSkillModel->catSkill, MAX_SKILL_VALUE);
	if (returnStateId == CHOOSE_MOUSE)
		catSkillModel->preChooseMouse = CAT_SKILL;
	return returnStateId;
}

StateId mouseSkillPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = MOUSE_SKILL;
	if (model == NULL)
		return returnStateId;
	GameDataRef mouseSkillModel = model;
	StateId mouseSkillStates[COMMON_MENU_NUM_BUTTONS] = {MOUSE_SKILL, PLAY_GAME, CHOOSE_MOUSE};
	returnStateId = generalMenuPHE(model, ViewState, logicalEvent, mouseSkillStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &mouseSkillModel->mouseSkillButton, &mouseSkillModel->mouseSkill, MAX_SKILL_VALUE);
	return returnStateId;
}

StateId loadGamePHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = LOAD_GAME;
	if (model == NULL)
		return returnStateId;
	GameDataRef loadGameModel = model;
	StateId loadGameStates[COMMON_MENU_NUM_BUTTONS] = {LOAD_GAME, CHOOSE_CAT, MAIN_MENU};
	returnStateId = generalMenuPHE(model, ViewState, logicalEvent, loadGameStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &loadGameModel->loadGameButton, &loadGameModel->loadGameWorld, MAX_WORLD);
	if (returnStateId == CHOOSE_CAT)
		loadGameModel->preChooseCat = LOAD_GAME;
	return returnStateId;
}

StateId editGamePHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = EDIT_GAME;
	if (model == NULL)
		return returnStateId;
	GameDataRef editGameModel = model;
	StateId editGameStates[COMMON_MENU_NUM_BUTTONS] = {EDIT_GAME, WORLD_BUILDER, MAIN_MENU};
	returnStateId = generalMenuPHE(model, ViewState, logicalEvent, editGameStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &editGameModel->editGameButton, &editGameModel->editedWorld, MAX_WORLD);
	return returnStateId;
}

StateId saveWorldPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = SAVE_WORLD;
	if (model == NULL)
		return returnStateId;
	GameDataRef saveWorldModel = model;
	StateId saveWorldStates[COMMON_MENU_NUM_BUTTONS] = {SAVE_WORLD, WORLD_BUILDER, WORLD_BUILDER};
	returnStateId = generalMenuPHE(model, ViewState, logicalEvent, saveWorldStates, COMMON_MENU_NUM_BUTTONS,
		returnStateId, &saveWorldModel->saveWorldButton, &saveWorldModel->saveOnWorld, MAX_WORLD);
	if (saveWorldModel->saveWorldButton == 1){
		/*update world file*/
	}
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
	GameDataRef chooseCatModel = model;
	StateId chooseCatStates[COMMON_MENU_NUM_BUTTONS] = {CHOOSE_MOUSE, CAT_SKILL, chooseCatModel->preChooseCat};
	if (chooseCatModel->preChooseCat == PLAY_GAME){
		chooseCatStates[0]=PLAY_GAME;
	}
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, chooseCatStates, COMMON_MENU_NUM_BUTTONS,
			returnStateId, &chooseCatModel->chooseCatButton);
	if (returnStateId == CHOOSE_MOUSE)
		chooseCatModel->preChooseMouse = CHOOSE_CAT;
	return returnStateId;
}

StateId chooseMousePHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = CHOOSE_MOUSE;
	if (model == NULL)
		return returnStateId;
	GameDataRef chooseMouseModel = model;
	StateId chooseMouseStates[COMMON_MENU_NUM_BUTTONS] = {PLAY_GAME, MOUSE_SKILL, chooseMouseModel->preChooseMouse};
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, chooseMouseStates, COMMON_MENU_NUM_BUTTONS,
			returnStateId, &chooseMouseModel->chooseMouseButton);
	return returnStateId;
}


/* from start main Menu
ViewStateref mainMenuView = initializeGUIViewState();
if (mainMenuView == NULL){
	return;
}
gui->viewState = mainMenuView;

SDL_Surface *mainMenuImages = SDL_LoadBMP("images/MainMenu_temp.bmp");
mainMenuView->image = mainMenuImages;
if (mainMenuImages == NULL){
	sdlErrorPrint("failed to load image");
	return;
}
Widget ** buttons = (Widget **)malloc(MAIN_MENU_NUM_BUTTONS*sizeof(Widget *));
mainMenuView->menuButtons = buttons;
if (buttons == NULL){
	perrorPrint("malloc");
	return;
}
Widget *win = create_window(WIN_W,WIN_H, 0, 0, 0);
if (win == NULL){
	return;
}
ListRef win_node = newList(win);
mainMenuView->UITree = win_node;
if (win_node == NULL){
	freeWidget(win);
	return;
}
Widget *panel = create_panel(275,225,250,350,220,230,240);
if (panel == NULL){
	return;
}
ListRef panel_node = addChildNode(win_node, panel);
if (panel_node == NULL){
	freeWidget(panel);
	return;
}
Widget *label = create_image(15,30,220, 40,mainMenuImages,0,175);
if (label == NULL){
	return;
}
ListRef label_node = addChildNode(panel_node, label);
if (label_node == NULL){
	freeWidget(label);
	return;
}
 Add buttons to buttons array and to UI tree
Sint16 button_x = 50, button_y = 100, isSelected_x = BUTTON_W, isSelected_y = 0, isNselected_x = 0, isNselected_y=0;
for (int i = 0; i < MAIN_MENU_NUM_BUTTONS; i++){
	buttons[i] = create_button(button_x,button_y, BUTTON_W, BUTTON_H,
					mainMenuImages, isSelected_x, isSelected_y, isNselected_x, isNselected_y, 0);

	if (buttons[i] == NULL){
		return;
	}
	ListRef newButtonNode = addChildNode(panel_node, buttons[i]);
	if (newButtonNode == NULL){
		freeWidget(buttons[i]);
		return;
	}
	button_y += BUTTON_H+15;
	isSelected_y += BUTTON_H;
	isNselected_y += BUTTON_H;
}
*/

/*
 * from start animal menu
 create image surface
animalSelectView->image = animalSelectImage;
if (animalSelectImage == NULL){
	sdlErrorPrint("failed to load image");
	return;
}
/* create buttons array
Widget ** buttons = (Widget **)malloc(COMMON_MENU_NUM_BUTTONS*sizeof(Widget *));
animalSelectView->menuButtons = buttons;
if (buttons == NULL){
	perrorPrint("malloc");
	return;
}
/* create the UItree
Widget *win = create_window(WIN_W,WIN_H, 0, 0, 0);
if (win == NULL){
	return;
}
ListRef win_node = newList(win);
animalSelectView->UITree = win_node;
if (win_node == NULL){
	freeWidget(win);
	return;
}
Widget *panel = create_panel(30,50,300,600,0,0,0);
if (panel == NULL){
	return;
}
ListRef panel_node = addChildNode(win_node, panel);
if (panel_node == NULL){
	freeWidget(panel);
	return;
}
int labelX;
int labelY;
if (animal == IS_CAT){
	labelX = 90;
	labelY = 90;
}
else {
	labelX = 90;
	labelY = 100;
}
Widget *label = create_image(30,50,300,600, animalSelectImage, labelX, labelY);
if (label == NULL){
	return;
}
ListRef label_node = addChildNode(panel_node, label);
if (label_node == NULL){
	freeWidget(label);
	return;
}
/* Add buttons to buttons array and to UI tree
int button_x = 20, button_y = 40, isSelected_x = 90, isSelected_y = 100, isNselected_x = 90, isNselected_y=120;
for (int i = 0; i < COMMON_MENU_NUM_BUTTONS; i++){
	buttons[i] = create_button(button_x,button_y, 100, 100,
			animalSelectImage, isSelected_x, isSelected_y, isNselected_x, isNselected_y, 0);
	if (buttons[i] == NULL){
		return;
	}
	ListRef newButtonNode = addChildNode(panel_node, buttons[i]);
	if (newButtonNode == NULL){
		freeWidget(buttons[i]);
		return;
	}
	button_y +=40;
	isSelected_y +=30;
	isNselected_y +=30;
}
if (initData == NULL){
	gui->model = initGameDataToDefault(); /* write this function
}
else{
	gui->model = initData;
} */
