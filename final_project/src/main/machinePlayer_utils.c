#include <stdio.h>
#include "machinePlayer_utils.h"


void updateMachineMoveIfNeeded(GUI pgGUI){
	PGDataRef pgModel = pgGUI.model;
	if (isCurrPlayerHuman(pgModel))
		return;
	ViewStateref pgViewState = pgGUI.viewState;
	GameStateRef currState = (GameStateRef)malloc(sizeof(GameState)); /* allocate memory for currState  */
	if (currState == NULL){ /* malloc failed */
		perrorPrint("malloc");
		return;
	}
	int currPlayerSkill = pgModel->isCatCurrPlayer ? pgModel->catSkill : pgModel->mouseSkill;
	currState->gridData = copyGrid(pgModel->gameGridData);
	currState->catPos = pgModel->catPos;
	currState->mousePos = pgModel->mousePos;
	currState->cheesePos = pgModel->cheesePos;
	currState->isCatCurrPlayer = pgModel->isCatCurrPlayer;
	currState->numTurnsLeft = pgModel->numTurnsLeft;
	gridItemPosition errorPos = {-1, -1};
	gridItemPosition movePos = suggestMove(currState, currPlayerSkill);
	if (isSamePos(movePos, errorPos)){ // there was an error while getting the suggested move
		freeState(currState); /* free memory */
		return;
	}
	SDL_Delay(1000);
	makeGameMoveIfLegal(pgViewState, pgModel,movePos);
	freeState(currState); /* free memory */
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
	direction directionArr[] = {UP, RIGHT, DOWN, LEFT};
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
char ** createChildGrid(GameStateRef parentState, gridItemPosition movePos, gridItemPosition currPlayerPos){
	char ** childGrid = copyGrid(parentState->gridData); /* copy parent's grid content into child grid */
	if (childGrid == NULL){ /* failed to initialize board */
		return NULL;
	}
	childGrid[currPlayerPos.row][currPlayerPos.col] = EMPTY_CELL_CHAR;
	childGrid[movePos.row][movePos.col] = parentState-> isCatCurrPlayer ? CAT_CHAR : MOUSE_CHAR;
	return childGrid;
}

/**
 * copyGrid function:
 * gets a grid of size NUM_ROWS*NUM_COLS and copies the content
 * to a newly created grid.
 */
char ** copyGrid(char ** fromGrid){
	char ** toGrid = initGrid(); /* create and initialize child board with zeros */
	if (toGrid == NULL){ /* failed to initialize board */
		return NULL;
	}
	for (int i = 0; i < ROW_NUM; i++){
		for(int j = 0; j < COL_NUM; j++){
			toGrid[i][j] = fromGrid[i][j];
		}
	}
	return toGrid;
}



/**
 * suggestMove function:
 * First, get the index of the best child of the current state's childList,
 * then return the child real direction according to the
 * state's valid moves.
 */
gridItemPosition suggestMove(GameStateRef state, int maxDepth){
	struct MiniMaxResult res = getBestChild(state, maxDepth, getChildren, freeState, evaluate, state->isCatCurrPlayer);
	if (res.index == -2){ /* getBestChild failed */
		gridItemPosition errorPos = {-1, -1};
		return errorPos; /*return error */
	}
	direction directionArr[] = {UP, RIGHT, DOWN, LEFT};
	gridItemPosition movePos = {-1,-1};
	gridItemPosition currPlayerPos = state->isCatCurrPlayer? state->catPos : state->mousePos;
	int j = -1;
	int i;
	for (i=0; i < NUM_DIRECTIONS ; i++){ /* go over each column */
		movePos = getPosByDirection(currPlayerPos, directionArr[i]);
		if (isMoveValid(state->gridData, currPlayerPos, movePos)){
			j++;
		}
		if (j == res.index){ /* check if the number of valid moves passed is equal to best child index */
			break;
		}
	}
	return movePos; /* movePos returned represents the move of the child index relative to the valid moves */
}

/**
 * evaluate function:
 * gets the current state as void * and takes only its board
 * uses evalBoard function to evaluate the board evaluation
 */
int evaluate(void * state){
	GameStateRef currState = state;
	gameOverType gameOverType = checkGameOverType(currState->catPos, currState->mousePos,
			currState->cheesePos, currState->numTurnsLeft);
	if (gameOverType == CAT_WINS)
		return MAX_EVALUATION;
	if (gameOverType == CAT_WINS)
		return MIN_EVALUATION;
	int eval;
	int CMrowDiff = abs(currState->catPos.row-currState->mousePos.row);
	int CMcolDiff = abs(currState->catPos.col-currState->mousePos.col);
	int MProwDiff = abs(currState->mousePos.row-currState->cheesePos.row);
	int MPcolDiff = abs(currState->mousePos.col-currState->cheesePos.col);
	if (currState->isCatCurrPlayer)
		eval = ROW_NUM - CMrowDiff + COL_NUM - CMcolDiff;
	else //mouse is currPlayer
		eval =  -1*(CMrowDiff + CMcolDiff) -1*(ROW_NUM - MProwDiff + COL_NUM - MPcolDiff);
	return eval;
}


/**
 * freeState function:
 * frees the memory allocated to the state's grid data,
 * and then frees the memory allocated to the state itself.
 */
void freeState(void * data){
	GameStateRef gameData = data;
	freeGridData(gameData->gridData);
	free(data);
}
