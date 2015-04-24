#include "Presenters.h"

/**** world builder PHE ****/

/* The phe function for the world builder */
StateId worldBuilderPHE(void* model, void* viewState, void* logicalEvent){
	StateId returnStateId = WORLD_BUILDER; /* default return stateId */
	if (logicalEvent == NULL || viewState == NULL || model == NULL)
		return returnStateId;
	logicalEventRef wbEvent = logicalEvent;
	ViewStateref wbView = viewState;
	WBDataRef wbModel = model; /* get the model data */
	/* create an array of return stateIds according to the buttons: */
	StateId states[WB_NUM_BUTTONS] = {SAVE_WORLD, MAIN_MENU, QUIT, WORLD_BUILDER, WORLD_BUILDER, WORLD_BUILDER,
			WORLD_BUILDER,WORLD_BUILDER};
	/* switch over logical events type */
	switch(wbEvent->type){
		case(DO_QUIT): /* exit the program */
			returnStateId = QUIT;
			break;
		case(SELECT_BUTTON_NUM):
			returnStateId = states[wbEvent->buttonNum];
			if(returnStateId == SAVE_WORLD && isGridInvalid(wbModel)) /* save world was pressed and grid is invalid */
				returnStateId = ERR_MSG;
			else if (returnStateId == WORLD_BUILDER){ /* place mouse/cat/cheese/wall/empty space was pressed */
				putGridItemInPos(wbModel, wbView->gridPanel, wbView->gridItemsImgArr, wbModel->currPos, wbEvent->buttonNum);
			}
			break;
		case(SELECT_SQUARE): /* a grid square was pressed */
			changeSelectedGridSquare(wbView->gridPanel, wbView->gridItemsImgArr,&wbModel->currPos, wbEvent->gridPos);
			break;
		case(GO_UP): /* up arrow pressed */
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImgArr, &wbModel->currPos, UP);
			break;
		case(GO_DOWN): /* down arrow pressed */
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImgArr, &wbModel->currPos, DOWN);
			break;
		case(GO_RIGHT): /* right arrow pressed */
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImgArr, &wbModel->currPos, RIGHT);
			break;
		case(GO_LEFT): /* left arrow pressed */
			changeSelectedPosByArrow(wbView->gridPanel, wbView->gridItemsImgArr, &wbModel->currPos, LEFT);
			break;
		case(NO_EVENT):
			break;
		default:
			break;

	}
	free(logicalEvent);
	wbModel->returnStateId = returnStateId; /* update returnStateId in model */
	return returnStateId;
}
