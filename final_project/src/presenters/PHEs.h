/*
 * presenters.h
 *
 *  Created on: Apr 17, 2015
 *      Author: naftaly1
 */

#ifndef PRESENTERS_H_
#define PRESENTERS_H_

/* includes */
#include "../services/MVPutils.h"

/* function decelerations */

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

/* stand alone PHEs */
StateId worldBuilderPHE(void* model, void* viewState, void* logicalEvent);
StateId errMsgPHE(void* model, void* viewState, void* logicalEvent);
StateId playGamePHE(void* model, void* viewState, void* logicalEvent);


#endif /* PRESENTERS_H_ */
