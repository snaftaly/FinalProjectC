#include "gridDataUtils.h"


/** grid initialization functions **/

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

/* put the EMPTY_CELL_CHAR in every position in the grid */
void setEmptyGrid(char ** grid){
	for (int i = 0; i< ROW_NUM; i++){
		for (int j = 0; j< COL_NUM; j++)
			grid[i][j] = EMPTY_CELL_CHAR;
	}
}


/** grid position info functions **/

/* return 1 if pos1 and pos2 have the same row and col */
int isSamePos(gridItemPosition pos1, gridItemPosition pos2){
	if (pos1.row == pos2.row && pos1.col == pos2.col)
		return 1;
	return 0;
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

/* get item char by grid item type */
char getItemChar(gridItem item){
	char itemsChars[5] = {CAT_CHAR, MOUSE_CHAR, CHEESE_CHAR, WALL_CHAR, EMPTY_CELL_CHAR};
	return itemsChars[item];
}

/** free functions **/

/* free memory allocated for the grid */
void freeGridData(char ** gridData){
	if (gridData != NULL){
		for (int i = 0; i < ROW_NUM; i++) /* free the cols */
			free(gridData[i]);
		free(gridData); /* free the rows */
	}
}
