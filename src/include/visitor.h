#ifndef VISITOR_H
#define VISITOR_H

#include "tree.h"
#include "builtins.h"

struct Visitor
{
    Tree *object;
};

Visitor *initVisitor();
Tree *visitorVisit(Visitor *visitor, Tree *tree, std::vector<Tree *> &list);
Tree *visitorVisitCompound(Visitor *visitor, Tree *tree, std::vector<Tree *> &list);
Tree *visitorVisitFuncDef(Visitor *visitor, Tree *tree, std::vector<Tree *> &list);
Tree *visitorVisitAssign(Visitor *visitor, Tree *tree, std::vector<Tree *> &list);
Tree *visitorVisitVariable(Visitor *visitor, Tree *tree, std::vector<Tree *> &list);
Tree *visitorVisitInt(Visitor *visitor, Tree *tree, std::vector<Tree *> &list);
Tree *visitorVisitCall(Visitor *visitor, Tree *tree, std::vector<Tree *> &list);
Tree *visitorVisitAccess(Visitor *visitor, Tree *tree, std::vector<Tree *> &list);

#endif