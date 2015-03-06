#ifndef WIDGET_UTILS_H_
#define WIDGET_UTILS_H_

/* includes: */
#include <SDL.h>
#include <SDL_video.h>
#include "../main/ErrorHandling.h"

/* define constants: */
#define WIN_W 800
#define WIN_H 800

typedef enum {
	WINDOW,
	PANEL,
	BUTTON,
	IMAGE
} widget_type;

typedef struct Widget{
	widget_type type; /* maybe not an enum?? */
	SDL_Surface * surface;
	SDL_Rect img_rect; /* the part of the image we cut */
	SDL_Rect location_rect; /* location and size, relative to containing panel/window */
	SDL_Rect button_non_selected_rect; /* the part of the image for the non selected button */
	SDL_Rect button_selected_rect; /* the part of the image for the non selected button */
	int absX, absY; /* the absolute x and y locations. */
		int isButtonSelected; /* is the Button widget selected */
	Widget * parentWidget; /*the window/panel containing the widget (NULL for window) */
} Widget;

/* function declarations: */
Widget * createGeneralWidget(int, int, int, int, widget_type);
Widget * create_window(int, int);
Widget * create_panel(int, int, int, int, int, int,
		int);
Widget * create_image(int, int, int, int, SDL_Surface *,
		int, int);
Widget * create_button(int, int, int, int, SDL_Surface *,
		int, int, int, int, int);

#endif /* WIDGET_UTILS_H_ */
