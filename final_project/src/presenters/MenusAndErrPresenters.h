#ifndef MEPRESENTERS_H_
#define MEPRESENTERS_H_

/*** includes ***/
#include "../services/LogEventItems.h"

/*** structs ***/

/* a struct for holding the menu data */
typedef struct MenuData{
	/* hold information about cat and mouse from cat/mouse choose/skill menus*/
	int catSkill;
	int mouseSkill;
	int isCatHuman;
	int isMouseHuman;
	/* hold the current selected button for each menu */
	int mainMenuButton;
	int chooseCatButton;
	int chooseMouseButton;
	int catSkillButton;
	int mouseSkillButton;
	int loadGameButton;
	int editGameButton;
	int saveWorldButton;
	int errMsgButton;
	/* for specific stateIds, hold the previous stateId which we came from to them */
	StateId preChooseCat;
	StateId preChooseMouse;
	StateId preWorldBuilder;

	StateId retStateId; /* the next stateId */

	int currValueTemp; /* used fot changing cat/mouse skill or world number values before pressing done */
	int loadGameWorld; /* which game to load in play game */
	int editedWorld; /* which game to edit in world builder */
	gridItemPosition wbCurrPos; /* used for saving the currPos of wb for when we get back from errMsg */
	int missingItems; /* used for passing the missing items from world builder to errr msg */
	int loadFromFile; /* used for telling world builder/play game if game should be loaded from file */
	int isGamePaused; /* used for going to reconfigure cat/mouse and returning to a paused game */

	/* game specific data */
	int isCatFirst;
	int numTurns;
	char ** gameGridData;

} MenuData;

typedef struct MenuData * MenuDataRef;


/*** function decelerations ***/

/* menus PHEs */
StateId generalMenuPHE(void* model, void* viewState, void* logicalEvent, StateId states[], int numOfButtons,
		StateId stateId, int* currButton, int* currValue, int maxValue);
StateId mainMenuPHE(void* model, void* viewState, void* logicalEvent);
StateId chooseCatPHE(void* model, void* viewState, void* logicalEvent);
StateId chooseMousePHE(void* model, void* viewState, void* logicalEvent);
StateId catSkillPHE(void* model, void* viewState, void* logicalEvent);
StateId mouseSkillPHE(void* model, void* viewState, void* logicalEvent);
StateId loadGamePHE(void* model, void* viewState, void* logicalEvent);
StateId editGamePHE(void* model, void* viewState, void* logicalEvent);
StateId saveWorldPHE(void* model, void* viewState, void* logicalEvent);

/* ErrMsg PHE */
StateId errMsgPHE(void* model, void* viewState, void* logicalEvent);


#endif /* MEPRESENTERS_H_ */
