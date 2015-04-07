#include <stdio.h>
#include "machinePlayer_utils.h"


void updateMachineMoveIfNeeded(GUI activeGUI){
//fill this functions
}


/*
 * getChildren function:
 * gets a state and return a List containing all possible
 * valid children (valid moves) states
 */
ListRef getChildren(void * data){
	GameStateRef currState = data;

	ListRef childsList = newList(NULL); /* create an empty childs list */
	if (childsList == NULL){ /* failed to create childList */
		return NULL;
	}
	int eval = evaluate(data); /* evaluate the current state */
	if (currState->numTurnsLeft == 0 || eval == MAX_EVALUATION || eval == MIN_EVALUATION){ /* if currState is a leaf state */
		return childsList; /* return empty childsList */
	}

	ListRef tempList = childsList; /* use a tempList pointer to add the next child into */
	gridItemPosition currPlayerPos = currState->isCatCurrPlayer? currState->catPos : currState->mousePos;
	gridItemPosition movePos = {-1,-1};
	logicalEventType directionArr[] = {GO_UP, GO_RIGHT, GO_DOWN, GO_LEFT};
	for (int i = 0; i < NUM_DIRECTIONS; i++){
		movePos = getPosByDirection(currPlayerPos, directionArr[i]);
		if (isMoveValid(currState->gridData, currPlayerPos, movePos)){
			GameStateRef childState = createChildState(currState, movePos, currPlayerPos); /* create the child's state */
			if (childState == NULL){ /* failed to create child's state */
				destroyList(childsList, freeState); /* free the list created so far */
				return NULL;
			}
			tempList = append(tempList, childState); /* append a new item to the list containing childState */
			if (tempList == NULL){ /* failed to append child */
				destroyList(childsList, freeState); /* free the list created so far */
				return NULL;
			}
		}
	}
	return childsList;
}

/**
 * createChildState function:
 * create a child's state according to its parent state, the move position and
 * the current player position.
 */
GameStateRef createChildState(GameStateRef parentState, gridItemPosition movePos, gridItemPosition currPlayerPos){

	GameStateRef childState = (GameStateRef)malloc(sizeof(GameState)); /* allocate memory for childState  */
	if (childState == NULL){ /* malloc failed */
		perrorPrint("malloc");
		return NULL;
	}
	char ** childGrid = createChildGrid(parentState, movePos, currPlayerPos); /* create the child board */
	if (childGrid == NULL){ /* failed to create the child board */
		free(childState);
		return NULL;
	}
	/* set the childState fields: */
	childState->gridData = childGrid;
	childState->isCatCurrPlayer = 1-parentState->isCatCurrPlayer;
	childState->numTurnsLeft = parentState->numTurnsLeft-1;
	childState->catPos = parentState->isCatCurrPlayer ? movePos : childState->catPos;
	childState->mousePos = parentState->isCatCurrPlayer ? childState->mousePos : movePos;

	return childState;
}

/**
 * createChildGrid function:
 * create a child grid according to its parent state and to the column
 * its disc should be added.
 */
char ** createChildGrid(GameState parentState, gridItemPosition movePos, gridItemPosition currPlayerPos){
	char ** childGrid = initGrid(); /* create and initialize child board with zeros */
	if (childGrid == NULL){ /* failed to initialize board */
		return NULL;
	}
	copyGrid(parentState->gridData, childGrid); /* copy parent's board content into childBoard */
	childGrid[currPlayerPos.row][currPlayerPos.col] = EMPTY_CELL_CHAR;
	childGrid[movePos.row][movePos.col] = parentState-> isCatCurrPlayer ? CAT_CHAR : MOUSE_CHAR;
	return childGrid;
}

/**
 * copyGrid function:
 * gets two integer matrices of size NUM_ROWS*NUM_COLS and copy the content
 * from one to another.
 */
void copyGrid(char ** fromGrid, char ** toGrid){
	for (int i = 0; i < ROW_NUM; i++){
		for(int j = 0; j < COL_NUM; j++){
			toGrid[i][j] = fromGrid[i][j];
		}
	}
}



/**
 * suggestMove function:
 * First, get the index of the best child of the current state's
 * childList, then return the child real column according to the
 * state's valid moves.
 */
int suggestMove(GameStateRef state, int maxDepth){
	struct MiniMaxResult res = getBestChild(state, maxDepth, getChildren, freeState, evaluate, state->isMaxPlayer);
	if (res.index == -2){ /* getBestChild failed */
		return -2; /*return error */
	}
	int ** board = state->board; /* get the game board of the current state */
	int j = -1;
	int i;
	for (i=0; i < NUMCOLS ; i++){ /* go over each column */
		if (!isColumnFull(board,i)){ /* if the column is not full then it is a valid move */
			j++;
		}
		if (j == res.index){ /* check if the number of valid moves passes is equal to best child index */
			break;
		}
	}
	return i; /* i is the column that represents the place of the child index relative to the valid moves */
}

/**
 * freeState function:
 * frees the memory allocated to the state's grid data,
 * and then frees the memory allocated to the state itself.
 */
void freeState(void * data){
	GameStateRef gameData = data;
	freeGridData(gameData->gridData);
	free(gameData);
}
