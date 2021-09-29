#ifndef TREE_H
#define TREE_H
#include <vector>
#include <string>

enum TreeType
{
    TREE_COMPOUND,  // 0
    TREE_FUNC_DEF,  // 1
    TREE_ASSIGN,    // 2
    TREE_TYPE_DEF,  // 3
    TREE_INT,       // 4
    TREE_VARIABLE,  // 5
    TREE_CALL,      // 6
    TREE_STATEMENT, // 7
    TREE_NO_OP      // 8
};

struct Tree
{
    unsigned int type;
    std::vector<Tree *> children;
    std::string name;
    Tree *data;
    int dataType;
    int intValue;
};

Tree *initTree(int type);

#endif