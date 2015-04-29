CFLAGS=-Wall -std=c99 -pedantic-errors -lm -g
SDL_L=`sdl-config --libs`
SDL_C=`sdl-config --cflags`

MAINDIR=src/main/
FACTDIR=src/factories/
PHEDIR=src/presenters/
SRVDIR=src/services/
STDIR=src/stateTransition/
VTEDIR=src/views/

H_ERR=$(SRVDIR)ErrorHandling.h
H_CnM=$(MAINDIR)CatAndMouse.h
H_LST=$(MAINDIR)ListUtils.h
H_LSTEXT=$(H_LST) $(MAINDIR)ListUtilsExt.h
H_MINIMAX=$(H_LST) $(MAINDIR)MiniMax.h
H_MINIMAXEXT=$(H_MINIMAX) $(MAINDIR)MiniMaxExt.h $(MAINDIR)ListUtilsExt.h
H_GRID=$(SRVDIR)gridDataUtils.h $(H_CnM)
H_WIDGFACT=$(FACTDIR)widgetsFactory.h $(H_CnM)
H_WIDGUTL=$(SRVDIR)widgetUtils.h $(H_WIDGFACT)
H_GUIUTL=$(SRVDIR)guiUtils.h $(H_WIDGUTL) $(H_LSTEXT)
H_CnMUTL=$(SRVDIR)CatAndMouseUtils.h $(H_GRID) $(H_MINIMAXEXT)
H_MVPUTL=$(SRVDIR)MVPutils.h $(H_CnMUTL) $(H_GUIUTL)


all: CatAndMouse ListUtilsDemo MiniMaxDemo

clean:
	-rm CatAndMouse MiniMaxDemo ListUtilsDemo CatAndMouse.o GUIsFactory.o PHEs.o VTEs.o stateTransition.o MVPutils.o CatAndMouseUtils.o guiUtils.o widgetUtils.o widgetsFactory.o gridDataUtils.o MiniMaxDemo.o MiniMax.o ListUtilsDemo.o ListUtils.o ErrorHandling.o 

CatAndMouse: CatAndMouse.o GUIsFactory.o PHEs.o VTEs.o stateTransition.o MVPutils.o CatAndMouseUtils.o guiUtils.o widgetUtils.o widgetsFactory.o gridDataUtils.o MiniMax.o ListUtils.o ErrorHandling.o
	gcc -o $@ $^ $(CFLAGS) $(SDL_L)

MiniMaxDemo: MiniMaxDemo.o MiniMax.o ListUtils.o ErrorHandling.o
	gcc -o $@ $^ $(CFLAGS) $(SDL_L)

ListUtilsDemo: ListUtils.o ListUtilsDemo.o ErrorHandling.o
	gcc -o $@ $^ $(CFLAGS) $(SDL_L)

CatAndMouse.o: $(MAINDIR)CatAndMouse.c $(FACTDIR)GUIsFactory.h $(PHEDIR)PHEs.h $(VTEDIR)VTEs.h $(STDIR)stateTransition.h $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(MAINDIR)$*.c $(SDL_C)

GUIsFactory.o: $(FACTDIR)GUIsFactory.c $(FACTDIR)GUIsFactory.h $(PHEDIR)PHEs.h $(VTEDIR)VTEs.h $(STDIR)stateTransition.h $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(FACTDIR)$*.c $(SDL_C)

PHEs.o: $(PHEDIR)PHEs.c $(PHEDIR)PHEs.h $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(PHEDIR)$*.c $(SDL_C)

VTEs.o: $(VTEDIR)VTEs.c $(VTEDIR)VTEs.h $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(VTEDIR)$*.c $(SDL_C)

stateTransition.o: $(STDIR)stateTransition.c $(STDIR)stateTransition.h $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(STDIR)$*.c $(SDL_C)

MVPutils.o: $(SRVDIR)MVPutils.c $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(SRVDIR)$*.c $(SDL_C)

CatAndMouseUtils.o: $(SRVDIR)CatAndMouseUtils.c $(H_CnMUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(SRVDIR)$*.c $(SDL_C)

guiUtils.o: $(SRVDIR)guiUtils.c $(SRVDIR)guiUtils.h $(H_GUIUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(SRVDIR)$*.c $(SDL_C)

widgetUtils.o: $(SRVDIR)widgetUtils.c $(H_WIDGUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(SRVDIR)$*.c $(SDL_C)

widgetsFactory.o: $(FACTDIR)widgetsFactory.c $(H_WIDGFACT) $(H_ERR)
	gcc -c $(CFLAGS) $(FACTDIR)$*.c $(SDL_C)

gridDataUtils.o: $(SRVDIR)gridDataUtils.c  $(H_GRID) $(H_ERR)
	gcc -c $(CFLAGS) $(SRVDIR)$*.c $(SDL_C)

MiniMaxDemo.o: $(MAINDIR)MiniMaxDemo.c $(MAINDIR)MiniMax.h $(MAINDIR)ListUtils.h $(H_ERR)
	gcc -c $(CFLAGS) $(MAINDIR)$*.c $(SDL_C)

MiniMax.o: $(MAINDIR)MiniMax.c $(H_MINIMAXEXT) $(H_ERR)
	gcc -c $(CFLAGS) $(MAINDIR)$*.c $(SDL_C)

ListUtilsDemo.o: $(MAINDIR)ListUtilsDemo.c $(MAINDIR)ListUtils.h $(H_ERR)
	gcc -c $(CFLAGS) $(MAINDIR)$*.c $(SDL_C)

ListUtils.o: $(MAINDIR)ListUtils.c $(H_LSTEXT) $(H_ERR)
	gcc -c $(CFLAGS) $(MAINDIR)$*.c $(SDL_C)

ErrorHandling.o: $(SRVDIR)ErrorHandling.c $(H_ERR)
	gcc -c $(CFLAGS) $(SRVDIR)$*.c $(SDL_C)
