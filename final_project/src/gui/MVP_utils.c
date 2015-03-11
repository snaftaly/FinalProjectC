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
		freeTree(guiViewState->UITree, freeWidget);
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



/* Main Menu specific MVP functions */
void startMainMenu(GUIref gui, void* initData){
	/* allocate memory for create viewState */
	ViewStateref mainMenuView = initializeGUIViewState();
	if (mainMenuView == NULL){
		return;
	}
	gui->viewState = mainMenuView;

	/* create image surface */
	SDL_Surface *mainMenuImages = SDL_LoadBMP("images/MainMenu_temp.bmp");
	mainMenuView->image = mainMenuImages;
	if (mainMenuImages == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	/* create buttons array */
	Widget ** buttons = (Widget **)malloc(MAIN_MENU_NUM_BUTTONS*sizeof(Widget *));
	mainMenuView->menuButtons = buttons;
	if (buttons == NULL){
		perrorPrint("malloc");
		return;
	}
	/* create the UItree */
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
	/* Add buttons to buttons array and to UI tree */
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
	if (initData == NULL){ /* memmory leak!!! */
		gui->model = initGameDataToDefault(); /* write this function */
	}
	else{
		gui->model = initData;
	}
	GameDataRef data = gui->model;
	mainMenuView->currButton = data->mainMenuButton;
	setButtonSelected(buttons[data->mainMenuButton]);

	/* draw GUI according to UItree */
	treeDFS(win_node, calcAbsWidgetXY, addChildWidgetsToParent);
	if (SDL_Flip(win->surface) != 0) {
		sdlErrorPrint("failed to flip buffer");
		return;
	}

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
void startChooseAnimal(GUIref gui, void* initData, int animal){
	/* allocate memory for create viewState */
	ViewStateref animalSelectView = initializeGUIViewState();
	if (animalSelectView == NULL){
		return;
	}
	gui->viewState = animalSelectView;
	/* create image surface */
	SDL_Surface *animalSelectImage = SDL_LoadBMP("../../images/animalSelectionButtons.bmp");
	animalSelectView->image = animalSelectImage;
	if (animalSelectImage == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	/* create buttons array */
	Widget ** buttons = (Widget **)malloc(COMMON_MENU_NUM_BUTTONS*sizeof(Widget *));
	animalSelectView->menuButtons = buttons;
	if (buttons == NULL){
		perrorPrint("malloc");
		return;
	}
	/* create the UItree */
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
	/* Add buttons to buttons array and to UI tree */
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
		gui->model = initGameDataToDefault(); /* write this function */
	}
	else{
		gui->model = initData;
	}
	GameDataRef data = gui->model;
	if (animal == IS_CAT){
		setButtonSelected(buttons[data->chooseCatButton]);
	}
	else{ /* animal == IS_MOUSE */
		setButtonSelected(buttons[data->chooseMouseButton]);
	}
	/* draw GUI according to UItree */
	treeDFS(win_node, calcAbsWidgetXY, addChildWidgetsToParent);
	if (SDL_Flip(win->surface) != 0) {
		sdlErrorPrint("failed to flip buffer");
		return;
	}
}


void startChooseCat(GUIref gui, void* initData){
	startChooseAnimal(gui, initData, IS_CAT);
}

void startChooseMouse(GUIref gui, void* initData){
	startChooseAnimal(gui, initData, IS_MOUSE);
}


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

/*StateId catSelectionPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = CHOOSE_CAT;
	if (logicalEvent == NULL || viewState == NULL || model == NULL)
		return returnStateId;
	logicalEventRef catSelectEvent = logicalEvent;
	GameDataRef catSelectModel = model;
	ViewStateref catSelectView = viewState;
	StateId catSelectStates[COMMON_MENU_NUM_BUTTONS] = {CHOOSE_MOUSE, CAT_SKILL, catSelectModel->preChooseCat};
	int currButton = catSelectModel->chooseCatButton;
	switch(catSelectEvent->type){
		case(SELECT_CURR_BUTTON):
			returnStateId = catSelectStates[currButton];
			if (returnStateId == CHOOSE_MOUSE)
				catSelectModel->preChooseMouse = CHOOSE_CAT;
			break;
		case(MARK_NEXT_BUTTON):
			changeSelectedButton(catSelectView->menuButtons[currButton],
					catSelectView->menuButtons[(currButton+1)%COMMON_MENU_NUM_BUTTONS]);
			catSelectModel->chooseCatButton = (currButton + 1)%COMMON_MENU_NUM_BUTTONS;
			break;
		case(MARK_AND_SELECT_BUTTON):
			catSelectModel->chooseCatButton = catSelectEvent->buttonNum;
			returnStateId = catSelectStates[catSelectEvent->buttonNum];
			break;
		case(NO_EVENT):
			break;
	}
	free(catSelectEvent);
	return returnStateId;
}
 */
 /*StateId mainMenuPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = MAIN_MENU;
	if (logicalEvent == NULL || viewState == NULL || model == NULL)
		return returnStateId;
	logicalEventRef mainMenuEvent = logicalEvent;
	GameDataRef mainMenuModel = model;
	ViewStateref mainMenuView = viewState;
	StateId mainMenuStates[MAIN_MENU_NUM_BUTTONS] = {CHOOSE_CAT, LOAD_GAME, WORLD_BUILDER, EDIT_GAME, QUIT};
	int currButton = mainMenuModel->mainMenuButton;
	switch(mainMenuEvent->type){
		case(SELECT_CURR_BUTTON):
			returnStateId = mainMenuStates[currButton];
			if (returnStateId == CHOOSE_CAT)
				mainMenuModel->preChooseCat = MAIN_MENU;
			break;
		case(MARK_NEXT_BUTTON):
			changeSelectedButton(mainMenuView->menuButtons[currButton],
					mainMenuView->menuButtons[(currButton+1)%MAIN_MENU_NUM_BUTTONS]);
			mainMenuModel->mainMenuButton = (currButton + 1)%MAIN_MENU_NUM_BUTTONS;
			break;
		case(MARK_AND_SELECT_BUTTON):
			mainMenuModel->mainMenuButton = mainMenuEvent->buttonNum;
			returnStateId = mainMenuStates[mainMenuEvent->buttonNum];
			break;
		case(NO_EVENT):
			break;
	}
	free(mainMenuEvent);
	return returnStateId;
}*/
