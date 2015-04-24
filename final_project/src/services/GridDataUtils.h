#ifndef GRIDDATAUTILS_H_
#define GRIDDATAUTILS_H_

/**** includes ****/
#include "../main/CatAndMouse.h"

/**** enums ****/

/* enumeration of the different grid Items */
typedef enum{
	CAT,
	MOUSE,
	CHEESE,
	WALL,
	EMPTY,
	SELECT,
	DESELECT,
	WARN
} gridItem;

/**** structs: ****/

/* a struct that holds a grid Item position -
 * row num and col num, where the top left corner is (0,0)
 */
typedef struct gridItemPosition{
	int row;
	int col;
} gridItemPosition;

/**** functions declerations ****/

/** grid initialization functions **/
char ** initGrid();
char ** copyGrid(char ** fromGrid);
void setEmptyGrid(char ** grid);

/** grid position info functions **/
int isSamePos(gridItemPosition pos1, gridItemPosition pos2);
int isAdjPos(gridItemPosition pos1, gridItemPosition pos2);
int isGridPosFree(gridItemPosition gridPos, char ** gridData);
gridItemPosition getPosByDirection(gridItemPosition currPos, direction direction);
void updateItemsPositions(gridItemPosition * mousePos,gridItemPosition * catPos,gridItemPosition * cheesePos,
		char ** gameGridData);
char getItemChar(gridItem item);

/** free functions **/
void freeGridData(char ** gridData);



#endif /* GRIDDATAUTILS_H_ */
