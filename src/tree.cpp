#include "include/tree.h"

/* initTree
 *    Purpose: create and initialize an abstract syntax tree
 * Parameters: unsigned int
 *    Returns: tree
 */
Tree *initTree(unsigned int type)
{
    Tree *tree = new Tree();
    tree->type = type;

    return tree;
}

/* printTree
 *    Purpose: print a BFS traversal of a tree
 * Parameters: tree
 *    Returns: None
 */
void printTree(Tree *root)
{
    std::queue<Tree *> q;
    q.push(root);

    while (!q.empty())
    {
        int nodeCount = q.size();
        Tree *node;

        bool b = false;

        while (nodeCount)
        {
            node = q.front();

            if (node->name.size())
            {
                std::cout << node->name << " ";
                b = true;
            }

            if (node->intValue)
            {
                std::cout << node->intValue << " ";
                b = true;
            }

            q.pop();

            for (size_t i = 0; i < node->children.size(); i++)
            {
                q.push(node->children[i]);
            }

            if (node->data)
            {
                q.push(node->data);
            }

            nodeCount--;
        }

        if (b)
        {
            std::cout << "\n";
        }
    }
}

/* treeTypeToString
 *    Purpose: create a string representation of a tree type
 * Parameters: unsigned int tree type
 *    Returns: string
 */
std::string treeTypeToString(unsigned int type)
{
    switch (type)
    {
    case TREE_COMPOUND:
        return "TREE_COMPOUND";

    case TREE_FUNC_DEC:
        return "TREE_FUNC_DEC";

    case TREE_ASSIGN:
        return "TREE_ASSIGN";

    case TREE_INT:
        return "TREE_INT";

    case TREE_VAR:
        return "TREE_VAR";

    case TREE_CALL:
        return "TREE_CALL";

    case TREE_STATEMENT:
        return "TREE_STATEMENT";

    case TREE_STRING:
        return "TREE_STRING";

    case TREE_ACCESS:
        return "TREE_ACCESS";

    case TREE_NO_OP:
        return "TREE_NO_OP";

    default:
        break;
    }

    return "";
}

/* treeToString
 *    Purpose: create a string representation of a tree
 * Parameters: tree
 *    Returns: string
 */
std::string treeToString(Tree *tree)
{
    std::string typeName = treeTypeToString(tree->type);
    return "TOKEN (" + tree->name + ") " + typeName;
}