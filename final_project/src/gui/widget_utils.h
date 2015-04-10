#ifndef WIDGET_UTILS_H_
#define WIDGET_UTILS_H_

/* includes: */
#include <SDL.h>
#include <SDL_video.h>
#include "../main/ErrorHandling.h"

/* define constants */
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
	struct Widget * parentWidget; /*the window/panel containing the widget (NULL for window) */
} Widget;

/* function declarations: */
Widget * createGeneralWidget(Sint16 x, Sint16 y, Uint16 width, Uint16 height, widget_type type);
Widget * create_window(int, int, int, int, int);
Widget * create_panel(int, int, int, int, int, int,
		int);
Widget * create_image(int, int, int, int, SDL_Surface *,
		int, int);
Widget * create_button(Sint16 x, Sint16 y, Uint16 width, Uint16 height, SDL_Surface * img,
		Sint16 imgSx, Sint16 imgSy, Sint16 imgNSx, Sint16 imgNSy, int isSelected);
void setButtonSelected(Widget *);
void setButtonNotSelected(Widget *);
void setButtonDisabled(Widget * button);
void setButtonEnabled(Widget * button);
void setImageTransparent(Widget *image, int red, int green, int blue);
void clearPanel(void * panel);

void freeWidget(void *);

#endif /* WIDGET_UTILS_H_ */
