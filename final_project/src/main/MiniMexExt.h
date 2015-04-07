#ifndef MINIMEXEXT_H_
#define MINIMEXEXT_H_

#include <stdio.h>
#include <stdlib.h>
#include "MiniMax.h"

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
