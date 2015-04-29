#include "PlayGamePresenter.h"
#include "../services/MVPutils.h"

/**** Play Game PHE ****/

/* The phe function for the play game */
StateId playGamePHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = PLAY_GAME; /* default return stateId */
	if (logicalEvent == NULL || viewState == NULL || model == NULL)
		return returnStateId;
	logicalEventRef pgEvent = logicalEvent;
	ViewStateref pgViewState = viewState;
	PGDataRef pgModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId states[PG_NUM_BUTTONS] = {PLAY_GAME, CHOOSE_MOUSE, CHOOSE_CAT, PLAY_GAME, MAIN_MENU, QUIT};
	if (!pgModel->isMouseHuman)
		states[1] = MOUSE_SKILL;
	if (!pgModel->isCatHuman)
		states[2] = CAT_SKILL;
	/* switch over logical events type */
	switch(pgEvent->type){
		case(DO_QUIT): /* exit the program */
			returnStateId = QUIT;
			break;
		case(SELECT_BUTTON_NUM):
			if (pgModel->isGamePaused || pgModel->isGameOver){
				returnStateId = states[pgEvent->buttonNum];
				if (pgEvent->buttonNum == 3){ /* restart game was pressed */
					restartGame(pgViewState, pgModel);
				}
				if (pgEvent->buttonNum == 0 && pgModel->isGamePaused){ /* game is paused and space was pressed */
					resumeGame(pgViewState, pgModel);
				}
			}
			else if (pgEvent->buttonNum == 0){ /* game is running and space was pressed */
				pauseGame(pgViewState, pgModel);
			}
			break;
		case(SELECT_SQUARE): /* a square was selected */
			if (isCurrPlayerHuman(pgModel)) /* if current player is human make the move if possible */
				makeGameMoveIfLegal(pgViewState, pgModel, pgEvent->gridPos);
			break;
		case(GO_UP): /* arrow button pressed - make move if possible */
			if (isCurrPlayerHuman(pgModel))
				makeGameMoveByArrowIfLegal(pgViewState, pgModel, UP);
			break;
		case(GO_DOWN): /* arrow button pressed - make move if possible */
			if (isCurrPlayerHuman(pgModel))
				makeGameMoveByArrowIfLegal(pgViewState, pgModel, DOWN);
			break;
		case(GO_RIGHT): /* arrow button pressed - make move if possible */
			if (isCurrPlayerHuman(pgModel))
				makeGameMoveByArrowIfLegal(pgViewState, pgModel, RIGHT);
			break;
		case(GO_LEFT): /* arrow button pressed - make move if possible */
			if (isCurrPlayerHuman(pgModel))
				makeGameMoveByArrowIfLegal(pgViewState, pgModel, LEFT);
			break;
		case(NO_EVENT):
			break;
		default:
			break;

	}
	/* if reconfigure cat/mouse was pressed */
	if (returnStateId == CHOOSE_CAT || returnStateId == CHOOSE_MOUSE ||
			returnStateId == CAT_SKILL || returnStateId == MOUSE_SKILL)
		pgModel->loadFromFile = 0; /* set the load from file value to 0, so that when we get back the same grid will be shown */
	free(logicalEvent);
	pgModel->returnStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}
