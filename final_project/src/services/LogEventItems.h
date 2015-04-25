#ifndef LOGITEMS_H_
#define LOGITEMS_H_

#include "GridDataUtils.h"

/* an enumeration of all the logical event types .*/
typedef enum{
	SELECT_CURR_BUTTON,
	MARK_NEXT_BUTTON,
	SELECT_BUTTON_NUM,
	MARK_VALUES_BUTTON,
	INCREASE_VALUE,
	DECREASE_VALUE,
	GO_UP,
	GO_RIGHT,
	GO_LEFT,
	GO_DOWN,
	SELECT_SQUARE,
	DO_QUIT,
	NO_EVENT
} logicalEventType;


/**** structs ****/

/* a struct fot the logical event
 * holds event type, and if necessary will hold the
 * button number pressed or the grid position pressed */
typedef struct logicalEvent{
	logicalEventType type;
	int buttonNum;
	gridItemPosition gridPos;
}logicalEvent;

typedef struct logicalEvent * logicalEventRef;


#endif /* LOGITEMS_H_ */
