#include <stdio.h>
#include <stdlib.h>
#include "MiniMax.h"


/**
 * Returns the best child of the given state, resulting from the MiniMax computation up to maxDepth.
 * The getChildren function will be used to get each state's children.
 * The freeState function will be used to deallocate any lists returned by GetChildren.
 * The evaluate function will be used to evaluate the state score.
 * The player who's turn it is to play at the given state is assumed to be the MAX player if and only if isMaxPlayer is non-zero,
 * otherwise, it is assumed to be the MIN player.
 *
 * Will NOT crash in case one of the given functions fails but result will be undefined.
 */
struct MiniMaxResult getBestChildWithPruning(
    	void* state,
		unsigned int maxDepth,
		ListRef (*getChildren) (void* state),
		FreeFunc freeState,
		int (*evaluate) (void* state),
		int isMaxPlayer,
		int alpha,
		int beta)
{
	struct MiniMaxResult res; /* will hold the result */
	if (maxDepth == 0){ /* if reached the maximal depth of the recursion */
		res.value = evaluate(state); /* res value will be the state's evaluation */
		res.index = -1; /* res index will be a non-valid index -1 (since there are no children) */
		return res;
	}

	ListRef childsList = getChildren(state); /* get the state's childs list */
	if (childsList == NULL){ /* getChildren failed */
		res.value = 0; /* this value will not be used! */
		res.index = -2; /* assign a non-valid index -2 that indicates the failure */
		return res;
	}

	if (isEmpty(childsList)){ /* if the state is a leaf then its child list will be empty */
		res.value = evaluate(state); /* res value will be the state's evaluation */
		res.index = -1; /* res index will be a non-valid index -1 (since there are no children) */
	}
	else {
		ListRef childsListTemp = childsList; /* create a temp list in order to get childsList on O(1) */

		/* init res.value to be more extreme than the possible state's evaluation: */
		res.value = isMaxPlayer ? MIN_EVALUATION-1 : MAX_EVALUATION+1;
		int ndx = 0; /* init the child index */
		/* go over the children list: */
		while (childsListTemp != NULL){
			struct MiniMaxResult childRes;/* will hold the value and index of the best child found so far */
			void* childState = headData(childsListTemp); /* get the child data (state) */
			/* a recursive call to get the child's best child */
			childRes = getBestChildWithPruning(childState, maxDepth-1, getChildren,
					freeState, evaluate, 1-isMaxPlayer, alpha, beta);
			if (childRes.index == -2){ /* getBestChild recursive call failed */
				res.index = -2; /* assign a non-valid index -2 that indicates the failure */
				return res;
			}
			/* check if the current child's value is better than the res.value achieved so far: */
			if ((isMaxPlayer && childRes.value > res.value) || (!isMaxPlayer && childRes.value < res.value)){
				res.value = childRes.value; /* update the child's value in res.value */
				res.index = ndx; /* update the child's index in res.index */
			}
			//test pruning
			if (isMaxPlayer){
				alpha = childRes.value > alpha ? childRes.value : alpha;
				if (alpha >= beta)
					break;
			}
			else{ //isMinPlayer
				beta = childRes.value < beta ? childRes.value : beta;
				if (alpha >= beta)
					break;
			}
			//end test
			ndx++; /* raise the ndx counter */
			childsListTemp=tail(childsListTemp); /* get the next child of the list */
		}/* end of while loop */
	}

	destroyList(childsList, freeState); /* once finished going over childsList, free its memory */
	return res; /* return the result */
}


struct MiniMaxResult getBestChild(
    	void* state,
		unsigned int maxDepth,
		ListRef (*getChildren) (void* state),
		FreeFunc freeState,
		int (*evaluate) (void* state),
		int isMaxPlayer)
{
	int alpha = MIN_EVALUATION-1, beta = MAX_EVALUATION+1;
	struct MiniMaxResult res = getBestChildWithPruning(state, maxDepth, getChildren,
			freeState, evaluate, isMaxPlayer, alpha, beta);
	return res;
}


//struct MiniMaxResult getBestChild(
//    	void* state,
//		unsigned int maxDepth,
//		ListRef (*getChildren) (void* state),
//		FreeFunc freeState,
//		int (*evaluate) (void* state),
//		int isMaxPlayer)
//{
//	struct MiniMaxResult res; /* will hold the result */
//	if (maxDepth == 0){ /* if reached the maximal depth of the recursion */
//		res.value = evaluate(state); /* res value will be the state's evaluation */
//		res.index = -1; /* res index will be a non-valid index -1 (since there are no children) */
//		return res;
//	}
//
//	ListRef childsList = getChildren(state); /* get the state's childs list */
//	if (childsList == NULL){ /* getChildren failed */
//		res.value = 0; /* this value will not be used! */
//		res.index = -2; /* assign a non-valid index -2 that indicates the failure */
//		return res;
//	}
//
//	if (isEmpty(childsList)){ /* if the state is a leaf then its child list will be empty */
//		res.value = evaluate(state); /* res value will be the state's evaluation */
//		res.index = -1; /* res index will be a non-valid index -1 (since there are no children) */
//	}
//	else {
//		ListRef childsListTemp = childsList; /* create a temp list in order to get childsList on O(1) */
//
//		/* init res.value to be more extreme than the possible state's evaluation: */
//		res.value = isMaxPlayer ? MIN_EVALUATION-1 : MAX_EVALUATION+1;
//		int ndx = 0; /* init the child index */
//
//		/* go over the children list: */
//		while (childsListTemp != NULL){
//			struct MiniMaxResult childRes;/* will hold the value and index of the best child found so far */
//			void* childState = headData(childsListTemp); /* get the child data (state) */
//			/* a recursive call to get the child's best child */
//			childRes = getBestChild(childState, maxDepth-1, getChildren, freeState, evaluate, 1-isMaxPlayer);
//			if (childRes.index == -2){ /* getBestChild recursive call failed */
//				res.index = -2; /* assign a non-valid index -2 that indicates the failure */
//				return res;
//			}
//			/* check if the current child's value is better than the res.value achieved so far: */
//			if ((isMaxPlayer && childRes.value > res.value) || (!isMaxPlayer && childRes.value < res.value)){
//				res.value = childRes.value; /* update the child's value in res.value */
//				res.index = ndx; /* update the child's index in res.index */
//			}
//
//			ndx++; /* raise the ndx counter */
//			childsListTemp=tail(childsListTemp); /* get the next child of the list */
//		}/* end of while loop */
//	}
//
//	destroyList(childsList, freeState); /* once finished going over childsList, free its memory */
//	return res; /* return the result */
//}
