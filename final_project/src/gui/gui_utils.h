#ifndef GUI_UTILS_H_
#define GUI_UTILS_H_

/* includes */
#include "../main/CatAndMouse.h"
#include "../main/ListUtilsExt.h"
#include "widget_utils.h"

#define STATES_COUNT 2
#define POLLING_DELAY 10
#define MAIN_MENU_NUM_BUTTONS 5
#define COMMON_MENU_NUM_BUTTONS 3
#define FIRST_BUTTON 0
#define REGULAR_BUTTON 0
#define UP_ARROW_BUTTON 1
#define DOWN_ARROW_BUTTON 2

#define MIN_VALUE 1
#define MAX_SKILL_VALUE 9
#define MAX_WORLD 5

#define WINDOW_RED 100
#define WINDOW_GREEN 100
#define WINDOW_BLUE 100

#define PANEL_RED 100
#define PANEL_GREEN 100
#define PANEL_BLUE 100


#define MENU_BUTTON_W 150
#define MENU_BUTTON_H 34
#define ARROW_BUTTON_X 120
#define ARROW_BUTTON_Y 17
#define MENU_TITLE_H 40
#define MENU_TITLE_X_GAP 20
#define MENU_TITLE_Y_GAP 20
#define MENU_BUTTON_Y 80
#define MENU_BUTTON_GAP 10




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
	MARK_AND_SELECT_BUTTON,
	MARK_VALUES_BUTTON,
	INCREASE_VALUE,
	DECREASE_VALUE,
	NO_EVENT  /* is necessary ???? */
} logicalEventType;

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

typedef struct ViewState{
	SDL_Surface * image;
	Widget ** menuButtons;
	ListRef UITree;
	int currButton;
} ViewState;

typedef struct ViewState * ViewStateref;

typedef struct logicalEvent{
	logicalEventType type;
	int buttonNum;
}logicalEvent;

typedef struct logicalEvent * logicalEventRef;

typedef struct GameData{
	int catSkill;
	int mouseSkill;
	int isCatHuman;
	int isMouseHuman;

	int mainMenuButton;
	int chooseCatButton;
	StateId preChooseCat;
	StateId preChooseMouse;
	int chooseMouseButton;
	int catSkillButton;
	int mouseSkillButton;
	int loadGameButton;
	int editGameButton;
	int saveWorldButton;

	int loadGameWorld;
	int editedWorld;
	int saveOnWorld;

	char ** currWorld;

} GameData;
typedef struct GameData * GameDataRef;

/* functions declarations */
int addChildWidgetsToParent(ListRef);
int blitChildToParentWidget(Widget * childWidget, Widget * parentWidget);
int calcAbsWidgetXY(ListRef node);
int isClickEventOnButton(SDL_Event* event, Widget * button, int buttonType);
int isClickEventOnSpecificButton(SDL_Event* event, Widget * button, int Xdelta, int YdeltaUp, int YdeltaDown);
int changeSelectedButton(Widget * oldButton, Widget * newButton);
void setValuesButtonFromInit(int value, Widget* valuesButton);
void increaseValuesButton(int* currValue, int maxValue, Widget* valuesButton);
void decreaseValuesButton(int* currValue, int maxValue, Widget* valuesButton);
GameDataRef initGameDataToDefault();
int calcPanelX(int titleWidth);
int calcPanelY(int numButtons);
int calcPanelWidth(int titleWidth);
int calcPanelHeight(int numButtons);
int calcMenuButtonX(int titleWidth);
int calcMenuButtonY();


#endif /* GUI_UTILS_H_ */
