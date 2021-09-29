#include "include/tree.h"

/* initTree
Tree *initTree(int type);
 *    Purpose: create and initialize an abstract syntax tree
 * Parameters: type of node
 *    Returns: tree
 */
Tree *initTree(int type)
{
    Tree *tree = new Tree();
    tree->type = type;

    return tree;
}
