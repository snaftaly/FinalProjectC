#include "widgetUtils.h"

/** buttons handling related function */

/* set a button as selected */
void setButtonSelected(Widget * button){
	button->isButtonSelected = 1; /* update the isSelected field */
	button->imgRect = button->buttonSelectedRect; /* update the imgRect according to the selectedRect */

}

/* set a button as not selected */
void setButtonNotSelected(Widget * button){
	button->isButtonSelected = 0; /* update the isSelected field */
	button->imgRect = button->buttonNonSelectedRect; /* update the imgRect according to the nonSelectedRect */
}

/* disable a button */
void setButtonDisabled(Widget * button){
	setButtonSelected(button); /* disable a button by calling the set button selected */
}

/* enable a button */
void setButtonEnabled(Widget * button){
	setButtonNotSelected(button); /* enable a button by calling the set button not selected */
}

/* set an image to be transparent for a given RGB set */
void setImageTransparent(Widget *image, int red, int green, int blue){
	/* call SDL_SetColorKey with the colors given, on the image surface */
	if (SDL_SetColorKey(image->surface, SDL_SRCCOLORKEY, SDL_MapRGB(image->surface->format, red, green, blue)) !=0){
		sdlErrorPrint("failed setting image transparent");
		isError = 1;
	}
}

/* clear a panel to its backgroung color */
void clearPanel(void * panel){
	Widget * toClear = panel;
	/* get panel colors */
	Uint32 temp, pixel;
	Uint8 red, green, blue;
	SDL_PixelFormat * fmt=toClear->surface->format;
	pixel=*((Uint32*)toClear->surface->pixels);
	temp=pixel&fmt->Rmask; /* Isolate red component */
	temp=temp>>fmt->Rshift;/* Shift it down to 8-bit */
	temp=temp<<fmt->Rloss; /* Expand to a full 8-bit number */
	red=(Uint8)temp;
	temp=pixel&fmt->Gmask; /* Isolate green component */
	temp=temp>>fmt->Gshift;/* Shift it down to 8-bit */
	temp=temp<<fmt->Gloss; /* Expand to a full 8-bit number */
	green=(Uint8)temp;
	/* Get Blue component */
	temp=pixel&fmt->Bmask; /* Isolate blue component */
	temp=temp>>fmt->Bshift;/* Shift it down to 8-bit */
	temp=temp<<fmt->Bloss; /* Expand to a full 8-bit number */
	blue=(Uint8)temp;
	/* recolor the panel */
	if (SDL_FillRect(toClear->surface,0,SDL_MapRGB(toClear->surface->format, red, green, blue)) != 0) {
		sdlErrorPrint("failed to fill rect with color");
		return;
	}
}

/** free functions **/

/* free widget function  - free the widget
 * and free its surface for panel or window
 * (images surfaces will be freed at once when freeing the gui's image surface) */
void freeWidget(void * widgetPtr){
	Widget * widget = widgetPtr;
	if (widget != NULL){
		if (widget->surface != NULL && widget->type == PANEL){
			/* free the surface of a panel -
			 * comment: window surface will be frees by sdl_exit and button/imuage
			 * surface are freed through thier image (in free view state function */
			SDL_FreeSurface(widget->surface);
		}
		free(widget); /* free the widget */
	}
}
