#ifndef STATETRANSITION_H_
#define STATETRANSITION_H_

/***** includes ****/
#include "../services/MVPutils.h"

/**** function decelerations ****/

/* start functions */
void startGeneralMenu(GUIref gui, char * imgPath, int titleImgX, int titleImgY, int titleWidth, int numButtons, int selectedButton, int firstButtonNumOpts, int value);
void startMainMenu(GUIref gui, void* initData);
void startChooseAnimal(GUIref gui, void* initData);
void startAnimalSkill(GUIref gui, void* initData);
void startWorldMenu(GUIref gui, void* initData);
void startWorldBuilder(GUIref gui, void* initData);
void startErrMsg(GUIref gui, void* initData);
void startPlayGame(GUIref gui, void* initData);

/* stop functions */
void* stopMenu(GUIref gui); /* maybe this will be a general stop function */
void* stopWorldBuilder(GUI * gui);
void* stopPlayGame(GUI * gui);


#endif /* STATETRANSITION_H_ */
