#include "CatAndMouseUtils.h"



/* run console mode */
void consoleMode(int isCatCurrPlayer){
	/*set the variables */
	int numTurnsLeft;
	gridItemPosition catPos, mousePos, cheesePos;
	char ** gridData = NULL;
	GameStateRef currState = NULL;

	while(1){
		/* get grid data and numturns from stdin */
		gridData = initGameDataByFile(-1, &numTurnsLeft, NULL);
		if (gridData == NULL) /* an error occurred or q\n was typed */
			return;
		getchar(); /* get the \n that comes right after the grid */
//		if (endChar == 'q'){
//			freeGridData(gridData);
//			return;
//		}
		/*update the items positions by the grid data */
		updateItemsPositions(&mousePos,&catPos,&cheesePos, gridData);

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

/* return 1 if the move is adjacent to the player position and the grid position is free */
int isMoveValid(char ** gridData, gridItemPosition currPlayerPos, gridItemPosition movePos){
	if (isAdjPos(currPlayerPos, movePos) && isGridPosFree(movePos, gridData))
		return 1;
	return 0;
}

/* initialize game data by file - reads the file and updated the grid, num turns and isCatFirst
 * if worldnum is > 0, reads from the file with the appropriate file name
 * if worldnum is = 0, created an empty grid, sets num turns to DEFAULT_TURNS
 * if worldnum is -1  reads the data from stdin , and check if we exit the program */
char ** initGameDataByFile(int worldNum, int * numTurns, int * isCatFirst){
	char ** grid = NULL ;
	char firstLine[3]; /* initialize first line value for console mode */

	*numTurns = DEFAULT_TURNS; /* set num turns to DEFAULT_TURNS */
	char firstAnimal[6];
	if (worldNum == 0){ /* if world num is 0, create an empty grid */
		grid = initGrid(); /* initialize the grid */
		if (isError)
			return NULL;
		setEmptyGrid(grid); /* fill the grid with the empty cell char */
	}
	else{ /* world num != 0 */
		FILE * worldFile = NULL;
		if (worldNum == -1){ /* read from standard input - for console mode */
			worldFile = stdin;
			/* check first line content */
			if (fscanf(worldFile, "%s", firstLine) < 0){
				perrorPrint("fscanf");
				return NULL;
			}
			if (strcmp(firstLine, "q") == 0){ /* check if we need to quit */
				return NULL;
			}
			else if (sscanf(firstLine, "%d", numTurns) < 0){ /* update num turns */
				perrorPrint("fscanf");
				return NULL;
			}
		}
		else { /* read from appropriate world file */
			/* create the file name */
			char filename[WORLD_FILE_NAME_LEN];
			if (sprintf(filename, "%s%s%d.%s", WORLD_FILE_PATH, WORLD_FILE_NAME_PREFIX, worldNum, WORLD_FILE_NAME_TYPE) < 0){
				perrorPrint("sprintf");
				return NULL;
			}
			worldFile = fopen(filename,"r"); /* open the file */
			if (worldFile == NULL){
				perrorPrint("fopen");
				return NULL;
			}
			/* update numTurns */
			if (fscanf(worldFile, "%d", numTurns) < 0){
				perrorPrint("fscanf");
				return NULL;
			}
		}

		/* update isCatFirst if the isCatFirst (pointer) variable is not NULL */
		if (fscanf(worldFile, "%s", firstAnimal) < 0){
			perrorPrint("fscanf");
			return NULL;
		}
		if (isCatFirst != NULL ){ /* if isCatFirst is not a NULL pointer */
			if (strcmp(firstAnimal, "cat") == 0)
				*isCatFirst = 1;
			else
				*isCatFirst = 0;
		}
		/* init grid and fill it by file: */
		grid = initGrid(); /* initialize the grid */
		if (isError)
			return NULL;
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

/* saveGridDataToFile is used by save world to save the grid data to a file */
void saveGameDataToFile(int worldNum, int isCatFirst, char ** gridData){
	/* get the world file name by according to the world num */
	char filename[WORLD_FILE_NAME_LEN];
	worldNum = worldNum ? worldNum : 1;
	if (sprintf(filename, "%s%s%d.%s", WORLD_FILE_PATH, WORLD_FILE_NAME_PREFIX, worldNum, WORLD_FILE_NAME_TYPE) < 0){
		perrorPrint("sprintf");
		return;
	}
	FILE * worldFile = fopen(filename,"w"); /* open the file */
	if (worldFile == NULL){
		perrorPrint("fopen");
		return;
	}

	/* update numTurns */
	if (fprintf(worldFile, "%d\n", DEFAULT_TURNS) < 0){
		perrorPrint("fprintf");
		return;
	}
	/* update isCatFirst */
	char firstAnimal[6];
	if (isCatFirst)
		strcpy(firstAnimal,"cat");
	else
		strcpy(firstAnimal,"mouse");
	if (fprintf(worldFile, "%s\n", firstAnimal) < 0){
		perrorPrint("fprintf");
		return;
	}

	/* fill file by grid: */
	char nextChar;
	for (int i = 0; i< ROW_NUM;i++){
		for (int j = 0; j< COL_NUM; j++){
			nextChar = gridData[i][j];
			if ((fprintf(worldFile, "%c" , nextChar)) < 0){
				perrorPrint("fprintf");
				return;
			}
			if (j == COL_NUM-1 && i < ROW_NUM-1){ /* add \n on each row, apart from the last */
				if (fprintf(worldFile, "\n") < 0){
					perrorPrint("fprintf");
					return;
				}
			}
		}
	}
	/* close the file */
	fclose(worldFile);
}


/**** machine player utils ****/

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


/* getChildren function:
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
	if (eval == EVALERR)
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
	if (gameOverType == TIE)
		return 0;

	int ** distCat = getDistanceWithBFS(currState->catPos, currState->gridData);
	if (isError) { return EVALERR; }
	int ** distMouse = getDistanceWithBFS(currState->mousePos, currState->gridData);
	if (isError){
		freeDistMatrix(distCat);
		return EVALERR;
	}
	int catFromMouse = distCat[currState->mousePos.row][currState->mousePos.col];
	int catFromCheese = distCat[currState->cheesePos.row][currState->cheesePos.col];
	int mouseFromCheese = distMouse[currState->cheesePos.row][currState->cheesePos.col];
	int eval;
	int CMrowDiff = abs(currState->catPos.row-currState->mousePos.row);
//	int CMcolDiff = abs(currState->catPos.col-currState->mousePos.col);
//	int MProwDiff = abs(currState->mousePos.row-currState->cheesePos.row);
//	int MPcolDiff = abs(currState->mousePos.col-currState->cheesePos.col);
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


/* return 0 if the grid positions contains the WALL_CHAR or VISITED_CHAR, otherwise return 1 */
int isPosReachable(gridItemPosition pos, char ** gridData){
	if (gridData[pos.row][pos.col] == WALL_CHAR || gridData[pos.row][pos.col] == VISITED_CHAR)
		return 0;
	return 1;
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

/* free the dist matrix */
void freeDistMatrix(int ** matrix){
	if (matrix != NULL){
		for (int i = 0; i < ROW_NUM; i++){ /* free the cols */
			free(matrix[i]);
		}
		free(matrix); /* free the rows */
	}
}

