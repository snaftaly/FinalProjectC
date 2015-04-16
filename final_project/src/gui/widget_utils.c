#include "widget_utils.h"

/* a function for setting all widgets common settings */
Widget * createGeneralWidget(Sint16 x, Sint16 y, Uint16 width, Uint16 height, widget_type type){
	Widget * new_widget = malloc(sizeof(Widget)); /* allocate memory */
	if(new_widget == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	new_widget->type = type; /* assign type */
	SDL_Rect location_rect = {x, y, width, height}; /* create the location rect */
	new_widget->locationRect = location_rect; /* assign the location rect  */
	new_widget->parentWidget = NULL; /* assign parent widget as NULL */
	return new_widget;
}

/* create window with a specific width, height, and a specific color */
Widget * createWindow(int width, int height, int red, int green, int blue){
	Widget * new_window = createGeneralWidget(0,0,width,height,WINDOW); /* create widget with general setting */
	if(new_window == NULL){
		return NULL;
	}
	new_window->surface  = SDL_SetVideoMode(width, height, 0, SDL_HWSURFACE | SDL_DOUBLEBUF); /* set the surface */
	if (new_window->surface == NULL){
		sdlErrorPrint("failed to set video mode");
		return NULL;
	}
	/* fill the window with color */
	if (SDL_FillRect(new_window->surface,0,SDL_MapRGB(new_window->surface->format, red, green, blue)) != 0) {
		sdlErrorPrint("failed to fill rect with color");
		return NULL;
	}
	return new_window;
}

/* create a panel with certain location, size and color */
Widget * create_panel(int x, int y, int width, int height, int red, int green, int blue){
	Widget * new_panel = createGeneralWidget(x, y, width, height, PANEL); /* create widget with general setting */
	if(new_panel == NULL){
		return NULL;
	}
	new_panel->surface  = SDL_CreateRGBSurface(0, width, height, 32,0, 0, 0, 0); /* create the surface */
	if (new_panel->surface == NULL){
		sdlErrorPrint("failed to create RGB surface");
	}
	/* fill with color */
	if (SDL_FillRect(new_panel->surface,0,SDL_MapRGB(new_panel->surface->format, red, green, blue)) != 0) {
		sdlErrorPrint("failed to fill rect with color");
		return NULL;
	}
	return new_panel;
}

/* create an image with certain location, size and color from a specific location of an image surface */
Widget * create_image(int x, int y, int width, int height, SDL_Surface * imgSurface,
		int imgx, int imgy){
	Widget * new_image = createGeneralWidget(x, y, width, height, IMAGE); /* create widget with general setting */
	if(new_image == NULL){
		return NULL;
	}
	new_image->surface = imgSurface; /* the surface of the image will be the given image surface */
	SDL_Rect imgRect = {imgx, imgy, width ,height}; /* create the imgRect - location of the image in the image surface */
	new_image->imgRect= imgRect;

	return new_image;
}

/* create a button with certain location, size and color from a specific location of an image surface
 * and set it as selected or not selected */
Widget * create_button(Sint16 x, Sint16 y, Uint16 width, Uint16 height, SDL_Surface * img,
		Sint16 imgSx, Sint16 imgSy, Sint16 imgNSx, Sint16 imgNSy, int isSelected){
	Widget * new_button = createGeneralWidget(x, y, width, height, BUTTON);/* create widget with general setting */
	if(new_button == NULL){
		return NULL;
	}
	new_button->surface = img; /* the surface of the image will be the given image surface */
	/* set the location of the image in the image surface  for the button selecte and not selected image */
	SDL_Rect button_selected_rect = {imgSx, imgSy, width ,height};
	SDL_Rect button_non_selected_rect = {imgNSx, imgNSy, width ,height};
	new_button->buttonSelectedRect = button_selected_rect;
	new_button->buttonNonSelectedRect = button_non_selected_rect;
	new_button->isButtonSelected = isSelected; /* update if the button is selected */
	/* update the imgRect according to whether the button is selected or not */
	if (isSelected)
		new_button->imgRect = button_selected_rect;
	else
		new_button->imgRect = button_non_selected_rect;
	return new_button;
}


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
		if (widget->surface != NULL && (widget->type == PANEL || widget->type == WINDOW)){ //window ????
			SDL_FreeSurface(widget->surface);
		}
		free(widget);
	}
}
