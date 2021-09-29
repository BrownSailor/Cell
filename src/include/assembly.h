#ifndef ASM_H
#define ASM_H

#include <string>
#include "tree.h"

std::string asmInit(Tree *tree);
std::string asmMain(Tree *tree);
std::string asmCompound(Tree *tree);
std::string asmFuncDef(Tree *tree);
std::string asmAssign(Tree *tree);
std::string asmVariable(Tree *tree);
std::string asmInt(Tree *tree);
std::string asmCall(Tree *tree);

#endif