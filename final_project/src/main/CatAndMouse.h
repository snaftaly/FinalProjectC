#ifndef CATANDMOUSE_H_
#define CATANDMOUSE_H_

/**** includes ****/
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_video.h>
#include "../services/ErrorHandling.h"

/**** define constants ****/

/* game related constants */
#define GAMENAME "Cat and Mouse"
#define MOUSE_PLAYER_NAME "mouse"
#define CAT_PLAYER_NAME "cat"
#define STATES_COUNT 11
#define DEFAULT_SKILL 5
#define DEFAULT_WORLD 1
#define DEFAULT_TURNS 20
#define MIN_VALUE 1
#define MAX_SKILL_VALUE 9
#define MAX_WORLD 5
#define ROW_NUM 7
#define COL_NUM 7
#define NUM_DIRECTIONS 4

/* world file constants */
#define WORLD_FILE_NAME_LEN 20
#define WORLD_FILE_PATH "worlds/"
#define WORLD_FILE_NAME_PREFIX "world_"
#define WORLD_FILE_NAME_TYPE "txt"

/* grid chars */
#define EMPTY_CELL_CHAR '#'
#define CAT_CHAR 'C'
#define MOUSE_CHAR 'M'
#define CHEESE_CHAR 'P'
#define WALL_CHAR 'W'
#define VISITED_CHAR 'V' /* used by BFS function */

/* delays constants */
#define POLLING_DELAY 10
#define WARN_DELAY 200
#define MOVE_DELAY 1000

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

/* an enumeration of the different game over types */
typedef enum{
	CAT_WINS,
	MOUSE_WINS,
	TIE,
	GAME_NOT_OVER
} gameOverType;

/* an enumeration of the different arrow directions */
typedef enum{
	UP,
	DOWN,
	RIGHT,
	LEFT
} direction;

/**** structs ****/
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
#endif /* CATANDMOUSE_H_ */
