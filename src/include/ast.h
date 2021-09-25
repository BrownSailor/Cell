#ifndef AST_H
#define AST_H

enum TreeType
{
    AST_COMPOUND,
    AST_FUNC_DEF,
    AST_TYPE_DEF,
    AST_VARIABLE,
    AST_STATEMENT,
    AST_NO_OP
};

struct Tree
{
    unsigned int type;
};

#endif