#ifndef WIDGET_UTILS_H_
#define WIDGET_UTILS_H_

/* includes: */
#include <SDL.h>
#include <SDL_video.h>
#include "../main/ErrorHandling.h"

/* define constants */
#define WIN_W 800
#define WIN_H 800

/* an enumeration of the different widgets */
typedef enum {
	WINDOW,
	PANEL,
	BUTTON,
	IMAGE
} widget_type;

/* struct widget - holds all the data needed for the different widgets */
typedef struct Widget{
	widget_type type; /* the type of the widget */
	SDL_Surface * surface; /* the SDL surface of the widget */
	SDL_Rect imgRect; /* the part of the image we cut */
	SDL_Rect locationRect; /* location and size, relative to containing panel/window */
	/* the part of the image for the non selected button
	 * - also used as button enabled rect for play game */
	SDL_Rect buttonNonSelectedRect;
	/* the part of the image for the non selected button
	 * - also used as button disabled rect for play game */
	SDL_Rect buttonSelectedRect;
	int absX, absY; /* the absolute x and y locations. */
		int isButtonSelected; /* is the Button widget selected */
	struct Widget * parentWidget; /*the window/panel containing the widget (NULL for window) */
} Widget;

/***** function declarations: *****/

/** widget creation functions **/
Widget * createGeneralWidget(Sint16 x, Sint16 y, Uint16 width, Uint16 height, widget_type type);
Widget * createWindow(int, int, int, int, int);
Widget * create_panel(int, int, int, int, int, int,
		int);
Widget * create_image(int, int, int, int, SDL_Surface *,
		int, int);
Widget * create_button(Sint16 x, Sint16 y, Uint16 width, Uint16 height, SDL_Surface * img,
		Sint16 imgSx, Sint16 imgSy, Sint16 imgNSx, Sint16 imgNSy, int isSelected);

/** buttons related function */
void setButtonSelected(Widget *);
void setButtonNotSelected(Widget *);
void setButtonDisabled(Widget * button);
void setButtonEnabled(Widget * button);

/* image and panel functions */
void setImageTransparent(Widget *image, int red, int green, int blue);
void clearPanel(void * panel);

/* free functions */
void freeWidget(void *);

#endif /* WIDGET_UTILS_H_ */
