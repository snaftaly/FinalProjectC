#ifndef GUISFACTORY_H_
#define GUISFACTORY_H_

#include "../main/CatAndMouse.h"
#include "../views/Views.h"
#include "../stateTransition/StateTransition.h"

GUI createGUIForState(StateId stateId);

#endif /* GUISFACTORY_H_ */
