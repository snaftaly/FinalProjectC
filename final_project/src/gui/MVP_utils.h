#ifndef MVP_UTILS_H_
#define MVP_UTILS_H_

/* includes: */
#include <stdio.h>
#include "gui_utils.h"
#include "../main/CatAndMouseUtils.h"


/**** enums ****/

/* an enumeration of all the different states of the program.
/ each state should correspond to a specific GUI.*/
typedef enum{
	SELECT_CURR_BUTTON,
	MARK_NEXT_BUTTON,
	SELECT_BUTTON_NUM,
	MARK_VALUES_BUTTON,
	INCREASE_VALUE,
	DECREASE_VALUE,
	GO_UP,
	GO_RIGHT,
	GO_LEFT,
	GO_DOWN,
	SELECT_SQUARE,
	NO_EVENT  /* is necessary ???? */
} logicalEventType;

typedef enum {
	MAIN_MENU,
	CHOOSE_CAT,
	CHOOSE_MOUSE,
	CAT_SKILL,
	MOUSE_SKILL,
	LOAD_GAME,
	EDIT_GAME,
	WORLD_BUILDER,
	SAVE_WORLD,
	PLAY_GAME,
	ERR_MSG,
	QUIT
} StateId;


typedef struct logicalEvent{
	logicalEventType type;
	int buttonNum;
	gridItemPosition gridPos;
}logicalEvent;



typedef struct logicalEvent * logicalEventRef;

typedef struct ViewState{
	SDL_Surface * image;
	SDL_Surface * bgImage;
	SDL_Surface * gridItemImage;
	Widget ** menuButtons;
	ListRef UITree;
	int currButton;
	ListRef topPanelNode;
	ListRef sidePanelNode;
	Widget * gridPanel;
	Widget ** gridItemsImgArr;
	Widget ** labelArr;
} ViewState;
typedef struct ViewState * ViewStateref;

typedef struct MenuData{
	int catSkill;
	int mouseSkill;
	int isCatHuman;
	int isMouseHuman;

	int mainMenuButton;
	int chooseCatButton;
	StateId preChooseCat;
	int chooseMouseButton;
	StateId preChooseMouse;
	StateId preWorldBuilder;
	int catSkillButton;
	int mouseSkillButton;
	int loadGameButton;
	int editGameButton;
	int saveWorldButton;
	int errMsgButton;

	int currValueTemp;
	int loadGameWorld;
	int editedWorld;
	int saveOnWorld;

	int isCatFirst;
	int numTurns;
	gridItemPosition wbCurrPos;
	int loadFromFile;
	int missingItems;
	int isGamePaused;

	StateId retStateId;

	char ** gameGridData;

} MenuData;

typedef struct MenuData * MenuDataRef;


typedef struct WBData{

	gridItemPosition catPos;
	gridItemPosition mousePos;
	gridItemPosition cheesePos;
	gridItemPosition currPos;
	int editedWorld;
	int isCatFirst;
	int numTurns;
	int currValueTemp;
	char missingItems;

	StateId returnStateId;

	char ** gameGridData;

} WBData;

typedef struct WBData * WBDataRef;

typedef struct PGData{

	gridItemPosition catPos;
	gridItemPosition mousePos;
	gridItemPosition cheesePos;
	//gridItemPosition currPos; we probably don't need this!
	int loadGameWorld;
	int loadFromFile;

	int isCatCurrPlayer;
	int numTurnsLeft;
	int isGameOver;
	gameOverType gameOverType;
	int isGamePaused;
	int doRestartGame;

	int isCatHuman;
	int isMouseHuman;
	int catSkill;
	int mouseSkill;

	StateId returnStateId;

	char ** gameGridData;

} PGData;

typedef struct PGData * PGDataRef;




/* The GUI structure. */
typedef struct GUI {
	/* The unique state id: */
	StateId stateId;

	/* The model and viewState. Assumed to be NULL until GUI is started and once it is stopped.
	/ The model and view state should NOT hold references to each other. */
	void *model, *viewState;

	/* The function pointer for starting this GUI - initializing the model
	 * and viewState as well as drawing the initial state of the GUI.*/
	void (*start) (struct GUI* gui, void* initData);

	/* The function pointer for translating the SDL_Event to a logical event according to the current viewState.
	/ The logical event will be passed to the presenter which will take care of updating the model and/or the view.
	/ The data type of the logical event is assumed to be known by the presenter and will also be freed by it, if need be.*/
	void* (*viewTranslateEvent) (void* viewState, SDL_Event* event);

	/* The function pointer for handling the given logical event.
	/ may or may not update the model and/or viewState.
	/ Returns the next state to transition to. */
	StateId (*presenterHandleEvent) (void* model, void* viewState, void* logicalEvent);

	/* The function pointer for deactivating this GUI - freeing the model and viewState and any other
	 * resources used. Returns the initialization data for the next state (can be NULL if not required).*/
	void* (*stop) (struct GUI* gui);
} GUI;

typedef struct GUI* GUIref;






/**** functions declaration: ****/
GUI createGUIForState(StateId stateId);

/** MVP functions: **/
/* start functions */
void initMenuModel(GUIref gui, void* initData);
void initMainMenuData(GUIref mainMenuGui, void * initData);
void initWorldBuilderModel(GUIref gui, void* initData);
void initPlayGameModel(GUIref gui, void* initData);
ViewStateref initGUIViewState();
MenuDataRef initMenuDataToDefault();

void startGeneralMenu(GUIref gui, char * imgPath, int titleImgX, int titleImgY, int titleWidth, int numButtons, int selectedButton, int firstButtonNumOpts, int value);
void startMainMenu(GUIref gui, void* initData);
void startChooseAnimal(GUIref gui, void* initData);
void startAnimalSkill(GUIref gui, void* initData);
void startWorldMenu(GUIref gui, void* initData);
void startWorldBuilder(GUIref gui, void* initData);
void startErrMsg(GUIref gui, void* initData);
void startPlayGame(GUIref gui, void* initData);


/* VTE fucntions: */
void* simpleMenuVTE(void* viewState, SDL_Event* event, int numOfButtons);
void* mainMenuVTE(void* viewState, SDL_Event* event);
void* chooseAnimalVTE(void* viewState, SDL_Event* event);
void* complexMenuVTE(void* viewState, SDL_Event* event);
void* worldBuilderVTE(void* viewState, SDL_Event* event);
void* errMsgVTE(void* viewState, SDL_Event* event);
void* playGameVTE(void* viewState, SDL_Event* event);

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
StateId errMsgPHE(void* model, void* viewState, void* logicalEvent);
StateId playGamePHE(void* model, void* viewState, void* logicalEvent);

/* stop functions */
void* stopMenu(GUIref gui); /* maybe this will be a general stop function */
void* stopWorldBuilder(GUI * gui);
void* stopPlayGame(GUI * gui);


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
		gridItemPosition * currPos, direction direction);
void changeSelectedGridSquare(Widget * gridPanel, Widget ** gridItemsImages, gridItemPosition * currPos,
		gridItemPosition newPos);

void saveGridDataToFile(int worldNum, int isCatFirst, char ** gridData);
int isGridInvalid(WBDataRef wbModel);

int updateGameOver(PGDataRef pgModel);
int isCurrPlayerHuman(PGDataRef pgModel);
int getWBButtonNum(SDLKey key);
int getPGButtonNum(SDLKey key);

//thre part gui layout functions:
void addButtonsToSidePanel(ViewStateref viewState, int buttonImgX, int buttonImgY,
		int buttonImgDisX, int buttonImgDisY, int fromButtonNum, int toButtonNum, int isDisabled);
void setThreePartLayout(ViewStateref viewState, char ** gameGridData);
void createGridItemsImgArr(ViewStateref viewState);

//play game top panel setup functions:
void setTopPanelGameOver(PGDataRef pgModel, ViewStateref pgViewState);
void setTopPanelPlayGame(PGDataRef pgModel, ViewStateref pgViewState);
void updateTopPanelPlayGame(ViewStateref pgViewState, PGDataRef pgModel);
void setPauseButton(PGDataRef pgModel, ViewStateref pgViewState);
void setNumTurnsLabels(PGDataRef pgModel, ViewStateref pgViewState);
void setPlayerMoveLabel(PGDataRef pgModel, ViewStateref pgViewState);
void setPlayerStateLabel(PGDataRef pgModel, ViewStateref pgViewState);
void enablePGSidePanelButtons(ViewStateref pgViewState);
void disablePGSidePanelButtons(ViewStateref pgViewState);
void pauseGame(ViewStateref pgViewState, PGDataRef pgModel);
void resumeGame(ViewStateref pgViewState, PGDataRef pgModel);
void restartGame(ViewStateref pgViewState, PGDataRef pgModel);
void warnIllegalMove(ViewStateref pgViewState, gridItemPosition eventPos, gridItemPosition currPlayerPos);
void changePosDirection(gridItemPosition * currPos, logicalEventType direction);
void makeGameMoveByArrowIfLegal(ViewStateref pgView, PGDataRef pgModel, direction direction);
void makeGameMoveIfLegal(ViewStateref pgViewState, PGDataRef pgModel, gridItemPosition eventPos);
gridItemPosition * getCurrPlayerPos(PGDataRef pgModel);
void handleThreePartLayoutMouseSelect(SDL_Event * event, logicalEventRef returnEvent, Widget ** buttons, int numButtons);
void handleThreePartLayoutArrowKey(SDLKey key, logicalEventRef returnEvent);


void freeViewState(ViewStateref guiViewState);
void freeGridItems(Widget ** gridItemsImages);
void freeMenuData(MenuDataRef menuData);
void freePGData(PGDataRef pgData, int doFreeGrid);
void freeWBData(WBDataRef wbData, int doFreeGrid);


//void printPos(gridItemPosition pos); //delete this functions!!!!!!
void updateMachineMoveIfNeeded(GUI activeGUI);



#endif /* MVP_UTILS_H_ */
