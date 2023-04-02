#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <string>
#include <list>
#include <unordered_map>

struct Node;

typedef std::unordered_map<std::string, Node *> Scope;

struct Node
{
    enum Type
    {
        NODE_NONE,
        NODE_LIT,
        NODE_OP,
        NODE_KEY,
        NODE_VAR,
        NODE_VAR_ASN,
        NODE_FUN_CALL,
        NODE_FUN_IN,
        NODE_FUN_OUT,
        NODE_FUN_BODY,
        NODE_FUN_DEC,
        NODE_PROG
    };

    Type type = NODE_NONE;
    Token token;
    Scope scope;

    std::list<Node *> children;
};

Node *parse_program(std::list<Token> &tokens);
void pretty_print(Node *node);
void free_tree(Node *node);

#endif
