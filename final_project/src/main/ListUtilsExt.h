#ifndef TREEUTILS_H_
#define TREEUTILS_H_
#include "ListUtils.h"
#include "ErrorHandling.h"

/*
 * struct List implementation:
 * The list itself is composed of nodes of type struct List.
 * Each item holds two fields:
 * - void * data - to hold the node's data pointer.
 * - ListRef next - to hold the pointer to the next node.
 */
struct List{
    void *data;
    ListRef next;
    ListRef child;
    ListRef parent;
};

/* function declarations */
ListRef addChildNode(ListRef parent, void * data);
void freeTree(ListRef root, FreeFunc freeData);
int treeDFS(ListRef root, int (* treeNodePreFunction) (ListRef node),
		int (* treeNodePostFunction) (ListRef node));

#endif /* TREEUTILS_H_ */
