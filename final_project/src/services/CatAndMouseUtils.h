#ifndef CATANDMOUSEUTLS_H_
#define CATANDMOUSEUTLS_H_

/**** includes ****/
#include "gridDataUtils.h"
#include "../main/MiniMaxExt.h"


/**** structs ****/
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

/**** function declarations : ****/

/**** general functions ****/
GUI createGUIForState(StateId stateId);
void consoleMode();
gameOverType checkGameOverType(gridItemPosition catPos, gridItemPosition mousePos,
		gridItemPosition cheesePos, int numTurnsLeft);
int isMoveValid(char ** gridData, gridItemPosition currPlayerPos, gridItemPosition movePos);
char ** initGameDataByFile(int worldNum, int * numTurns, int * isCatFirst);
void saveGameDataToFile(int worldNum, int isCatFirst, char ** gridData);

/**** machine player functions ****/

gridItemPosition suggestMove(GameStateRef state, int maxDepth);
ListRef getChildren(void * data);
GameStateRef createChildState(GameStateRef parentState, gridItemPosition movePos,
		gridItemPosition currPlayerPos);
char ** createChildGrid(GameStateRef parentState, gridItemPosition movePos,
		gridItemPosition currPlayerPos);
int evaluate(void * state);
int ** getDistanceWithBFS (gridItemPosition itemPos, char ** gridData);
gridItemPosition * createPosRef(gridItemPosition movePos);
int ** initDistMatrix();
int ** createIntMatrix(int rows, int cols);
int isPosReachable(gridItemPosition pos, char ** gridData);
int getPosDistance(gridItemPosition pos, int ** distances);
void setPosDistance(gridItemPosition pos, int distance, int ** distances);
void setPosVisited(gridItemPosition pos, char ** gridData);

void freeDistMatrix(int ** matrix);
void freeState(void * data);


#endif /* CATANDMOUSEUTLS_H_ */



