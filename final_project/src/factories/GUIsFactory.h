#ifndef GUISFACTORY_H_
#define GUISFACTORY_H_

#include "../main/CatAndMouse.h"
#include "../views/VTEs.h"
#include "../presenters/PHEs.h"
#include "../stateTransition/stateTransition.h"

GUI createGUIForState(StateId stateId);

#endif /* GUISFACTORY_H_ */
