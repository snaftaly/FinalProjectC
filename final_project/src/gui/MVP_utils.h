#ifndef MVP_UTILS_H_
#define MVP_UTILS_H_

/* includes: */
#include <stdio.h>

#include "gui_utils.h"

/**** functions declaration: ****/
GUI createGUIForState(StateId stateId);

/** MVP functions: **/
/* start functions */
void drawGui(GUIref gui);
void initializeMenuModel(GUIref gui, void* initData);
void initializeWorldBuilderModel(GUIref gui, void* initData);
ViewStateref initializeGUIViewState();
void startGeneralMenu(GUIref gui, void * initData, char * imgPath, int titleImgX, int titleImgY, int titleWidth, int numButtons, int selectedButton, int firstButtonNumOpts, int value);
void startMainMenu(GUIref gui, void* initData);
void startChooseAnimal(GUIref gui, void* initData);
void startAnimalSkill(GUIref gui, void* initData);
void startWorldMenu(GUIref gui, void* initData);
void startWorldBuilder(GUIref gui, void* initData);

/* VTE fucntions: */
void* simpleMenuVTE(void* viewState, SDL_Event* event, int numOfButtons);
void* mainMenuVTE(void* viewState, SDL_Event* event);
void* chooseAnimalVTE(void* viewState, SDL_Event* event);
void* complexMenuVTE(void* viewState, SDL_Event* event);

/* PHE functions */
StateId generalMenuPHE(void* model, void* viewState, void* logicalEvent, StateId states[], int numOfButtons,
		StateId stateId, int* currButton, int* currValue, int maxValue);
StateId mainMenuPHE(void* model, void* viewState, void* logicalEvent);
StateId chooseCatPHE(void* model, void* viewState, void* logicalEvent);
StateId chooseMousePHE(void* model, void* viewState, void* logicalEvent);
StateId catSkillPHE(void* model, void* viewState, void* logicalEvent);
StateId mouseSkillPHE(void* model, void* viewState, void* logicalEvent);
StateId loadGamePHE(void* model, void* viewState, void* logicalEvent);
StateId editGamePHE(void* model, void* viewState, void* logicalEvent);
StateId saveWorldPHE(void* model, void* viewState, void* logicalEvent);

/* stop functions */
void* menuStop(GUIref gui); /* maybe this will be a general stop function */

char ** initGameData(int worldNum, int * numTurns, int * isCatFirst);
void setEmptyGrid(char ** grid);
char ** initGrid();
void initColumns(int rownum, int colnum, char ** grid);
void setImageTransparent(Widget *image, int red, int green, int blue);
void setGridPosSelected(Widget *gridPanel, Widget *gridSelectImage, int row, int col);
void createGridByData(Widget *gridPanel, char **gridData, Widget **gridItemImages);


#endif /* MVP_UTILS_H_ */
