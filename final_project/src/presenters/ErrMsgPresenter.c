#include "Presenters.h"

/**** errMsg PHE ****/


/* The phe function for error message. calls generalMenuPHE */
StateId errMsgPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = ERR_MSG; /* default return stateId */
	if (model == NULL)
		return returnStateId;
	MenuDataRef errMsgModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId errMsgStates[MAIN_MENU_NUM_BUTTONS] = {WORLD_BUILDER};
	/* get stateId by logical event, and update model and view */
	returnStateId = generalMenuPHE(model, viewState, logicalEvent, errMsgStates, ERR_MSG_NUM_BUTTONS,
			returnStateId, &errMsgModel->errMsgButton, NULL, 0);
	if (returnStateId == WORLD_BUILDER) /* back button pressed */
		errMsgModel->preWorldBuilder = ERR_MSG;
	errMsgModel->retStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}
