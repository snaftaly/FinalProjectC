#ifndef GUI_UTILS_H_
#define GUI_UTILS_H_

/* includes */
#include "../main/CatAndMouse.h"
#include "../main/ListUtilsExt.h"
#include "widget_utils.h"

/* define constants: */
#define MAIN_MENU_NUM_BUTTONS 5
#define COMMON_MENU_NUM_BUTTONS 3
#define ERR_MSG_NUM_BUTTONS 1
#define WB_NUM_BUTTONS 8
#define WB_TOP_PANEL_NUM_BUTTONS 3
#define WB_SIDE_PANEL_NUM_BUTTONS 5
#define PG_NUM_BUTTONS 6
#define PG_TOP_PANEL_NUM_BUTTONS 1
#define PG_SIDE_PANEL_NUM_BUTTONS 5
#define PG_NUM_LABELS 4
#define NUM_GRID_ITEMS 8

#define FIRST_BUTTON 0
#define REGULAR_BUTTON 0
#define UP_ARROW_BUTTON 1
#define DOWN_ARROW_BUTTON 2

#define MENU_BUTTON_W 260
#define MENU_BUTTON_H 60
#define ARROW_BUTTON_X 220
#define ARROW_BUTTON_Y 30
#define MENU_TITLE_W 460
#define MENU_TITLE_H 80
#define MENU_BUTTON_GAP 20
#define ERR_MSG_TITLE_W 500
#define MISSING_ITEM_MSG_W 440
#define MENU_TITLE_X_GAP 1
#define MENU_TITLE_Y_GAP 20

#define TOP_PANEL_H 200
#define SIDE_PANEL_W 200
#define SIDE_PANEL_H 590
#define PANEL_BUTTON_W 176
#define PANEL_BUTTON_H 80
#define PANEL_WIDGET_Y_GAP 20

#define GRID_SIZE 590
#define GRID_SQUARE_SIZE 80
#define GRID_GAP_SIZE 5

#define WB_TITLE_W 240
#define WB_TITLE_H 60
#define WB_BUTTON_X_GAP 50

#define GAME_OVER_LABEL_W 424
#define GAME_OVER_LABEL_H 100
#define GAME_INFO_LABEL_H 40
#define GAME_INFO_LABEL_W 180
#define STATE_LABEL_W 500
#define STATE_LABEL_H 40
#define DIGIT_LABEL_W 30
#define CAT_MOUSE_DIF 35
#define PAUSE_BUTTON_H 50
#define PAUSE_BUTTON_W 400

#define WINDOW_RED 100
#define WINDOW_GREEN 100
#define WINDOW_BLUE 100

#define PANEL_RED 215
#define PANEL_GREEN 225
#define PANEL_BLUE 255

#define TR_RED  0
#define TR_GREEN 255
#define TR_BLUE 0



/* an enumeration of all the different states of the program.
/ each state should correspond to a specific GUI.*/
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

typedef enum{
	UP,
	DOWN,
	RIGHT,
	LEFT
} direction;

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

typedef enum{
	CAT_WINS,
	MOUSE_WINS,
	TIE,
	GAME_NOT_OVER
} gameOverType;

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

typedef struct gridItemPosition{
	int row;
	int col;
} gridItemPosition;

typedef struct ViewState{
	SDL_Surface * image;
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

typedef struct logicalEvent{
	logicalEventType type;
	int buttonNum;
	gridItemPosition gridPos;
}logicalEvent;

typedef struct logicalEvent * logicalEventRef;

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



/* functions declarations */
void drawGui(ViewStateref);
int addChildWidgetsToParent(ListRef);
int blitChildToParentWidget(Widget * childWidget, Widget * parentWidget);
int calcAbsWidgetXY(ListRef node);
int isClickEventOnButton(SDL_Event* event, Widget * button, int buttonType);
int isClickEventOnSpecificButton(SDL_Event* event, Widget * button, int Xdelta, int YdeltaUp, int YdeltaDown);
int changeSelectedButton(Widget * oldButton, Widget * newButton);
void setValuesButtonFromInit(int value, Widget* valuesButton);
void increaseValuesButton(int* currValue, int maxValue, Widget* valuesButton);
void decreaseValuesButton(int* currValue, int maxValue, Widget* valuesButton);
int calcPanelX(int titleWidth);
int calcPanelY(int numButtons);
int calcPanelWidth(int titleWidth);
int calcPanelHeight(int numButtons);
int calcMenuButtonX(int titleWidth);
int calcMenuButtonY();
int calcTopButtonX();
int calcTopButtonY();
int calcSideButtonX();
int calcSideButtonY();
int calcWBtitleX(int title_W);
int calcErrPanelY();
int calcErrPanelHeight();
int calcMisItemMsgX(int messageWidth, int titleWidth);
int calcMisItemMsgY();
int calcMenuButtonX(int titleWidth);
int calcErrMsgButtonY();
int calcGameOverLabelX();
int calcGameOverLabelY();
int calcPauseButtonX();
int calcPauseButtonY();
int calcMoveLabelX(int isCatCurrPlayer);
int calcTensLabelX(int isCatCurrPlayer);
int calcUnitsLabelX(int isCatCurrPlayer);
int calcStateLabelX();
int calcStateLabelY();
void blitUpToWindow(void * widget);



#endif /* GUI_UTILS_H_ */
