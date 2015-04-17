#include <stdio.h>
#include "CatAndMouseUtils.h"

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
	if (isError)
		return NULL;
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
	/* update the child grid positions according to the move posistion */
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
	/* go over each cell and copy its contents */
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
	/* run getBestChild to get the best move index and value */
	struct MiniMaxResult res = getBestChild(state, maxDepth, getChildren, freeState, evaluate, state->isCatCurrPlayer);
	if (res.index == -2){ /* getBestChild failed */
		gridItemPosition errorPos = {-1, -1};
		return errorPos; /*return error */
	}
	direction directionArr[] = {UP, RIGHT, DOWN, LEFT}; /* possible moves */
	gridItemPosition movePos = {-1,-1}; /* init move pos */
	gridItemPosition currPlayerPos = state->isCatCurrPlayer ? state->catPos : state->mousePos;	/* get curr player position */

	/* go over all possible moves, find which ones are valid for the current player position,
	 * and find which one matches the index */
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

//	GameStateRef stateRef = state;
//	int ** dist = getDistanceWithBFS(stateRef->catPos, stateRef->gridData);
//	int catFromMouse = dist[stateRef->mousePos.row][stateRef->mousePos.col];
//	printf("cat from mouse: %d!!!!!!!!!!!!!!!!!!!!!!\n", catFromMouse);


	GameStateRef currState = state;
	gameOverType gameOverType = checkGameOverType(currState->catPos, currState->mousePos,
				currState->cheesePos, currState->numTurnsLeft);
	if (gameOverType == CAT_WINS)
		return MAX_EVALUATION;
	if (gameOverType == CAT_WINS)
		return MIN_EVALUATION;

	int ** distCat = getDistanceWithBFS(currState->catPos, currState->gridData);
	if (isError) { return 300000; }
	int ** distMouse = getDistanceWithBFS(currState->mousePos, currState->gridData);
	if (isError){
		freeDistMatrix(distCat);
		return 300000;
	}
	int catFromMouse = distCat[currState->mousePos.row][currState->mousePos.col];
	int catFromCheese = distCat[currState->cheesePos.row][currState->cheesePos.col];
	int mouseFromCheese = distMouse[currState->cheesePos.row][currState->cheesePos.col];
	int eval;
	int CMrowDiff = abs(currState->catPos.row-currState->mousePos.row);
	int CMcolDiff = abs(currState->catPos.col-currState->mousePos.col);
	int MProwDiff = abs(currState->mousePos.row-currState->cheesePos.row);
	int MPcolDiff = abs(currState->mousePos.col-currState->cheesePos.col);
	if (currState->isCatCurrPlayer == 0){
		//eval = ROW_NUM - CMrowDiff + COL_NUM - CMcolDiff;
		if (catFromCheese + 3 > mouseFromCheese){
			eval = (20-catFromCheese)*100 + (20-catFromMouse);
//			printf("ifcat %d\n", eval);
//			printf("catrow: %d, catcol: %d, mouserow: %d, mousecol: %d, catfrommouse: %d, mousecheese: %d\n",
//					currState->catPos.row, currState->catPos.col, currState->mousePos.row,
//					currState->mousePos.col, catFromMouse, mouseFromCheese);

		}
		else{
			eval = (20-catFromCheese) + (20-catFromMouse)*100 + (20-2*CMrowDiff);
//			printf("elsecat %d\n", eval);
//			printf("catrow: %d, catcol: %d, mouserow: %d, mousecol: %d, catfrommouse: %d, mousecheese: %d\n",
//				currState->catPos.row, currState->catPos.col, currState->mousePos.row,
//				currState->mousePos.col, catFromMouse, mouseFromCheese);

		}
	}
	else{ //mouse is currPlayer
		//eval =  -1*(CMrowDiff + CMcolDiff) -1*(ROW_NUM - MProwDiff + COL_NUM - MPcolDiff);
		if (catFromMouse < 3){
			eval = (-1) * (20-mouseFromCheese) - (catFromMouse)*100;
//			printf("iscatcurr: %d\n", currState->isCatCurrPlayer);
//			if (currState->isCatCurrPlayer){
//			printf("if %d\n", eval);
//			printf("catrow: %d, catcol: %d, mouserow: %d, mousecol: %d, catfrommouse: %d, mousecheese: %d\n",
//					currState->catPos.row, currState->catPos.col, currState->mousePos.row,
//					currState->mousePos.col, catFromMouse, mouseFromCheese);
//			}
		}
		else{
			eval = (-1) * (20-mouseFromCheese)*100 - (catFromMouse);
//			printf("iscatcurr: %d\n", currState->isCatCurrPlayer);
//			if (currState->isCatCurrPlayer){
//			printf("else %d\n", eval);
//			printf("catrow: %d, catcol: %d, mouserow: %d, mousecol: %d, catfrommouse: %d, mousecheese: %d\n",
//				currState->catPos.row, currState->catPos.col, currState->mousePos.row,
//				currState->mousePos.col, catFromMouse, mouseFromCheese);
//			}
		}
	}
	freeDistMatrix(distCat);
	freeDistMatrix(distMouse);
	return eval;
}

/* getDistanceWithBFS gets the distances (by number of legal game moves), between the current position and
 * all the available grid positions. it returns the distance Matrix */
int ** getDistanceWithBFS (gridItemPosition itemPos, char ** gridData){
	int ** distMatrix = initDistMatrix(); /* initialize the distance matrix */
	if (distMatrix == NULL)
		return NULL;
	char ** copiedGrid = copyGrid(gridData); /* copy the grid, so we can mark on it moves that we already made  */
	if (copiedGrid == NULL){
		freeDistMatrix(distMatrix);
		return NULL;
	}
	setPosDistance(itemPos, 0, distMatrix); /* set the distance from current location to itself 0 */
	setPosVisited(itemPos, copiedGrid); /* set the position as visited */
	gridItemPosition * addPos = createPosRef(itemPos);
	if (addPos == NULL){
		freeGridData(copiedGrid);
		freeDistMatrix(distMatrix);
		return NULL;
	}
	ListRef queue = newList(addPos); /*create a queue and add addPos to it */
	if (queue == NULL){
		freeGridData(copiedGrid);
		freeDistMatrix(distMatrix);
		free(addPos);
		return NULL;
	}
	ListRef queueTail = queue; /* add a pointer to the queue tail */
	direction directionArray[] = {UP, RIGHT, DOWN ,LEFT}; /* an array of possible directions */
	gridItemPosition movePos;

	while (queue != NULL){
		gridItemPosition * currPos = headData(queue); /* get position in queue head */
		for (int i = 0; i < NUM_DIRECTIONS; i++){ /* go over each direction */
			movePos = getPosByDirection(*currPos, directionArray[i]); /* get the grid pos of the direction */
			if (!isSamePos(*currPos, movePos) && isPosReachable(movePos, copiedGrid)){ /* if the pos is valid */
				setPosDistance(movePos, getPosDistance(*currPos, distMatrix) + 1, distMatrix); /* update pos ditance in matrix */
				if (isGridPosFree(movePos, copiedGrid)){ /* if the pos is free of items and not visited alreay */
					addPos = createPosRef(movePos); /* create a new position item */
					if (addPos == NULL){ /* if there was an error free the data */
						freeDistMatrix(distMatrix);
						freeGridData(copiedGrid);
						destroyList(queue, free);
						return NULL;
					}
					queueTail = append(queueTail, addPos); /* append the addPos to the queue, and update the tail*/
					if (queueTail == NULL){ /* if there was an error free the data */
						freeDistMatrix(distMatrix);
						destroyList(queue, free);
						freeGridData(copiedGrid);
						return NULL;
					}
				}
				setPosVisited(movePos, copiedGrid);
			}
		}
		ListRef headNode = queue;
		queue = tail(queue);
		freeNode(headNode, free);
	}
	freeGridData(copiedGrid);
	return distMatrix;
}
/* allocate memory for gridItemPosition and copy the attributes from the argument passed */
gridItemPosition * createPosRef(gridItemPosition movePos){
	gridItemPosition * addPos = malloc(sizeof(gridItemPosition)); /* allocate memory */
	if (addPos == NULL)
		return NULL;
	/* copy attributes */
	addPos->row = movePos.row;
	addPos->col = movePos.col;
	return addPos;
}

/**
 * initDistMatrix function:
 * allocates memory for the distance matrix (rows and columns).
 * return an integer matrix of size ROW_NUM*COL_NUM filled with a non legal
 * value of distance (a negative value)
 */
int ** initDistMatrix(){
	int ** distMatrix = createIntMatrix(ROW_NUM, COL_NUM); /* initialize an int matrix */
	if (distMatrix == NULL)
		return NULL;
	/* fill all positions with -1 */
	for (int i = 0; i < ROW_NUM ; i++)
		for (int  j = 0; j<COL_NUM; j++)
			distMatrix[i][j] = -1;
	return distMatrix;
}

/**
 * createIntMatrix function:
 * allocates memory for the distance matrix (rows and columns).
 * return an integer matrix of size ROW_NUM*COL_NUM
 */
int ** createIntMatrix(int rows, int cols){
	int ** intMatrix = (int **)malloc(rows*sizeof(int *)); /* allocate memory for the rows */
	if (intMatrix == NULL){ /* malloc failed */
		perrorPrint("malloc");
		return NULL;
	}
	for (int i = 0; i < rows; i++){
		intMatrix[i] = (int *)malloc(cols*sizeof(int)); /* allocate memory for the columns of each row */
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

/* get the distance value of a specific position in the grid from distances matrix */
int getPosDistance(gridItemPosition pos, int ** distances){
	return distances[pos.row][pos.col];
}

/* set the distance value of a specific position in the grid in distances matrix*/
void setPosDistance(gridItemPosition pos, int distance, int ** distances){
	distances[pos.row][pos.col] = distance;
}

/* set the position visited in the gridData, using the visited char */
void setPosVisited(gridItemPosition pos, char ** gridData){
	gridData[pos.row][pos.col] = VISITED_CHAR;
}

/* return 0 if the grid positions contains the WALL_CHAR or VISITED_CHAR, otherwise return 1 */
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

/* run console mode */
void consoleMode(){
	/*set the variables */
	int isCatCurrPlayer, numTurnsLeft;
	gridItemPosition catPos, mousePos, cheesePos;
	char ** gridData = NULL;
	GameStateRef currState = NULL;
	int doExit = 0;
	char endChar;

	while(!doExit){
		/* get grid data, numturns and current player from stdin */
		gridData = initGameDataByFile(-1, &numTurnsLeft, &isCatCurrPlayer);
		if (gridData == NULL)
			return;
		updateItemsPositions(&mousePos,&catPos,&cheesePos, gridData); /*update the items positions by the grid data */

		/* create a state and put the appropriate data in its fields */
		currState = (GameStateRef)malloc(sizeof(GameState)); /* allocate memory for currState  */
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
		if (isError){
			freeState(currState); /* free the current state including the grid data */
			return;
		}
		printf("%d\n",eval);
		endChar = getchar();
		if (endChar == 'q')
			doExit = 1;
		freeState(currState); /* free the current state including the grid data */
	}
}

/* return the type of the game over by items positions,
 * if the game is not over return GAME_NOT_OVER */
gameOverType checkGameOverType(gridItemPosition catPos, gridItemPosition mousePos,
		gridItemPosition cheesePos, int numTurnsLeft){

	if (isAdjPos(catPos, mousePos)){ /* if cat and mouse are adjacent - cat wins */
		return CAT_WINS;
	}
	if (isAdjPos(cheesePos, mousePos)){ /* else if mouse and chees are adjacent - mouse wins */
		return MOUSE_WINS;
	}
	if (numTurnsLeft==0){ /* else if no turns are left - it's a tie */
		return TIE;
	}
	return GAME_NOT_OVER; /* else - game is not over */
}

/* check of two grid positions are adjacent */
/* if they have the same col, and the rows are in difference of 1
 * or if they have the same row, and the cols are in difference of 1
 * return 1, else return 0 */
int isAdjPos(gridItemPosition pos1, gridItemPosition pos2){

	if ((pos1.col == pos2.col && abs(pos1.row - pos2.row) == 1) ||
			(pos1.row == pos2.row && abs(pos1.col - pos2.col) == 1))
		return 1;
	return 0;
}

/* return 1 if the grid position have the empth cell char, else return 0 */
int isGridPosFree(gridItemPosition gridPos, char ** gridData){
	if (gridData[gridPos.row][gridPos.col] == EMPTY_CELL_CHAR)
		return 1;
	return 0;
}

/* allocate memory for the grid */
char ** initGrid(){
	char ** grid = (char **)malloc(ROW_NUM*sizeof(char *)); /* allocate memory for the rows */
	if (grid == NULL){
		perrorPrint("malloc");
		return NULL;
	}
    for (int i = 0; i < ROW_NUM ; i++){ /* allocate memory for each col */
    	grid[i] = (char *)malloc(COL_NUM*sizeof(char));
    	if (grid[i] == NULL){ /* if there was an error - free the cols added so far and free the rows */
    		perrorPrint("malloc");
    		for (int j = 0; j < i; j++)
    			free(grid[j]);
    		free(grid);
    		return NULL;
    	}
    }
	return grid;
}

/* get the position of the location according to the direction from the current position
 * if the new position is outside the grid borders - return the current position */
gridItemPosition getPosByDirection(gridItemPosition currPos, direction direction){
	switch(direction){
		case(UP):
			if (currPos.row > 0)
				currPos.row -= 1;
			break;
		case(DOWN):
			if (currPos.row < ROW_NUM-1)
				currPos.row += 1;
			break;
		case(LEFT):
			if (currPos.col > 0)
				currPos.col -= 1;
			break;
		case(RIGHT):
			if (currPos.col < COL_NUM-1)
				currPos.col += 1;
			break;
		default:
			return currPos;
	}
	return currPos;
}

/* return 1 if the move is adjacent to the player position and the grid position is free */
int isMoveValid(char ** gridData, gridItemPosition currPlayerPos, gridItemPosition movePos){
	if (isAdjPos(currPlayerPos, movePos) && isGridPosFree(movePos, gridData))
		return 1;
	return 0;
}

/* free memory allocated for the grid */
void freeGridData(char ** gridData){
	if (gridData != NULL){
		for (int i = 0; i < ROW_NUM; i++) /* free the cols */
			free(gridData[i]);
		free(gridData); /* free the rows */
	}
}

/* update the cat, cheese and mouse positions */
void updateItemsPositions(gridItemPosition * mousePosRef,gridItemPosition * catPosRef, gridItemPosition * cheesePosRef,
		char ** gameGridData){
	/* first set the positions to a non valid position (out of the grid)*/
	gridItemPosition catPos = {-1, -1};
	gridItemPosition mousePos = {-1, -1};
	gridItemPosition cheesePos = {-1, -1};
	*catPosRef = catPos;
	*mousePosRef = mousePos;
	*cheesePosRef = cheesePos;
	/* go over each position in the gris, and if one of the items is there - update its position */
	for (int i = 0 ; i < ROW_NUM ;i++){
		for (int j = 0; j < COL_NUM; j++){
			char currItemChar = gameGridData[i][j];
			if (currItemChar == MOUSE_CHAR){
				mousePosRef->row = i;
				mousePosRef->col = j;
			}
			else if (currItemChar == CAT_CHAR){
				catPosRef->row = i;
				catPosRef->col = j;
			}
			else if (currItemChar == CHEESE_CHAR){
				cheesePosRef->row = i;
				cheesePosRef->col = j;
			}
		}
	}
}

/* initialize game data by file - reads the file and updated the grid, num turns and isCatFirst
 * if worldnum is > 0, reads from the file with the appropriate file name
 * if worldnum is = 0, created an empty grid, sets num turns to DEFAULT_TURNS
 * if worldnum is -1  reads the data from stdin */
char ** initGameDataByFile(int worldNum, int * numTurns, int * isCatFirst){
	char ** grid = initGrid(); /* initialize the grid */
	if (isError)
		return NULL;
	*numTurns = DEFAULT_TURNS; /* set num turns to DEFAULT_TURNS */
	char firstAnimal[6];
	if (worldNum == 0){ /* if world num is 0, create an empty grid */
		setEmptyGrid(grid);
	}
	else{ /* world num != 0 */
		FILE * worldFile = NULL;
		if (worldNum == -1){ /* read from standard input */
			worldFile = stdin;
		}
		else { /* read from appropriate world file */
			/* create the file name */
			char filename[WORLD_FILE_NAME_LEN];
			if (sprintf(filename, "%s%s%d.%s", WORLD_FILE_PATH, WORLD_FILE_NAME_PREFIX, worldNum, WORLD_FILE_NAME_TYPE) < 0){
				perrorPrint("sprintf");
				freeGridData(grid);
				return NULL;
			}
			worldFile = fopen(filename,"r"); /* open the file */
			if (worldFile == NULL){
				perrorPrint("fopen");
				freeGridData(grid);
				return NULL;
			}
		}
		/* update numTurns */
		if (fscanf(worldFile, "%d", numTurns) < 0){
			perrorPrint("fscanf");
			freeGridData(grid);
			return NULL;
		}
		/* update isCatFirst */
		if (fscanf(worldFile, "%s", firstAnimal) < 0){
			perrorPrint("fscanf");
			freeGridData(grid);
			return NULL;
		}
		if (strcmp(firstAnimal, "cat") == 0)
			*isCatFirst = 1;
		else
			*isCatFirst = 0;
		/* fill grid by file: */
		char nextChar;
		for (int i = 0; i < ROW_NUM;i++){
			for (int j = 0; j < COL_NUM; j++){
				if (j == 0){ /* for the first char in each line */
					while (1){ /* read the first char while it is still \n ot \r */
						if ((fscanf(worldFile, "%c" , &nextChar)) < 0){
							perrorPrint("fscanf");
							freeGridData(grid);
							return NULL;
						}
						if (nextChar != '\r' && nextChar != '\n')
							break;
					}
					grid[i][0] = nextChar; /* put the first char of the line in the grid */
				}
				else{
					if ((fscanf(worldFile, "%c" , &nextChar)) < 0){
						perrorPrint("fscanf");
						freeGridData(grid);
						return NULL;
					}
					grid[i][j] = nextChar;
				}
			}
		}
		/*close the file if it is not stdin */
		if (worldNum > 0)
			fclose(worldFile);
	}
	return grid;
}

/* put the EMPTY_CELL_CHAR in every position in the grid */
void setEmptyGrid(char ** grid){
	for (int i = 0; i< ROW_NUM; i++){
		for (int j = 0; j< COL_NUM; j++)
			grid[i][j] = EMPTY_CELL_CHAR;
	}
}

/* return 1 if pos1 and pos2 have the same row and col */
int isSamePos(gridItemPosition pos1, gridItemPosition pos2){
	if (pos1.row == pos2.row && pos1.col == pos2.col)
		return 1;
	return 0;
}
/* free the dist matrix */
void freeDistMatrix(int ** matrix){
	if (matrix != NULL){
		for (int i = 0; i < ROW_NUM; i++){ /* free the cols */
			free(matrix[i]);
		}
		free(matrix); /* free the rows */
	}

}
