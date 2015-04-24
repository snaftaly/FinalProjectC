#ifndef WIDGETUTILS_H_
#define WIDGETUTILS_H_

/**** includes ****/
#include "../factories/WidgetsFactory.h"

/**** functions decelerations ****/

/** buttons related function **/
void setButtonSelected(Widget *);
void setButtonNotSelected(Widget *);
void setButtonDisabled(Widget * button);
void setButtonEnabled(Widget * button);

/* image and panel functions **/
void setImageTransparent(Widget *image, int red, int green, int blue);
void clearPanel(void * panel);

/* free functions */
void freeWidget(void *);


#endif /* WIDGETUTILS_H_ */
