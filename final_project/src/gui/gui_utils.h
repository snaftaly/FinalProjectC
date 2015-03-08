#ifndef GUI_UTILS_H_
#define GUI_UTILS_H_

/* includes */
#include "../main/CatAndMouse.h"
#include "../main/ListUtilsExt.h"
#include "widget_utils.h"
#include "MVP_utils.h"

#define STATES_COUNT 2
#define POLLING_DELAY 10
#define MAIN_MENU_NUM_BUTTONS 5
#define COMMON_MENU_NUM_BUTTONS 3

#define BUTTON_W 150
#define BUTTON_H 35
#define TITLE_H 40


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

	int loadGameWorld;
	int editedWorld;

	int ** currWorld;

} GameData;

typedef struct GameData * GameDataRef;

/* functions declarations */
int addWidgetToParent(ListRef);
int blitChildToParentWidget(Widget * childWidget, Widget * parentWidget);
int calcAbsWidgetXY(ListRef node);
int isClickEventOnButton(SDL_Event* event, Widget * button);
int changeSelectedButton(Widget * oldButton, Widget * newButton);
GameDataRef initGameDataToDefault();


#endif /* GUI_UTILS_H_ */
