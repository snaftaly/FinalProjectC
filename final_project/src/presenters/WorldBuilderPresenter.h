#ifndef WBPRESENTER_H_
#define WBPRESENTER_H_

/*** includes ***/
#include "../services/LogEventItems.h"


/*** structs ***/

/* a struct for holding the world builder data */
typedef struct WBData{
	/* hold the non-reusable items positions and the current selscted position */
	gridItemPosition catPos;
	gridItemPosition mousePos;
	gridItemPosition cheesePos;
	gridItemPosition currPos;

	int editedWorld; /* which world file is currently edited */
	char missingItems; /* will hold data about missing items (cat/mouse/cheese) in the grid */

	StateId returnStateId; /* the next stateId */

	/* game specific data */
	int isCatFirst; /* will save the first player in case create game is selected */
	int numTurns; /* will save num turns left from the file */
	char ** gameGridData;

} WBData;

typedef struct WBData * WBDataRef;


/*** function deceleration ***/

StateId worldBuilderPHE(void* model, void* viewState, void* logicalEvent);


#endif /* WBPRESENTER_H_ */
