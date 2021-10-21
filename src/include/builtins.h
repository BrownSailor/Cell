#ifndef BUILTINS_H
#define BUILTINS_H

#include "tree.h"

void builtInInit(std::vector<Tree *> &list);
void builtInRegister(std::vector<Tree *> &list, std::string name, Tree *(*functionPointer)(Visitor *visitor, Tree *tree, std::vector<Tree *> &list));
Tree *functionPointerPrint(Visitor *visitor, Tree *tree, std::vector<Tree *> &list);

#endif