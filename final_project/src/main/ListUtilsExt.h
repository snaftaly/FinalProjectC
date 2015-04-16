#ifndef TREEUTILS_H_
#define TREEUTILS_H_
#include "ListUtils.h"
#include "ErrorHandling.h"

/*
 * struct List implementation:
 * The list itself is composed of nodes of type struct List.
 * Each item holds 4 fields:
 * - void * data - to hold the node's data pointer.
 * - ListRef next - to hold the pointer to the next node.
 * - ListRef child - to hold the pointer to the child node (for tree implementation)
 * - ListRef parent - to hold the pointer to the parent node (for tree implementation)
 */
struct List{
    void *data;
    ListRef next;
    ListRef child;
    ListRef parent;
};

/* function declarations */
ListRef addChildNode(ListRef parent, void * data);
int treeDFS(ListRef root, int (* treeNodePreFunction) (ListRef node),
		int (* treeNodePostFunction) (ListRef node));
void freeTree(ListRef root, FreeFunc freeData);
void freeDescendants(ListRef root, FreeFunc freeData);
void freeNode(ListRef node, FreeFunc freeData);

#endif /* TREEUTILS_H_ */
