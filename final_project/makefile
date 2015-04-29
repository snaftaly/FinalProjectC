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
H_GRID=$(SRVDIR)GridDataUtils.h $(H_CnM)
H_WIDGFACT=$(FACTDIR)WidgetsFactory.h $(H_CnM)
H_WIDGUTL=$(SRVDIR)WidgetUtils.h $(H_WIDGFACT)
H_GUIUTL=$(SRVDIR)GUIUtils.h $(H_WIDGUTL) $(H_LSTEXT)
H_CnMUTL=$(SRVDIR)CatAndMouseUtils.h $(H_GRID) $(H_MINIMAXEXT)
H_ALLPHEs=$(PHEDIR)MenusAndErrPresenters.h $(PHEDIR)PlayGamePresenter.h $(PHEDIR)WorldBuilderPresenter.h
H_ALLVTEs=$(VTEDIR)MenusAndErrViews.h $(VTEDIR)PGandWBViews.h
H_MVPUTL=$(SRVDIR)MVPutils.h $(SRVDIR)LogEventItems.h $(H_ALLPHEs) $(H_ALLVTEs) $(H_CnMUTL) $(H_GUIUTL)


all: CatAndMouse ListUtilsDemo MiniMaxDemo

clean:
	-rm CatAndMouse MiniMaxDemo ListUtilsDemo CatAndMouse.o GUIsFactory.o MenusPresenters.o WorldBuilderPresenter.o ErrMsgPresenter.o PlayGamePresenter.o PlayGameView.o ErrMsgView.o WorldBuilderView.o MenusViews.o StateTransition.o MVPutils.o CatAndMouseUtils.o GUIUtils.o WidgetUtils.o WidgetsFactory.o GridDataUtils.o MiniMaxDemo.o MiniMax.o ListUtilsDemo.o ListUtils.o ErrorHandling.o 

CatAndMouse: CatAndMouse.o GUIsFactory.o MenusPresenters.o WorldBuilderPresenter.o ErrMsgPresenter.o PlayGamePresenter.o PlayGameView.o ErrMsgView.o WorldBuilderView.o MenusViews.o StateTransition.o MVPutils.o CatAndMouseUtils.o GUIUtils.o WidgetUtils.o WidgetsFactory.o GridDataUtils.o MiniMax.o ListUtils.o ErrorHandling.o
	gcc -o $@ $^ $(CFLAGS) $(SDL_L)

MiniMaxDemo: MiniMaxDemo.o MiniMax.o ListUtils.o ErrorHandling.o
	gcc -o $@ $^ $(CFLAGS) $(SDL_L)

ListUtilsDemo: ListUtils.o ListUtilsDemo.o ErrorHandling.o
	gcc -o $@ $^ $(CFLAGS) $(SDL_L)

CatAndMouse.o: $(MAINDIR)CatAndMouse.c $(FACTDIR)GUIsFactory.h $(STDIR)StateTransition.h $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(MAINDIR)$*.c $(SDL_C)

GUIsFactory.o: $(FACTDIR)GUIsFactory.c $(FACTDIR)GUIsFactory.h $(STDIR)StateTransition.h $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(FACTDIR)$*.c $(SDL_C)

PlayGamePresenter.o: $(PHEDIR)PlayGamePresenter.c $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(PHEDIR)$*.c $(SDL_C)

ErrMsgPresenter.o: $(PHEDIR)ErrMsgPresenter.c $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(PHEDIR)$*.c $(SDL_C)

WorldBuilderPresenter.o: $(PHEDIR)WorldBuilderPresenter.c $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(PHEDIR)$*.c $(SDL_C)

MenusPresenters.o: $(PHEDIR)MenusPresenters.c $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(PHEDIR)$*.c $(SDL_C)

PlayGameView.o: $(VTEDIR)PlayGameView.c $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(VTEDIR)$*.c $(SDL_C)

ErrMsgView.o: $(VTEDIR)ErrMsgView.c $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(VTEDIR)$*.c $(SDL_C)

WorldBuilderView.o: $(VTEDIR)WorldBuilderView.c $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(VTEDIR)$*.c $(SDL_C)

MenusViews.o: $(VTEDIR)MenusViews.c $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(VTEDIR)$*.c $(SDL_C)

StateTransition.o: $(STDIR)StateTransition.c $(STDIR)StateTransition.h $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(STDIR)$*.c $(SDL_C)

MVPutils.o: $(SRVDIR)MVPutils.c $(H_MVPUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(SRVDIR)$*.c $(SDL_C)

CatAndMouseUtils.o: $(SRVDIR)CatAndMouseUtils.c $(H_CnMUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(SRVDIR)$*.c $(SDL_C)

GUIUtils.o: $(SRVDIR)GUIUtils.c $(SRVDIR)GUIUtils.h $(H_GUIUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(SRVDIR)$*.c $(SDL_C)

WidgetUtils.o: $(SRVDIR)WidgetUtils.c $(H_WIDGUTL) $(H_ERR)
	gcc -c $(CFLAGS) $(SRVDIR)$*.c $(SDL_C)

WidgetsFactory.o: $(FACTDIR)WidgetsFactory.c $(H_WIDGFACT) $(H_ERR)
	gcc -c $(CFLAGS) $(FACTDIR)$*.c $(SDL_C)

GridDataUtils.o: $(SRVDIR)GridDataUtils.c  $(H_GRID) $(H_ERR)
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
