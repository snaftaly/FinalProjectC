#ifndef MVP_UTILS_H_
#define MVP_UTILS_H_

/* includes: */
#include "gui_utils.h"

/* functions declaration: */
GUI createGUIForState(StateId stateId);
ViewStateref initializeGUIViewState();

void* simpleMenuVTE(void* viewState, SDL_Event* event, int numOfButtons);
StateId generalMenuPHE(void* model, void* viewState, void* logicalEvent, StateId states[], int numOfButtons,
		StateId stateId, int* currButton);
void* menuStop(GUIref gui);

void startMainMenu(GUIref gui, void* initData);
void* mainMenuVTE(void* viewState, SDL_Event* event);
StateId mainMenuPHE(void* model, void* viewState, void* logicalEvent);
void startChooseAnimal(GUIref gui, void* initData, int animal);
void startChooseCat(GUIref gui, void* initData);
void startChooseMouse(GUIref gui, void* initData);
void* chooseAnimalVTE(void* viewState, SDL_Event* event);
StateId chooseCatPHE(void* model, void* viewState, void* logicalEvent);



#endif /* MVP_UTILS_H_ */
