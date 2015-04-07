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
	char ** gridData;
	int isCatCurrPlayer;
	int numTurnsLeft;
	gridItemPosition catPos;
	gridItemPosition mousePos;
} GameState;

typedef struct GameState * GameStateRef; /* a pointer to struct GameState */

void updateMachineMoveIfNeeded(GUI activeGUI);


#endif //MACHINEUTILS_H_
