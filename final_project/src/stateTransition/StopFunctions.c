#include "StateTransition.h"

/**** stop functions: ****/

/* stop function for all menus and err msg */
void* stopMenu(GUIref gui){ /* maybe this will be a general stop function */
	/* get the model and view state */
	MenuDataRef returnData = gui->model; /* return data will be menu data */
	ViewStateref guiViewState = gui->viewState;
	/* reset the model and view to NULL */
	gui->model = NULL;
	gui->viewState = NULL;

	freeViewState(guiViewState); /* free viewstate */

	if (isError || returnData->retStateId == QUIT){ /* if we are in a final situation, free the return data */
		freeMenuData(returnData);
		return NULL;
	}
	return returnData;
}

/* stop function for world builder */
void* stopWorldBuilder(GUI * gui){
	/* get the model and view state */
	ViewStateref guiViewState = gui->viewState;
	WBDataRef wbData = gui->model;
	/* reset the model and view to NULL */
	gui->model = NULL;
	gui->viewState = NULL;

	/* create a reference to the ThreePartLayoutViewExt and free it */
	ThreePartViewExtRef threePartView = guiViewState->ViewExt;
	freeThreePartExtViewState(threePartView); /* free the three Part view extention */

	freeViewState(guiViewState); /* free viewstate */

	/* if we are in a final situation, or we go back to Main Menu, free the wb data */
	if (isError || wbData->returnStateId == QUIT || wbData->returnStateId == MAIN_MENU){
		freeWBData(wbData, 1); /* free wbData with grid free */
		return NULL;
	}

	/* initialize return data as menu data default values */
	MenuDataRef returnData = initMenuDataToDefault();
	if (isError){
		freeWBData(wbData, 1); /* free wbData with grid free */
		return NULL;
	}
	/* Assign values in return Data for save world/error message: */
	returnData->gameGridData = wbData->gameGridData;
	returnData->editedWorld = wbData->editedWorld;
	returnData->isCatFirst = wbData->isCatFirst;
	returnData-> numTurns = wbData->numTurns;
	returnData->wbCurrPos = wbData->currPos;
	returnData->currValueTemp = wbData->editedWorld ? wbData->editedWorld : MIN_VALUE;
	returnData->missingItems = wbData->missingItems;

	freeWBData(wbData, 0); /* free wbData without grid free */

	return returnData;
}


/* stop function for play game */
void* stopPlayGame(GUI * gui){
	/* get the model and view state */
	ViewStateref guiViewState = gui->viewState;
	PGDataRef pgData = gui->model;
	/* reset the model and view to NULL */
	gui->model = NULL;
	gui->viewState = NULL;

	/* create a reference to the ThreePartLayoutViewExt and free it*/
	ThreePartViewExtRef threePartView = guiViewState->ViewExt;

	freeThreePartExtViewState(threePartView); /* free the three Part view extention */
	freeViewState(guiViewState); /* free viewstate */

	/* if we are in a final situation, or we go back to Main Menu, free the PG data */
	if (isError || pgData->returnStateId == QUIT || pgData->returnStateId == MAIN_MENU){
		freePGData(pgData, 1); /* free pgData with grid free */
		return NULL;
	}

	/* initialize return data as menu data default values */
	MenuDataRef returnData = initMenuDataToDefault();
	if (isError){
		freePGData(pgData, 1); /* free pgData with grid free */
		return NULL;
	}
	/*Assign values in return Data for choose cat & choose mouse: */
	returnData->gameGridData = pgData->gameGridData;
	returnData->loadGameWorld = pgData->loadGameWorld;
	returnData->isCatFirst = pgData->isCatCurrPlayer;
	returnData-> numTurns = pgData->numTurnsLeft;
	returnData->catSkill = pgData->catSkill;
	returnData->mouseSkill = pgData->mouseSkill;
	returnData->gameGridData = pgData->gameGridData;
	returnData->isCatHuman = pgData->isCatHuman;
	returnData->isMouseHuman = pgData->isMouseHuman;
	returnData->preChooseCat = PLAY_GAME;
	returnData->preChooseMouse = PLAY_GAME;
	returnData->chooseCatButton = pgData->isCatHuman ? 0 : 1;
	returnData->chooseMouseButton = pgData->isMouseHuman ? 0 : 1;
	returnData->isGamePaused = pgData->isGamePaused;
	returnData->loadFromFile = pgData->loadFromFile;
	if (pgData->returnStateId == CAT_SKILL)
		returnData->currValueTemp = pgData->catSkill;
	if (pgData->returnStateId == MOUSE_SKILL)
		returnData->currValueTemp = pgData->mouseSkill;

	freePGData(pgData, 0); /* free pgData without grid free */

	return returnData;
}

