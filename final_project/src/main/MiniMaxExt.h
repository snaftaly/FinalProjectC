#ifndef MINIMEXEXT_H_
#define MINIMEXEXT_H_

#include <stdio.h>
#include <stdlib.h>
#include "MiniMax.h"
#include "ListUtilsExt.h"
#include "../services/ErrorHandling.h"

/* defines* */
/* The value the evaluate will return in case of an error */
#define EVALERR 200000

/* function declarations */

struct MiniMaxResult getBestChildWithPruning(
    	void* state,
		unsigned int maxDepth,
		ListRef (*getChildren) (void* state),
		FreeFunc freeState,
		int (*evaluate) (void* state),
		int isMaxPlayer,
		int alpha,
		int beta);

#endif /* MINIMEXEXT_H_ */
