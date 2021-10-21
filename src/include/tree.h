#ifndef TREE_H
#define TREE_H
#include <vector>
#include <string>
#include <queue>
#include <iostream>

enum TreeType
{
    TREE_COMPOUND,  // 0
    TREE_FUNC_DEC,  // 1
    TREE_ASSIGN,    // 2
    TREE_INT,       // 3
    TREE_VAR,       // 4
    TREE_CALL,      // 5
    TREE_STATEMENT, // 6
    TREE_STRING,    // 7
    TREE_ACCESS,    // 8
    TREE_NO_OP      // 9
};

struct Visitor;

struct Tree
{
    unsigned int type;
    std::vector<Tree *> children;
    std::string name;
    Tree *data;
    int dataType;
    int intValue;
    unsigned int byteSize;
    Tree *(*functionPtr)(Visitor *visitor, Tree *tree, std::vector<Tree *> &list);
};

Tree *initTree(unsigned int type);
void printTree(Tree *root);
std::string treeTypeToString(unsigned int type);
std::string treeToString(Tree *tree);

#endif