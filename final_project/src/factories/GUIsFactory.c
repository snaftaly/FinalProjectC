#include "GUIsFactory.h"

/* assign the GUI its start, VTE, PHE and stop function according to its stateId */
GUI createGUIForState(StateId stateId){
	GUI returnGUI;
	returnGUI.stateId = stateId; /* set GUI's stateId */
	/* set GUI view state and model as NULL */
	returnGUI.model = NULL;
	returnGUI.viewState = NULL;
	/* set the GUI function pointers */
	switch(stateId){
		case(MAIN_MENU):
			returnGUI.start = startMainMenu;
			returnGUI.viewTranslateEvent = mainMenuVTE;
			returnGUI.presenterHandleEvent = mainMenuPHE;
			returnGUI.stop = stopMenu;
			break;
		case(CHOOSE_CAT):
			returnGUI.start = startChooseAnimal;
			returnGUI.viewTranslateEvent = chooseAnimalVTE;
			returnGUI.presenterHandleEvent = chooseCatPHE;
			returnGUI.stop = stopMenu;
			break;
		case(CHOOSE_MOUSE):
			returnGUI.start = startChooseAnimal;
			returnGUI.viewTranslateEvent = chooseAnimalVTE;
			returnGUI.presenterHandleEvent = chooseMousePHE;
			returnGUI.stop = stopMenu;
			break;
		case(CAT_SKILL):
			returnGUI.start = startAnimalSkill;
			returnGUI.viewTranslateEvent = complexMenuVTE;
			returnGUI.presenterHandleEvent = catSkillPHE;
			returnGUI.stop = stopMenu;
			break;
		case(MOUSE_SKILL):
			returnGUI.start = startAnimalSkill;
			returnGUI.viewTranslateEvent = complexMenuVTE;
			returnGUI.presenterHandleEvent = mouseSkillPHE;
			returnGUI.stop = stopMenu;
			break;
		case(LOAD_GAME):
			returnGUI.start = startWorldMenu;
			returnGUI.viewTranslateEvent = complexMenuVTE;
			returnGUI.presenterHandleEvent = loadGamePHE;
			returnGUI.stop = stopMenu;
			break;
		case(WORLD_BUILDER):
			returnGUI.start = startWorldBuilder;
			returnGUI.viewTranslateEvent = worldBuilderVTE;
			returnGUI.presenterHandleEvent = worldBuilderPHE;
			returnGUI.stop = stopWorldBuilder;
			break;
		case(EDIT_GAME):
			returnGUI.start = startWorldMenu;
			returnGUI.viewTranslateEvent = complexMenuVTE;
			returnGUI.presenterHandleEvent = editGamePHE;
			returnGUI.stop = stopMenu;
			break;
		case(SAVE_WORLD):
			returnGUI.start = startWorldMenu;
			returnGUI.viewTranslateEvent = complexMenuVTE;
			returnGUI.presenterHandleEvent = saveWorldPHE;
			returnGUI.stop = stopMenu;
			break;
		case(PLAY_GAME):
			returnGUI.start = startPlayGame;
			returnGUI.viewTranslateEvent = playGameVTE;
			returnGUI.presenterHandleEvent = playGamePHE;
			returnGUI.stop = stopPlayGame;
			break;
		case(ERR_MSG):
			returnGUI.start = startErrMsg;
			returnGUI.viewTranslateEvent = errMsgVTE;
			returnGUI.presenterHandleEvent = errMsgPHE;
			returnGUI.stop = stopMenu;
			break;
		default:
			break;
	}
	return returnGUI;
}
