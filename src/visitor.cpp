#include "include/visitor.h"

static Tree *varLookUp(std::vector<Tree *> &list, std::string name)
{
    for (size_t i = 0; i < list.size(); i++)
    {
        Tree *tree = list[i];
        if (tree->type == TREE_VAR && tree->name == name)
        {
            return tree;
        }
    }

    return nullptr;
}

Visitor *initVisitor()
{
    Visitor *visitor = new Visitor();
    visitor->object = initTree(TREE_COMPOUND);

    builtInInit(visitor->object->children);

    return visitor;
}

Tree *visitorVisit(Visitor *visitor, Tree *tree, std::vector<Tree *> &list)
{
    switch (tree->type)
    {
    case TREE_COMPOUND:
        return visitorVisitCompound(visitor, tree, list);

    case TREE_FUNC_DEC:
        return visitorVisitFuncDef(visitor, tree, list);

    case TREE_ASSIGN:
        return visitorVisitAssign(visitor, tree, list);

    case TREE_VAR:
        return visitorVisitVariable(visitor, tree, list);

    case TREE_INT:
        return visitorVisitInt(visitor, tree, list);

    case TREE_CALL:
        return visitorVisitCall(visitor, tree, list);

    case TREE_ACCESS:
        return visitorVisitAccess(visitor, tree, list);

    default:
        std::cerr << "[Visitor]: No method for visiting Tree of type `" << treeTypeToString(tree->type) << "`\n";
        exit(EXIT_FAILURE);
    }
}

Tree *visitorVisitCompound(Visitor *visitor, Tree *tree, std::vector<Tree *> &list)
{
    Tree *compound = initTree(TREE_COMPOUND);
    for (size_t i = 0; i < tree->children.size(); i++)
    {
        compound->children.push_back(visitorVisit(visitor, tree->children[i], list));
    }

    return compound;
}

Tree *visitorVisitFuncDef(Visitor *visitor, Tree *tree, std::vector<Tree *> &list)
{
    Tree *funcDef = initTree(TREE_FUNC_DEC);
    funcDef->name = tree->name;
    funcDef->data = visitorVisit(visitor, tree->data, list);
    for (int i = 0; i < tree->children.size(); i++)
    {
        funcDef->children.push_back(visitorVisit(visitor, tree->children[i], list));
    }
    list.push_back(funcDef);

    return funcDef;
}

Tree *visitorVisitAssign(Visitor *visitor, Tree *tree, std::vector<Tree *> &list)
{
    Tree *var = initTree(TREE_VAR);
    var->name = tree->name;
    var->data = visitorVisit(visitor, tree->data, list);
    list.push_back(var);

    return var;
}

Tree *visitorVisitVariable(Visitor *visitor, Tree *tree, std::vector<Tree *> &list)
{
    Tree *var = varLookUp(visitor->object->children, tree->name);

    if (var != nullptr)
    {
        return var;
    }

    return tree;
}

Tree *visitorVisitInt(Visitor *visitor, Tree *tree, std::vector<Tree *> &list)
{
}

Tree *visitorVisitCall(Visitor *visitor, Tree *tree, std::vector<Tree *> &list)
{
    if (tree->name == "ret")
    {
        return tree;
    }

    Tree *var = varLookUp(visitor->object->children, tree->name);

    if (var != nullptr && var->functionPtr != nullptr)
    {
        return var->functionPtr(visitor, tree, tree->data->children);
    }

    return tree;
}

Tree *visitorVisitAccess(Visitor *visitor, Tree *tree, std::vector<Tree *> &list)
{
}
