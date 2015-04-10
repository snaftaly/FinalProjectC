#ifndef MACHINEUTILS_H_
#define MACHINEUTILS_H_

#include "../gui/MVP_utils.h"
#include "ErrorHandling.h"
#include "MiniMax.h"

#define NUM_DIRECTIONS 4

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

void updateMachineMoveIfNeeded(GUI activeGUI);
ListRef getChildren(void * data);
GameStateRef createChildState(GameStateRef parentState, gridItemPosition movePos,
		gridItemPosition currPlayerPos);
char ** createChildGrid(GameStateRef parentState, gridItemPosition movePos,
		gridItemPosition currPlayerPos);
char ** copyGrid(char ** fromGrid);
gridItemPosition suggestMove(GameStateRef state, int maxDepth);
int evaluate(void * state);
void consoleMode();
int ** initDistMatrix();
int ** createIntMatrix(int rows, int cols);
int getPosDistance(gridItemPosition pos, int ** distances);
void setPosDistance(gridItemPosition pos, int distance, int ** distances);
void setPosVisited(gridItemPosition pos, char ** gridData);
int isPosReachable(gridItemPosition pos, char ** gridData);

void freeState(void * data);



#endif //MACHINEUTILS_H_
