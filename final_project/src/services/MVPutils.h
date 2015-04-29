#ifndef MVP_UTILS_H_
#define MVP_UTILS_H_

/* includes: */
#include "GUIUtils.h"
#include "CatAndMouseUtils.h"
#include "../views/Views.h"
#include "../presenters/MenusAndErrPresenters.h"
#include "../presenters/PlayGamePresenter.h"
#include "../presenters/WorldBuilderPresenter.h"


/******* functions declaration: *******/

/***** MVP helper functions *****/

/** data initialization functions: **/
ViewStateref initGUIViewState();
MenuDataRef initMenuDataToDefault();
void initMenuModel(GUIref gui, void* initData);
void initMainMenuData(GUIref mainMenuGui, void * initData);
void initWorldBuilderModel(GUIref gui, void* initData);
void initPlayGameModel(GUIref gui, void* initData);

/**** general MVP helper functions ****/

/** VTE helper functions **/
void handleThreePartLayoutMouseSelect(SDL_Event * event, logicalEventRef returnEvent, Widget ** buttons, int numButtons);
void handleThreePartLayoutArrowKey(SDLKey key, logicalEventRef returnEvent);
int isClickEventOnButton(SDL_Event* event, Widget * button, int buttonType);
int isClickEventOnButtonSpecificArea(SDL_Event * event, Widget * button, int Xdelta, int YdeltaUp, int YdeltaDown);

/** PHE helper functions **/
/* buttons update related functions */
void changeSelectedButton(Widget * oldButton, Widget * newButton);
void setValuesButtonFromInit(int value, Widget* valuesButton);
void increaseValuesButton(int* currValue, int maxValue, Widget* valuesButton);
void decreaseValuesButton(int* currValue, int maxValue, Widget* valuesButton);

/** grid related functions **/
void createGridItemsImgArr(ViewStateref viewState);
void createGridByData(Widget *gridPanel, char **gridData, Widget **gridItemImages);
void selectGridPos(Widget *gridPanel, Widget ** gridSelectImages, gridItemPosition currPos);
void deselectGridPos(Widget *gridPanel, Widget ** gridDeselectImages, gridItemPosition currPos);
void changeSelectedGridSquare(Widget * gridPanel, Widget ** gridItemsImages, gridItemPosition * currPos,
		gridItemPosition newPos);
void blitItemToGrid(Widget *gridPanel, Widget * itemImage, int row, int col);

/**** layout functions - used for setting/ updating the layout of the view ****/
void setThreePartLayout(ViewStateref viewState, char ** gameGridData);
void addButtonsToSidePanel(ViewStateref viewState, int buttonImgX, int buttonImgY,
		int buttonImgDisX, int buttonImgDisY, int fromButtonNum, int toButtonNum, int isDisabled);
void addButtonsToWBTopPanel(ViewStateref viewState, int buttonImgX, int buttonImgY,
		int buttonImgDisX, int buttonImgDisY, int fromButtonNum, int toButtonNum, int isDisabled);

/**** MVP free data functions ****/
void freeViewState(ViewStateref guiViewState);
void freeGridItemsArray(Widget ** gridItemsImages);
void freeMenuData(MenuDataRef menuData);
void freeWBData(WBDataRef wbData, int doFreeGrid);
void freePGData(PGDataRef pgData, int doFreeGrid);

/** play game specific functions **/
/* play game layout functions */
void setGameOver(PGDataRef pgModel, ViewStateref pgViewState);
void setTopPanelGameOver(PGDataRef pgModel, ViewStateref pgViewState);
void setPlayGame(ViewStateref pgViewState, PGDataRef pgModel);
void updatePlayGame(ViewStateref pgViewState, PGDataRef pgModel, gridItemPosition * eventPos);
void setTopPanelPlayGame(PGDataRef pgModel, ViewStateref pgViewState);
void updateTopPanelPlayGame(ViewStateref pgViewState, PGDataRef pgModel);
void setPauseButton(PGDataRef pgModel, ViewStateref pgViewState);
void setNumTurnsLabels(PGDataRef pgModel, ViewStateref pgViewState);
void setPlayerMoveLabel(PGDataRef pgModel, ViewStateref pgViewState);
void setPlayerStateLabel(PGDataRef pgModel, ViewStateref pgViewState);
/* play game get data functions */
int getPGButtonNum(SDLKey key);
gridItemPosition * getCurrPlayerPos(PGDataRef pgModel);
int checkAndupdateGameOverType(PGDataRef pgModel);
int isCurrPlayerHuman(PGDataRef pgModel);
/* play game make move functions */
void makeGameMoveIfLegal(ViewStateref pgViewState, PGDataRef pgModel, gridItemPosition eventPos);
void makeGameMoveByArrowIfLegal(ViewStateref pgView, PGDataRef pgModel, direction direction);
void warnIllegalMove(ViewStateref pgViewState, gridItemPosition eventPos, gridItemPosition currPlayerPos);
void updateMachineMoveIfNeeded(GUI activeGUI);
/* play game change state functions */
void restartGame(ViewStateref pgViewState, PGDataRef pgModel);
void pauseGame(ViewStateref pgViewState, PGDataRef pgModel);
void resumeGame(ViewStateref pgViewState, PGDataRef pgModel);
void disablePGSidePanelButtons(ViewStateref pgViewState);
void enablePGSidePanelButtons(ViewStateref pgViewState);


/**** world builder helper functions ****/
/* world builder get data functions */
int getWBButtonNum(SDLKey key);
int isGridInvalid(WBDataRef wbModel);
/* world builder grid functions */
void putGridItemInPos(WBDataRef wbModel, Widget * gridPanel, Widget ** gridItemsImages,
		gridItemPosition currPos, int buttonNum);
void addReusableItemToPos(gridItem itemType, char ** gridData, Widget * gridPanel,
		Widget ** gridItemsImages, gridItemPosition currPos);
void moveItemToPos(gridItem itemType, Widget ** gridItemsImages, Widget * gridPanel, char ** gridData,
		gridItemPosition currPos, gridItemPosition * prevItemPos);
void fixOverride(gridItem itemType, WBDataRef wbModel, gridItemPosition currPos);
void changeSelectedPosByArrow(Widget * gridPanel, Widget ** gridItemsImages,
		gridItemPosition * currPos, direction direction);


#endif /* MVP_UTILS_H_ */
