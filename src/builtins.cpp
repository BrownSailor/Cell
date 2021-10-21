#include "include/visitor.h"

void builtInInit(std::vector<Tree *> &list)
{
    builtInRegister(list, "print", functionPointerPrint);
}

void builtInRegister(std::vector<Tree *> &list, std::string name, Tree *(*functionPointer)(Visitor *visitor, Tree *tree, std::vector<Tree *> &list))
{
    Tree *funcPtrPrintVar = initTree(TREE_VAR);
    funcPtrPrintVar->name = name;
    funcPtrPrintVar->functionPtr = functionPointer;

    list.push_back(funcPtrPrintVar);
}

Tree *functionPointerPrint(Visitor *visitor, Tree *tree, std::vector<Tree *> &list)
{
    std::cout << "yo\n";
    return tree;
}