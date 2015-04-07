CFLAGS=-Wall -std=c99 -pedantic-errors -lm -g 
SDL_L=`sdl-config --libs`
SDL_C=`sdl-config --cflags`

all: CatAndMouse

clean:
	-rm CatAndMouse.o MVP.o MVP_utils.o gui_utils.o ListUtils.o widget_utils.o ErrorHandling.o CatAndMouse

CatAndMouse: CatAndMouse.o machinePlayer_utils.o MVP.o MVP_utils.o gui_utils.o widget_utils.o MiniMax.o ListUtils.o ErrorHandling.o
	gcc -o $@ $^ $(CFLAGS) $(SDL_L)

CatAndMouse.o: src/main/CatAndMouse.c src/gui/MVP_utils.h src/gui/gui_utils.h src/gui/widget_utils.h src/main/ListUtils.h src/main/ListUtilsExt.h src/main/ErrorHandling.h
	gcc -c src/main/CatAndMouse.c $(CFLAGS) $(SDL_C)

machinePlayer_utils.o: src/main/machinePlayer_utils.c src/main/machinePlayer_utils.h src/gui/MVP_utils.h src/gui/gui_utils.h src/gui/widget_utils.h src/main/MiniMax.h src/main/ListUtils.h src/main/ListUtilsExt.h src/main/ErrorHandling.h
	gcc -c src/main/machinePlayer_utils.c $(CFLAGS) $(SDL_C)

MVP.o: src/gui/MVP.c src/gui/gui_utils.h src/gui/widget_utils.h src/main/ListUtils.h src/main/ListUtilsExt.h src/main/ErrorHandling.h
	gcc -c src/gui/MVP.c $(CFLAGS) $(SDL_C)

MVP_utils.o: src/gui/MVP_utils.c src/gui/gui_utils.h src/gui/widget_utils.h src/main/ListUtils.h src/main/ListUtilsExt.h src/main/ErrorHandling.h
	gcc -c src/gui/MVP_utils.c $(CFLAGS) $(SDL_C)

gui_utils.o: src/gui/gui_utils.c src/gui/gui_utils.h src/gui/widget_utils.h src/main/ListUtils.h src/main/ListUtilsExt.h src/main/ErrorHandling.h
	gcc -c src/gui/gui_utils.c $(CFLAGS) $(SDL_C)

MiniMax.o: src/main/MiniMax.c src/main/MiniMax.h src/main/ErrorHandling.h
	gcc -c $(CFLAGS) src/main/MiniMax.c

ListUtils.o: src/main/ListUtils.c src/main/ListUtils.h src/main/ListUtilsExt.h src/main/ErrorHandling.h
	gcc -c $(CFLAGS) src/main/ListUtils.c

widget_utils.o: src/gui/widget_utils.c src/gui/widget_utils.h
	gcc -c src/gui/widget_utils.c $(CFLAGS) $(SDL_C)

ErrorHandling.o: src/main/ErrorHandling.c src/main/ErrorHandling.h
	gcc -c src/main/ErrorHandling.c $(CFLAGS) $(SDL_C)
