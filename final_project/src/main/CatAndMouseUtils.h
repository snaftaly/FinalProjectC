#ifndef CATANDMOUSE_H_
#define CATANDMOUSE_H_

#include "../gui/gui_utils.h"
#include "ErrorHandling.h"
#include "MiniMax.h"

/**** define constants ****/
/* game related constants */
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
#define EMPTY_CELL_CHAR '#'
#define CAT_CHAR 'C'
#define MOUSE_CHAR 'M'
#define CHEESE_CHAR 'P'
#define WALL_CHAR 'W'
#define VISITED_CHAR 'V'
/* delays constants */
#define POLLING_DELAY 10
#define WARN_DELAY 200

/**** enums ****/

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

/**** structs: ****/

/* a struct that holds a grid Item position -
 * row num and col num, where the top left corner is (0,0)
 */
typedef struct gridItemPosition{
	int row;
	int col;
} gridItemPosition;

/*
 * struct GameState is used to represent a game state.
 * It holds following information:
 * - the state's game grid data
 * - who is the current player (cat or mouse)
 * - how many turns are left
 * - current positions of cat and mouse
 */
typedef struct GameState{
	int isCatCurrPlayer;
	int numTurnsLeft;
	gridItemPosition catPos;
	gridItemPosition mousePos;
	gridItemPosition cheesePos;
	char ** gridData;
} GameState;

typedef struct GameState * GameStateRef; /* a pointer to struct GameState */


/* function declarations : */
void consoleMode();

ListRef getChildren(void * data);
GameStateRef createChildState(GameStateRef parentState, gridItemPosition movePos,
		gridItemPosition currPlayerPos);
char ** createChildGrid(GameStateRef parentState, gridItemPosition movePos,
		gridItemPosition currPlayerPos);
char ** copyGrid(char ** fromGrid);
gridItemPosition suggestMove(GameStateRef state, int maxDepth);
int evaluate(void * state);
int ** initDistMatrix();
int ** createIntMatrix(int rows, int cols);
int getPosDistance(gridItemPosition pos, int ** distances);
void setPosDistance(gridItemPosition pos, int distance, int ** distances);
void setPosVisited(gridItemPosition pos, char ** gridData);
int isPosReachable(gridItemPosition pos, char ** gridData);
int isAdjPos(gridItemPosition pos1, gridItemPosition pos2);
int isGridPosFree(gridItemPosition gridPos, char ** gridData);
char ** initGrid();
gridItemPosition getPosByDirection(gridItemPosition currPos, direction direction);
int isMoveValid(char ** gridData, gridItemPosition currPlayerPos, gridItemPosition movePos);
gameOverType checkGameOverType(gridItemPosition catPos, gridItemPosition mousePos,
		gridItemPosition cheesePos, int numTurnsLeft);
void updateItemsPositions(gridItemPosition * mousePos,gridItemPosition * catPos,gridItemPosition * cheesePos,
		char ** gameGridData);
void freeState(void * data);
void freeGridData(char ** gridData);
char ** initGameDataByFile(int worldNum, int * numTurns, int * isCatFirst);
void setEmptyGrid(char ** grid);
int isSamePos(gridItemPosition pos1, gridItemPosition pos2);
void freeDistMatrix(int ** matrix);
gridItemPosition * createPosRef(gridItemPosition movePos);
int ** getDistanceWithBFS (gridItemPosition itemPos, char ** gridData);



#endif /* CATANDMOUSE_H_ */



