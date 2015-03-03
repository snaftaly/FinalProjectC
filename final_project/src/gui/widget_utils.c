#include "widget_utils.h"

Widget * create_window(int width, int height, int red, int green, int blue){
	Widget * new_window = (Widget *)malloc(sizeof(Widget));
	if(new_window == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	new_window->type = WINDOW;
	new_window->surface  = SDL_SetVideoMode(width, height, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (new_window->surface == NULL){
		sdlErrorPrint("failed to set video mode");
		return NULL;
	}
	if (SDL_FillRect(new_window->surface,0,SDL_MapRGB(new_window->surface->format, red, green, blue)) != 0) {
		sdlErrorPrint("failed to fill rect with color");
		return NULL;
	}
	SDL_Rect location_rect = {0, 0, width ,height};
	new_window->location_rect = location_rect;
	return new_window;
}

Widget * create_panel(int x, int y, int width, int height, int red, int green, int blue){
	Widget * new_panel = (Widget *)malloc(sizeof(Widget));
	if(new_panel == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	new_panel->type = PANEL;
	new_panel->surface  = SDL_CreateRGBSurface(0, width, height, 32,0, 0, 0, 0);
	if (new_panel->surface == NULL){
		sdlErrorPrint("failed to create RGB surface");
	}
	if (SDL_FillRect(new_panel->surface,0,SDL_MapRGB(new_panel->surface->format, red, green, blue)) != 0) {
		sdlErrorPrint("failed to fill rect with color");
		return NULL;
	}
	SDL_Rect location_rect = {x, y, width ,height};
	new_panel->location_rect = location_rect;
	return new_panel;
}

Widget * create_image(int x, int y, int width, int height, SDL_Surface * img,
		int imgx, int imgy){
	Widget * new_image = (Widget *)malloc(sizeof(Widget));
	if(new_image == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	new_image->type = IMAGE;
	new_image->surface = img;
	SDL_Rect location_rect = {x, y, width ,height};
	new_image->location_rect = location_rect;
	SDL_Rect img_rect = {imgx, imgy, width ,height};
	new_image->img_rect= img_rect;
	return new_image;
}

Widget * create_button(int x, int y, int width, int height, SDL_Surface * img,
		int imgSx, int imgSy, int imgNSx, int imgNSy, int isSelected){
	Widget * new_button = (Widget *)malloc(sizeof(Widget));
	if(new_button == NULL){
		perrorPrint("malloc");
		return NULL;
	}
	new_button->type = BUTTON;
	new_button->surface = img;
	SDL_Rect location_rect = {x, y, width ,height};
	new_button->location_rect = location_rect;
	SDL_Rect button_selected_rect = {imgSx, imgSy, width ,height};
	SDL_Rect button_non_selected_rect = {imgNSx, imgNSy, width ,height};
	new_button->isButtonSelected = isSelected;
	if (isSelected)
		new_button->img_rect = button_selected_rect;
	else
		new_button->img_rect = button_non_selected_rect;
	return new_button;
}
