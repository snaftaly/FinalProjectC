#include "gui_utils.h"

void startMainMenu(GUIref gui, void* initData){
	/* allocate memory for create viewState */
	ViewStateref mainMenuView = initializeGUIViewState();
	if (mainMenuView == NULL){
		return;
	}
	gui->viewState = mainMenuView;

	/* create image surface */
	SDL_Surface *mainMenuImages = SDL_LoadBMP("../../images/mainMenuButtons.bmp");
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
	Widget *win = create_window(WIN_W,WIN_H);
	if (win == NULL){
		return;
	}
	ListRef win_node = newList(win);
	mainMenuView->UITree = win_node;
	if (win_node == NULL){
		freeWidget(win);
		return;
	}
	Widget *panel = create_panel(30,50,300,600,0,0,0);
	if (panel == NULL){
		return;
	}
	ListRef panel_node = addChildNode(win, panel);
	if (panel_node == NULL){
		freeWidget(panel);
		return;
	}
	Widget *label = create_image(30,50,300,600,mainMenuImages,0,0);
	if (label == NULL){
		return;
	}
	ListRef label_node = addChildNode(panel, label);
	if (label_node == NULL){
		freeWidget(label);
		return;
	}
	/* Add buttons to buttons array and to UI tree */
	int button_x = 20, button_y = 40, isSelected_x = 90, isSelected_y = 100, isNselected_x = 90, isNselected_y=120;
	for (int i = 0; i < MAIN_MENU_NUM_BUTTONS; i++){
		buttons[i] = create_button(button_x,button_y, 100, 100,
				mainMenuImages, isSelected_x, isSelected_y, isNselected_x, isNselected_y, 0);
		if (buttons[i] == NULL){
			return;
		}
		ListRef newButtonNode = addChildNode(panel, buttons[i]);
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
	setButtonSelected(buttons[data->mainMenuButton]);

	/* draw GUI according to UItree */
	treeDFS(win_node, calcAbsWidgetXY, addWidgetToParent);
	if (SDL_Flip(win) != 0) {
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
	generalMenuPHE(model, viewState, logicalEvent, mainMenuStates, MAIN_MENU_NUM_BUTTONS, returnStateId,
			&mainMenuModel->mainMenuButton);
	if (returnStateId == CHOOSE_CAT)
		mainMenuModel->preChooseCat = MAIN_MENU;
	return returnStateId;
}

void* mainMenuStop(GUIref gui){ /* maybe this will be a general stop function */
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

void startCatSelctionMenu(GUIref gui, void* initData){
	startAnimalSelctionMenu(gui, initData, IS_CAT);
}

void startCatSelctionMenu(GUIref gui, void* initData){
	startAnimalSelctionMenu(gui, initData, IS_MOUSE);
}

void startAnimalSelctionMenu(GUIref gui, void* initData, int animal){
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
	Widget *win = create_window(WIN_W,WIN_H);
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
	ListRef panel_node = addChildNode(win, panel);
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
	ListRef label_node = addChildNode(panel, label);
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
		ListRef newButtonNode = addChildNode(panel, buttons[i]);
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
	treeDFS(win_node, calcAbsWidgetXY, addWidgetToParent);
	if (SDL_Flip(win) != 0) {
		sdlErrorPrint("failed to flip buffer");
		return;
	}
}

void* animalSelectionVTE(void* viewState, SDL_Event* event){
	logicalEventRef returnEvent = simpleMenuVTE(viewState, event, COMMON_MENU_NUM_BUTTONS);
	return returnEvent;
}

void* simpleMenuVTE(void* viewState, SDL_Event* event, int numOfButtons){
	logicalEventRef returnEvent = (logicalEventRef)malloc(sizeof(logicalEvent));
	if (returnEvent == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	switch (event.type) {
		case (SDL_KEYUP):
			if (event.key.keysym.sym == SDLK_TAB){
				returnEvent->type = MARK_NEXT_BUTTON;
			}
			if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER){
				returnEvent->type = SELECT_CURR_BUTTON;
			}
			break;
		case (SDL_MOUSEBUTTONUP):
			ViewStateref menuViewState = viewState;
			for (int i = 0; i< numOfButtons; i++){
				Widget * currButton = menuViewState->menuButtons[i];
				if (isClickEventOnButton(event, currButton)){
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

StateId generalMenuPHE(void* model, void* viewState, void* logicalEvent, StateId states[], int numOfButtons,
		StateId stateId, int* currButton){
	StateId returnStateId = stateId;
	if (logicalEvent == NULL || viewState == NULL)
		return returnStateId;
	logicalEventRef menuEvent = logicalEvent;
	GameDataRef menuModel = model;
	ViewStateref menuView = viewState;
	switch(menuEvent->type){
		case(SELECT_CURR_BUTTON):
			returnStateId = states[*currButton];
			break;
		case(MARK_NEXT_BUTTON):
			changeSelectedButton(menuView->menuButtons[*currButton],
					menuView->menuButtons[(*currButton+1)%numOfButtons]);
			*currButton = (*currButton + 1)%numOfButtons;
			break;
		case(MARK_AND_SELECT_BUTTON):
			*currButton = menuEvent->buttonNum;
			returnStateId = states[menuEvent->buttonNum];
			break;
		case(NO_EVENT):
			break;
	}
	free(logicalEvent);
	return returnStateId;

}

StateId chooseCatPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = CHOOSE_CAT;
	if (model == NULL)
		return returnStateId;
	GameDataRef chooseCatModel = model;
	StateId chooseCatStates[COMMON_MENU_NUM_BUTTONS] = {CHOOSE_MOUSE, CAT_SKILL, chooseCatModel->preChooseCat};
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, chooseCatStates, COMMON_MENU_NUM_BUTTONS,
			returnStateId, &chooseCatModel->chooseCatButton);
	if (returnStateId == CHOOSE_MOUSE);
		chooseCatModel->preChooseMouse = CHOOSE_CAT;
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
}
  */
  */
