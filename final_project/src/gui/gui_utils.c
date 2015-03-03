/* includes */
#include "gui_utils.h"



int addWidgetToParent(ListRef parent){
	ListRef currChild = parent->child;
	if (currChild==NULL){
		return 0;
	}
	Widget * parentWidget = parent->data;
	while(currChild != NULL){
		Widget * childWidget = currChild->data;
		childWidget->parentWidget = parentWidget;
		switch (childWidget->type){
			case(PANEL):
				if (SDL_BlitSurface(childWidget->surface, NULL,
						parentWidget->surface, &childWidget->location_rect) != 0){
					fprintf(stderr, "ERROR: failed to blit panel: %s\n", SDL_GetError());
					return -1;
				}
				break;
			case(BUTTON):
				if (SDL_BlitSurface(childWidget->surface, &childWidget->img_rect,
						parentWidget->surface, &childWidget->location_rect) != 0){
					fprintf(stderr, "ERROR: failed to blit button: %s\n", SDL_GetError());
					return -1;
				}
				break;
			case(IMAGE):
				if (SDL_BlitSurface(childWidget->surface, &childWidget->img_rect,
						parentWidget->surface, &childWidget->location_rect) != 0){
					fprintf(stderr, "ERROR: failed to blit image/label: %s\n", SDL_GetError());
					return -1;
				}
				break;
			default:
				break;
		}
		currChild = currChild->next;
	}
	return 0;
}

GUI createGUIForState(StateId stateId){
	GUI returnGUI;
	returnGUI.stateId = stateId;
	returnGUI.model = NULL;
	returnGUI.viewState = NULL;
	switch(stateId){
		case(MAIN_MENU):
			returnGUI.start = startMainMenu;
			returnGUI.stop = stopMainMenu;
			/*...*/
			break;
		/*
		case(LOAD_GAME):
			returnGUI.start = startLoadGame;
			returnGUI.stop = stopLoadGame;
			break;
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

void startMainMenu(GUIref gui, void* initData){
	/* create widgets array and UItree and image*/
	Widget ** widgets = (Widget **)malloc(MAIN_MENU_NUM_WIDGETS*sizeof(Widget *));
	if (widgets == NULL){
		perrorPrint("malloc");
	}
	SDL_Surface *mainMenuImages = SDL_LoadBMP("../../images/mainMenuButtons.bmp");
	if (mainMenuImages == NULL){
		sdlErrorPrint("failed to load image");
		return;
	}
	int curr = 0;
	Widget *win = create_window(WIN_W,WIN_H);
	if (win == NULL){
		free(widgets);
		return;
	}
	widgets[curr++] = win;
	ListRef win_node = newList(win);
	if (win_node == NULL){
		freeWidget(widgets[curr-1]);
		free(widgets);
		return;
	}
	Widget *panel = create_panel(30,50,300,600,0,0,0);
	if (panel == NULL){
		freeTree(win_node, freeWidget);
		free(widgets);
		return;
	}
	widgets[curr++] = panel;
	ListRef panel_node = addChildNode(win, panel);
	if (panel_node == NULL){
		freeWidget(widgets[curr-1]);
		freeTree(win_node, freeWidget);
		free(widgets);
	}

	Widget *label = create_image(30,50,300,600,mainMenuImages,0,0);
	if (label == NULL){
		freeTree(win_node, freeWidget);
		free(widgets);
		return;
	}
	widgets[curr++] = label;
	ListRef label_node = addChildNode(panel, label);
	if (label_node == NULL){
		freeWidget(widgets[curr-1]);
		freeTree(win_node, freeWidget);
		free(widgets);
	}

	int button_x = 20, button_y = 40, isSelected_x = 90, isSelected_y = 100, isNselected_x = 90, isNselected_y=120;
	while (curr != MAIN_MENU_NUM_WIDGETS+1){
		widgets[curr++] = create_button(button_x,button_y, 100, 100,
				mainMenuImages, isSelected_x, isSelected_y, isNselected_x, isNselected_y, 0);
		if (widgets[curr-1] == NULL){
			freeTree(win_node, freeWidget);
			free(widgets);
			return;
		}
		ListRef newButtonNode = addChildNode(panel, widgets[curr-1]);
		if (newButtonNode == NULL){
			freeWidget(widgets[curr-1]);
			freeTree(win_node, freeWidget);
			free(widgets);
		}
		button_y +=40;
		isSelected_y +=30;
		isNselected_y +=30;
	}
	if (initData == NULL){
		widgets[3]->isButtonSelected = 1;
		widgets[3]->img_rect = widgets[3]->button_selected_rect;
	}
	else{
		/*take care of other init data options */
	}
	/* draw GUI according to UItree */
	treeDFS(win_node, addWidgetToParent);
	if (SDL_Flip(win) != 0) {
		sdlErrorPrint("failed to flip buffer");
		return;
	}
	/* build main menu viewState */
	ViewStateref mainMenuView = (ViewStateref)malloc(sizeof(ViewState));
	mainMenuView->UITree = win_node;
	mainMenuView->image = mainMenuImages;
	mainMenuView->widgets = widgets;
	/* build main menu model */


}
