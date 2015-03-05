#include "gui_utils.h"

void startMainMenu(GUIref gui, void* initData){
	/* create image surface */
	SDL_Surface *mainMenuImages = SDL_LoadBMP("../../images/mainMenuButtons.bmp");
	if (mainMenuImages == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	/* create buttons array */
	Widget ** buttons = (Widget **)malloc(MAIN_MENU_NUM_BUTTONS*sizeof(Widget *));
	if (buttons == NULL){
		perrorPrint("malloc");
		return;
	}
	/* allocate memory for create viewState */
	ViewStateref mainMenuView = (ViewStateref)malloc(sizeof(ViewState));
	if (mainMenuView == NULL){
		perrorPrint("malloc");
		break;
	}
	/* create the UItree */
	ListRef win_node = NULL;
	while(1){
		Widget *win = create_window(WIN_W,WIN_H);
		if (win == NULL){
			break;
		}
		ListRef win_node = newList(win);
		if (win_node == NULL){
			freeWidget(win);
			break;
		}
		Widget *panel = create_panel(30,50,300,600,0,0,0);
		if (panel == NULL){
			break;
		}
		ListRef panel_node = addChildNode(win, panel);
		if (panel_node == NULL){
			freeWidget(panel);
			break;
		}
		Widget *label = create_image(30,50,300,600,mainMenuImages,0,0);
		if (label == NULL){
			break;
		}
		ListRef label_node = addChildNode(panel, label);
		if (label_node == NULL){
			freeWidget(label);
			break;
		}
		/* Add buttons to buttons array and to UI tree */
		int buttonsStatus = 0; /* 0 - buttons arr not yet created, 1 - error creating button */
		int button_x = 20, button_y = 40, isSelected_x = 90, isSelected_y = 100, isNselected_x = 90, isNselected_y=120;
		for (int i = 0; i < MAIN_MENU_NUM_BUTTONS; i++){
			buttons[i] = create_button(button_x,button_y, 100, 100,
					mainMenuImages, isSelected_x, isSelected_y, isNselected_x, isNselected_y, 0);
			if (buttons[i] == NULL){
				buttonsStatus = 1;
				break;
			}
			ListRef newButtonNode = addChildNode(panel, buttons[i]);
			if (newButtonNode == NULL){
				freeWidget(buttons[i]);
				buttonsStatus = 1;
				break;
			}
			button_y +=40;
			isSelected_y +=30;
			isNselected_y +=30;
		}
		if (buttonsStatus){ /* check if there was an error while creating the buttons */
			break;
		}
		buttonsStatus = 2;
		if (initData == NULL){
			buttons[0]->isButtonSelected = 1;
			buttons[0]->img_rect = buttons[0]->button_selected_rect;
		}
		else{
			/* take care of other init data options */
		}
		/* draw GUI according to UItree */
		treeDFS(win_node, calcAbsWidgetXY, addWidgetToParent);
		if (SDL_Flip(win) != 0) {
			sdlErrorPrint("failed to flip buffer");
			break;
		}
		/* build main menu viewState */
		mainMenuView->UITree = win_node;
		mainMenuView->image = mainMenuImages;
		mainMenuView->menuButtons = buttons;
		/* build main menu model... (To Do!) */
		if (initData == NULL)
			gui->model = initGameDataToDefault(); /* write this function */
		else
			gui->model = initData;
	}
	if (isError){ /* maybe move to stop */
		free(buttons);
		free(mainMenuView);
		freeTree(win_node, freeWidget);
		SDL_FreeSurface(mainMenuImages);
	}
}

void* mainMenuVTE(void* viewState, SDL_Event* event){
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
			ViewStateref mainMenuViewState = viewState;
			for (int i = 0; i< MAIN_MENU_NUM_BUTTONS; i++){
				Widget * currButton = mainMenuViewState->menuButtons[i];
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

StateId mainMenuPHE(void* model, void* viewState, void* logicalEvent){
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
