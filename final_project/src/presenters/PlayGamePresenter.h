#ifndef PGPRESENTER_H_
#define PGPRESENTER_H_

/* includes */
#include "../services/LogEventItems.h"


/* a struct for holding the play game data */
typedef struct PGData{

	/* hold the cat,mouse,cheese current positions */
	gridItemPosition catPos;
	gridItemPosition mousePos;
	gridItemPosition cheesePos;

	int loadGameWorld; /* the game file tha was loaded */
	int loadFromFile; /* will be used for reconfigure mouse/cat transitions */

	/* information about the players */
	int isCatHuman;
	int isMouseHuman;
	int catSkill;
	int mouseSkill;
	int isCatCurrPlayer;
	/* information about the game */
	int numTurnsLeft;
	int isGameOver;
	gameOverType gameOverType;
	int isGamePaused;
	char ** gameGridData;

	StateId returnStateId; /* the next stateId */

} PGData;

typedef struct PGData * PGDataRef;

/* function decelerations */

StateId playGamePHE(void* model, void* viewState, void* logicalEvent);


#endif /* PGPRESENTER_H_ */
