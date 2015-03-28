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
void* worldBuilderVTE(void* viewState, SDL_Event* event);

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
StateId worldBuilderPHE(void* model, void* viewState, void* logicalEvent);

/* stop functions */
void* menuStop(GUIref gui); /* maybe this will be a general stop function */
void* stopWorldBuilder(GUI * gui);

char ** initGameData(int worldNum, int * numTurns, int * isCatFirst);
void setEmptyGrid(char ** grid);
char ** initGrid();
void initColumns(int rownum, int colnum, char ** grid);
void selectGridPos(Widget *gridPanel, Widget ** gridSelectImages, gridItemPosition currPos);
void deselectGridPos(Widget *gridPanel, Widget ** gridDeselectImages, gridItemPosition currPos);//maybe??????
void createGridByData(Widget *gridPanel, char **gridData, Widget **gridItemImages);
void blitItemToGrid(Widget *gridPanel, Widget * itemImage, int row, int col);
void putGridItemInPos(WBDataRef wbModel, Widget * gridPanel, Widget ** gridItemsImages,
		gridItemPosition currPos, int buttonNum);
void addReusableItemToPos(gridItem itemType, char ** gridData, Widget * gridPanel,
		Widget ** gridItemsImages, gridItemPosition currPos);
void moveItemToPos(gridItem itemType, Widget ** gridItemsImages, Widget * gridPanel, char ** gridData,
		gridItemPosition currPos, gridItemPosition * prevItemPos);
char getItemChar(gridItem item);
void fixOverride(gridItem itemType, WBDataRef wbModel, gridItemPosition currPos);
void changeSelectedPosByArrow(Widget * gridPanel, Widget ** gridItemsImages,
		gridItemPosition * currPos, logicalEventType direction);
void changeSelectedGridSquare(Widget * gridPanel, Widget ** gridItemsImages, gridItemPosition * currPos,
		gridItemPosition newPos);
void updateItemsPostions(gridItemPosition * mousePos,gridItemPosition * catPos,gridItemPosition * cheesePos,
		char ** gameGridData);
void saveGridDataToFile(int worldNum, int isCatFirst, char ** gridData);
int isSamePos(gridItemPosition pos1, gridItemPosition pos2);
int isGridValid(WBDataRef wbModel);








void freeViewState(ViewStateref guiViewState);
void freeGridItems(Widget ** gridItemsImages);
void freeGridData(char ** gridData);



#endif /* MVP_UTILS_H_ */
