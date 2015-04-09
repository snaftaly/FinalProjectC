#include <stdio.h>
#include "machinePlayer_utils.h"


void updateMachineMoveIfNeeded(GUI pgGUI){
	PGDataRef pgModel = pgGUI.model;
	if (isCurrPlayerHuman(pgModel) || pgModel->isGameOver || pgModel->isGamePaused)
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
	childState->catPos = parentState->isCatCurrPlayer ? movePos : parentState->catPos;
	childState->mousePos = parentState->isCatCurrPlayer ? parentState->mousePos : movePos;
	childState->cheesePos = parentState->cheesePos;

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
	gridItemPosition currPlayerPos = state->isCatCurrPlayer ? state->catPos : state->mousePos;
	int j = -1;
	int i;
	for (i = 0; i < NUM_DIRECTIONS ; i++){ /* go over each column */
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

int ** getDistanceWithBFS (gridItemPosition * itemPos, char ** gridData){
	int ** distMatrix = initDistMatrix();
	if (distMatrix == NULL)
		return NULL;
	char ** copiedGrid = copyGrid(gridData);
	if (copiedGrid == NULL)
		return NULL;
	setPosDistance(*itemPos, 0, distMatrix);
	ListRef queueItemsList = newList(itemPos);
	if (queueItemsList == NULL)
		return NULL;
	ListRef realQueue = queueItemsList;
	ListRef queueTial = realQueue;
	setPosVisited(*itemPos, copiedGrid);
	direction directionArray[] = {UP, DOWN, RIGHT, LEFT};
	gridItemPosition adjPos;

	while (realQueue != NULL){
		gridItemPosition * currPos = headData(realQueue);
		for (int i = 0; i < NUM_DIRECTIONS; i++){
			adjPos = getPosByDirection(currPos, directionArray[i]);
			if (!isAdjPos(*currPos, adjPos) && isPosReachable(adjPos, copiedGrid)){
				setPosDistance(adjPos, getPosDistance(*currPos, distMatrix) + 1, distMatrix);
				if (isGridPosFree(adjPos, copiedGrid)){
					queueTial = append(queueTial, &adjPos);
					if (queueTial == NULL){
						destroyList(queueItemsList, free);
						freeGridData(copiedGrid);
						return NULL;
					}
				}
				setPosVisited(adjPos, copiedGrid);
			}
		}
		realQueue = tail(realQueue);
	}
	destroyList(queueItemsList, free);
	freeGridData(copiedGrid);
	return distMatrix;
}

/**
 * initDistMatrix function:
 * allocates memory for the distance matrix (rows and columns).
 * return an integer matrix of size ROW_NUM*COL_NUM filled with a number
 * that is higher than the largest possible distance between two items in the grid.
 */
int ** initDistMatrix(){
	int ** distMatrix = createIntMatrix();
	if (distMatrix == NULL)
		return NULL;
	for (int i = 0; i < ROW_NUM ; i++)
		for (int  j = 0; j<COL_NUM; j++)
			distMatrix[i][j] = ROW_NUM*ROW_NUM*ROW_NUM;
	return distMatrix;
}

/**
 * createIntMatrix function:
 * allocates memory for the distance matrix (rows and columns).
 * return an integer matrix of size ROW_NUM*COL_NUM
 */
int ** createIntMatrix(){
	int ** intMatrix = (int **)malloc(ROW_NUM*sizeof(int *)); /* allocate memory for the rows */
	if (intMatrix == NULL){ /* malloc failed */
		perrorPrint("malloc");
		return NULL;
	}
	for (int i = 0; i < ROW_NUM; i++){
		intMatrix[i] = (int *)malloc(COL_NUM*sizeof(int)); /* allocate memory for the columns of each row */
		if (intMatrix[i] == NULL){ /* malloc failed for this row */
			perrorPrint("malloc");
			for (int j=0; j < i ; j++){ /* free memory of each previous row's columns*/
				free(intMatrix[j]);
			}
			free(intMatrix); /* free memory of allocated for the rows */
			return NULL;
		}
	}
	return intMatrix; /* return the intMatrix */
}

int getPosDistance(gridItemPosition pos, int ** distances){
	return distances[pos.row][pos.col];
}

void setPosDistance(gridItemPosition pos, int distance, int ** distances){
	distances[pos.row][pos.col] = distance;
}

void setPosVisited(gridItemPosition pos, char ** gridData){
	gridData[pos.row][pos.col] = VISITED_CHAR;
}

int isPosReachable(gridItemPosition pos, char ** gridData){
	if (gridData[pos.row][pos.col] == WALL_CHAR || gridData[pos.row][pos.col] == VISITED_CHAR)
		return 0;
	return 1;
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




void consoleMode(){
	int isCatCurrPlayer, numTurnsLeft;
	gridItemPosition catPos, mousePos, cheesePos;
	char ** gridData = initGameDataByFile(-1, &numTurnsLeft, &isCatCurrPlayer);
	if (gridData == NULL)
		return;
	updateItemsPositions(&mousePos,&catPos,&cheesePos, gridData);

	GameStateRef currState = (GameStateRef)malloc(sizeof(GameState)); /* allocate memory for currState  */
	if (currState == NULL){ /* malloc failed */
		perrorPrint("malloc");
		freeGridData(gridData);
		return;
	}
	currState->gridData = gridData;
	currState->catPos = catPos;
	currState->mousePos = mousePos;
	currState->cheesePos = cheesePos;
	currState->isCatCurrPlayer = isCatCurrPlayer;
	currState->numTurnsLeft = numTurnsLeft;
	int eval = evaluate(currState);
	printf("%d\n",eval);
	// what should we do now????

}
