#ifndef MVP_UTILS_H_
#define MVP_UTILS_H_

/* includes: */
#include <stdio.h>
#include "gui_utils.h"
#include "../main/CatAndMouseUtils.h"


/**** enums ****/

/* an enumeration of all the different states of the program.
/ each state corresponds to a specific GUI.*/
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

/* an enumeration of all the logical event types .*/
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
	NO_EVENT
} logicalEventType;

/* enumeration of the different grid Items */
typedef enum{
	CAT,
	MOUSE,
	CHEESE,
	WALL,
	EMPTY,
	SELECT,
	DESELECT,
	WARN
} gridItem;

/**** structs ****/

/* a struct fot the logical event
 * holds event type, and if necessary will hold the
 * button number pressed or the grid position pressed */
typedef struct logicalEvent{
	logicalEventType type;
	int buttonNum;
	gridItemPosition gridPos;
}logicalEvent;

typedef struct logicalEvent * logicalEventRef;

/* a struct for the view state */
typedef struct ViewState{
	/* SDL_Surface refs for the different images used by the gui */
	SDL_Surface * image;
	SDL_Surface * bgImage;
	SDL_Surface * gridItemImage; /* used by play game and world builder */
	/* Widgets * arrays that will hold different widgets used by the gui for easy access */
	Widget ** menuButtons;
	Widget ** labelArr;
	Widget ** gridItemsImgArr; /* used by play game and world builder */

	ListRef UITree; /* holds all the widgets of the GUI (except grid items images widgets) */
	int currButton; /* holds the menus current selected button */
	/* list refs of the top/side panel nodes, used by play game/ world builder for easy access */
	ListRef topPanelNode;
	ListRef sidePanelNode;
	/* the panel of the grid, used by play game/ world builder for easy access */
	Widget * gridPanel;

} ViewState;

typedef struct ViewState * ViewStateref;

/* a struct for holding the menu data */
typedef struct MenuData{
	/* hold information about cat and mouse from cat/mouse choose/skill menus*/
	int catSkill;
	int mouseSkill;
	int isCatHuman;
	int isMouseHuman;
	/* hold the current selected button for each menu */
	int mainMenuButton;
	int chooseCatButton;
	int chooseMouseButton;
	int catSkillButton;
	int mouseSkillButton;
	int loadGameButton;
	int editGameButton;
	int saveWorldButton;
	int errMsgButton;
	/* for specific stateIds, hold the previous stateId which we came from to them */
	StateId preChooseCat;
	StateId preChooseMouse;
	StateId preWorldBuilder;

	StateId retStateId; /* the next stateId */

	int currValueTemp; /* used fot changing cat/mouse skill or world number values before pressing done */
	int loadGameWorld; /* which game to load in play game */
	int editedWorld; /* which game to edit in world builder */
	gridItemPosition wbCurrPos; /* used for saving the currPos of wb for when we get back from errMsg */
	int missingItems; /* used for passing the missing items from world builder to errr msg */
	int loadFromFile; /* used for telling world builder/play game if game should be loaded from file */
	int isGamePaused; /* used for going to reconfigure cat/mouse and returning to a paused game */

	/* game specific data */
	int isCatFirst;
	int numTurns;
	char ** gameGridData;

} MenuData;

typedef struct MenuData * MenuDataRef;

/* a struct for holding the world builder data */
typedef struct WBData{
	/* hold the non-reusable items positions and the current selscted position */
	gridItemPosition catPos;
	gridItemPosition mousePos;
	gridItemPosition cheesePos;
	gridItemPosition currPos;

	int editedWorld; /* which world file is currently edited */
	char missingItems; /* will hold data about missing items (cat/mouse/cheese) in the grid */

	StateId returnStateId; /* the next stateId */

	/* game specific data */
	int isCatFirst; /* will save the first player in case create game is selected */
	int numTurns; /* will save num turns left from the file */
	char ** gameGridData;

} WBData;

typedef struct WBData * WBDataRef;

/* a struct for holding the play game data */
typedef struct PGData{

	/* hold the cat,mouse,cheese current positions */
	gridItemPosition catPos;
	gridItemPosition mousePos;
	gridItemPosition cheesePos;

	int loadGameWorld; /* the game file tha was loaded */
	int loadFromFile; /* will be used for reconfigure mouse/cat transitions */

	/* information about the players */
	int isCatHuman;
	int isMouseHuman;
	int catSkill;
	int mouseSkill;
	int isCatCurrPlayer;
	/* information about the game */
	int numTurnsLeft;
	int isGameOver;
	gameOverType gameOverType;
	int isGamePaused;
	char ** gameGridData;

	StateId returnStateId; /* the next stateId */


} PGData;

typedef struct PGData * PGDataRef;


/* The GUI structure */
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



/******* functions declaration: *******/
GUI createGUIForState(StateId stateId);

/***** MVP functions: *****/
/* start functions */
void startGeneralMenu(GUIref gui, char * imgPath, int titleImgX, int titleImgY, int titleWidth, int numButtons, int selectedButton, int firstButtonNumOpts, int value);
void startMainMenu(GUIref gui, void* initData);
void startChooseAnimal(GUIref gui, void* initData);
void startAnimalSkill(GUIref gui, void* initData);
void startWorldMenu(GUIref gui, void* initData);
void startWorldBuilder(GUIref gui, void* initData);
void startErrMsg(GUIref gui, void* initData);
void startPlayGame(GUIref gui, void* initData);


/* VTE functions: */
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


/***** MVP helper functions *****/

/** data initialization functions: **/
ViewStateref initGUIViewState();
MenuDataRef initMenuDataToDefault();
void initMenuModel(GUIref gui, void* initData);
void initMainMenuData(GUIref mainMenuGui, void * initData);
void initWorldBuilderModel(GUIref gui, void* initData);
void initPlayGameModel(GUIref gui, void* initData);

/**** general MVP helper functions ****/
char getItemChar(gridItem item);
void saveGridDataToFile(int worldNum, int isCatFirst, char ** gridData);

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
