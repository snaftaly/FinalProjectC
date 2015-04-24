#ifndef GUI_UTILS_H_
#define GUI_UTILS_H_

/* includes */
#include "../main/ListUtilsExt.h"
#include "../services/WidgetUtils.h"


/*** define constants: ***/
/* button numbers of guis */
#define MAIN_MENU_NUM_BUTTONS 5
#define COMMON_MENU_NUM_BUTTONS 3
#define ERR_MSG_NUM_BUTTONS 1
#define WB_NUM_BUTTONS 8
#define WB_TOP_PANEL_NUM_BUTTONS 3
#define WB_SIDE_PANEL_NUM_BUTTONS 5
#define PG_NUM_BUTTONS 6
#define PG_TOP_PANEL_NUM_BUTTONS 1
#define PG_SIDE_PANEL_NUM_BUTTONS 5
#define PG_NUM_LABELS 4
#define NUM_GRID_ITEMS 8
/* numbering for buttons - for internal use*/
#define FIRST_BUTTON 0
#define REGULAR_BUTTON 0
#define UP_ARROW_BUTTON 1
#define DOWN_ARROW_BUTTON 2
/* size of buttons, labels and gaps between them for menus */
#define MENU_BUTTON_W 260
#define MENU_BUTTON_H 60
#define ARROW_BUTTON_X 220
#define ARROW_BUTTON_Y 30
#define MENU_TITLE_W 460
#define MENU_TITLE_H 80
#define ERR_MSG_TITLE_W 500
#define MISSING_ITEM_MSG_W 440
#define MENU_BUTTON_GAP 20
#define MENU_TITLE_X_GAP 1
#define MENU_TITLE_Y_GAP 20

/* size of buttons, labels, pnaels and gaps between them for three part layout */
#define TOP_PANEL_H 200
#define SIDE_PANEL_W 200
#define SIDE_PANEL_H 590
#define PANEL_BUTTON_W 176
#define PANEL_BUTTON_H 80
#define PANEL_WIDGET_Y_GAP 20
#define GRID_SIZE 590
#define GRID_SQUARE_SIZE 80
#define GRID_GAP_SIZE 5
/* sizes for world builder */
#define WB_TITLE_W 240
#define WB_TITLE_H 60
#define WB_BUTTON_X_GAP 50
/* sizes for play game */
#define GAME_OVER_LABEL_W 490
#define GAME_OVER_LABEL_H 100
#define GAME_INFO_LABEL_H 40
#define GAME_INFO_LABEL_W 180
#define STATE_LABEL_W 500
#define STATE_LABEL_H 40
#define DIGIT_LABEL_W 30
#define CAT_MOUSE_DIF 35
#define PAUSE_BUTTON_H 50
#define PAUSE_BUTTON_W 400

/* colors used in the game*/
#define WINDOW_RED 193
#define WINDOW_GREEN 201
#define WINDOW_BLUE 255

#define PANEL_RED 215
#define PANEL_GREEN 225
#define PANEL_BLUE 255

#define TR_RED  0
#define TR_GREEN 255
#define TR_BLUE 0


/**** functions declarations ****/

/** functions used for displaying the gui **/
void drawGui(ListRef UITree);
void blitUpToWindow(void * widget);
void blitChildToParentWidget(Widget * childWidget, Widget * parentWidget);
/* functions used when traversing the UI tree */
int addChildWidgetsToParent(ListRef);
int calcAbsWidgetXY(ListRef node);



/** size calculation functions **/
int calcMenuPanelX(int titleWidth);
int calcMenuPanelY(int numButtons);
int calcMenuPanelWidth(int titleWidth);
int calcMenuPanelHeight(int numButtons);
int calcMenuButtonX(int titleWidth);
int calcMenuButtonY();
int calcTopButtonX();
int calcTopButtonY();
int calcSideButtonX();
int calcSideButtonY();
int calcWBtitleX(int title_W);
int calcErrPanelY();
int calcErrPanelHeight();
int calcMisItemMsgX(int messageWidth, int titleWidth);
int calcMisItemMsgY();
int calcMenuButtonX(int titleWidth);
int calcErrMsgButtonY();
int calcGameOverLabelX();
int calcGameOverLabelY();
int calcPauseButtonX();
int calcPauseButtonY();
int calcMoveLabelX(int isCatCurrPlayer);
int calcTensLabelX(int isCatCurrPlayer);
int calcUnitsLabelX(int isCatCurrPlayer);
int calcStateLabelX();
int calcStateLabelY();

#endif /* GUI_UTILS_H_ */
