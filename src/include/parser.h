#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <string>
#include <vector>
#include <stack>
#include <unordered_set>

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
        NODE_IF,
        NODE_ELSE,
        NODE_LOOP,
        NODE_BODY,
        NODE_FUN_CALL,
        NODE_FUN_IN,
        NODE_FUN_OUT,
        NODE_FUN_DEC,
        NODE_PROG
    };

    Type type = NODE_NONE;
    Token token;

    /* 32 bits for an identifier for use with LLVM generator */
    uint32_t id = 0;
    uint64_t datatype = 0;
    std::vector<Node *> children;
};

template<> struct std::hash<Node *>
{
    size_t operator()(const Node *node) const
    {
        return reinterpret_cast<uintptr_t>(node);
    }
};

extern std::unordered_map<std::string, std::unordered_set<Node *>> functions;

Node *parse_program(std::list<Token> &tokens);
void free_tree(Node *node);

#endif
