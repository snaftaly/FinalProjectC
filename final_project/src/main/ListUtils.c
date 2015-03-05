#include <stdio.h>
#include <stdlib.h>
#include "ListUtilsExt.h"

/**
 * Allocate a new list with the given data pointer at the head.
 * The list does not support NULL data - in this case if the data is NULL then an empty list will be created.
 *
 * May return NULL if malloc fails.
 */
ListRef newList(void* headData){
	ListRef newList = (ListRef)malloc(sizeof(struct List)); /* allocate memory for the first item */
	if (newList==NULL){ /* malloc failed */
		perrorPrint("malloc");
	    return NULL;
	}
	/* data and next will point to NULL: */
	newList->data = NULL;
	newList->next = NULL;
	newList->child = NULL;
	newList->parent = NULL;

	/* if headData is not NULL, put it in the list data field */
	if (headData != NULL){
		newList->data = headData;
	}
	return newList;
}

/**
 * Return the data pointer held by the head of the list or NULL if the list is empty.
 */
void* headData(ListRef list){
	if (isEmpty(list)){
		return NULL;
	}
	return  list->data;
}

/**
 * Return the tail of the given list or NULL if the list has less than 2 elements.
 * E.g. if the list holds the addresses [1,2,3] then tail(list) will return the sub-list [2,3].
 */
ListRef tail(ListRef list){
	if (isEmpty(list)){
		return NULL;
	}
	return list->next; /* list->Next is the pointer to the list's tail.*/
}

/**
 * Append the given data to the end of the list and return a pointer to the sub-list containing only this data.
 * If the given data is NULL no change will be made to the list and NULL will be returned.
 * E.g. if the list holds the addresses [1,2,3] then after calling append(list, (void*)4) the list will hold [1,2,3,4]
 * and will return the sub-list [4].
 *
 * May return NULL if malloc fails.
 */
ListRef append(ListRef list, void* data){
	if (data == NULL){
		return list;
	}
	if (isEmpty(list)){
		/* in this implementation an empty list is a one node list
		 * that has its data pointer points to NULL. Thus adding the
		 * data to an empty list will be done by changing the data
		 * pointer to the new data. */
		list->data = data;
		return list;
	}
	else{ /* list is not empty */
		ListRef appendList = newList(data); /* create a new list node */
		if (appendList==NULL){ /* failed to create the list node */
			return NULL;
		}
		/* Loop until reached the last node of the list.
		 * If the list passed to the function is the last node
		 * of the list, then this loop will take O(1) time.
		 */
		while (list->next!=NULL){ /* while last node not reached */
			list = list->next; /* advance to the next node */
		}
		list->next = appendList; /* append the node at the end of the list */
		return appendList; /* return the node appended */
	}
}

/**
 * Free all memory allocated to the list and all its sub-lists.
 * Will use the given freeData function in order to free all the data contained in the list.
 * Given list assumed to have been returned from the 'newList' function (i.e. not a sub-list) -
 * otherwise all operations on the original list may have undefined behaviour.
 *
 * In this implementation, an empty list is a list node with NULL data pointer and NULL next pointer,
 * thus if reached an empty list, we just need to free the list node itself. If the list is not empty
 * then the data pointer != NULL, thus has to be freed first.
 */
void destroyList(ListRef list, FreeFunc freeData){
	if (list != NULL){ /* if reach a real list node */
		if (!isEmpty(list)){ /* list->data != NULL */
			destroyList(list->next, freeData); /* a recursive call to the next item */
			freeData(list->data); /* use freeData function to free the data of the item */
		}
		free(list); /* free the list node itself */
	}
}

/**
 * Return zero if the list has any data or non-zero if it is empty.
 * I this implementation, and empty list is a list node with NULL data pointer.
 */
int isEmpty(ListRef list){
	if (list->data == NULL){ /* if the data pointer is NULL.*/
		return 1;
	}
	return 0;
}

/* ****** extension to tree ****** */

/*
 * adds a new child to a listNode, at the end of the child list, and return it
 */
ListRef addChildNode(ListRef parent, void * data){
	ListRef childNode = newList(data); /* create a new child node */
	if (childNode==NULL){ /* failed to create the list node */
		return NULL;
	}
	if (parent->child == NULL){ /* parent has no children */
		parent->child = childNode;
	}
	else{
		ListRef currChild = parent->child;
		/* Loop until reached the last node of the children list. */
		while (currChild->next!=NULL){ /* while last node not reached */
			currChild = currChild->next; /* advance to the next node */
		}
		currChild->next = childNode; /* append the node at the end of the list */
	}
	childNode->parent = parent;
	return childNode; /* return the node added */
}

int treeDFS(ListRef root, int (* treeNodePreFunction) (ListRef node), int (* treeNodePostFunction) (ListRef node)){
	if (root == NULL)
		return 0;
	int preState = treeNodePreFunction(root);
	if (preState == -1){
		return -1;
	}
	if (root->child != NULL ){
		ListRef curr = root->child;
		while (curr != NULL){
			if (treeDFS(curr, treeNodePreFunction, treeNodePostFunction) == -1)
				return -1;
			curr = curr->next;
		}
	}
	int postState = treeNodePostFunction(root);
	if (postState == -1)
		return -1;
	return 0;
}


void freeTree(ListRef root, FreeFunc freeData){
	if (root != NULL){
		 if (root->child != NULL)
			 freeTree(root->child, freeData);
		 freeTree(root->next, freeData);
		 if (root->data != NULL)
		 	freeData(root->data);
		 free(root);
	}
}
/*
int treeDFS(ListRef root, int (* treeNodeFunction) (ListRef node)){  return value ???
	if (root == NULL)
		return 0;
	int state = treeNodeFunction(root);
	if (state == -1)
		return -1;
	else if (root->child == NULL) // root has no children
		return 0;
	else{ // root->child != NULL
		ListRef curr = root->child;
		while (curr != NULL){
			if (treeDFS(curr, treeNodeFunction) == -1)
				return -1;
			curr = curr->next;
		}
		return 0;
	}
}*/


