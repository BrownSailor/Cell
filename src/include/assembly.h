#ifndef ASM_H
#define ASM_H

#include <string>
#include "visitor.h"

std::string asmInit(Tree *tree, std::vector<Tree *> &list);
std::string asmMain(Tree *tree, std::vector<Tree *> &list);
std::string asmCompound(Tree *tree, std::vector<Tree *> &list);
std::string asmFuncDef(Tree *tree, std::vector<Tree *> &list);
std::string asmAssign(Tree *tree, std::vector<Tree *> &list);
std::string asmVariable(Tree *tree, std::vector<Tree *> &list, int id);
std::string asmInt(Tree *tree, std::vector<Tree *> &list);
std::string asmCall(Tree *tree, std::vector<Tree *> &list);
std::string asmAccess(Tree *tree, std::vector<Tree *> &list);

#endif