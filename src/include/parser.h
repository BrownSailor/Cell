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

    /*
     * 64 bits for an identifier for use with LLVM generator
     * 2 most signicant bits:
     * 00 -> uninitialized ID
     * 01 -> temporary ID
     * 10 -> variable ID
     * 11 -> label ID
     *
     * [2 bit key] [26 bit label ID] [16 bit var ID] [20 bit temp ID]
     */
    uint64_t id = 0;
    std::vector<Node *> children;
};

template<> struct std::hash<Node *>
{
    size_t operator()(const Node *node) const
    {
        return reinterpret_cast<uintptr_t>(node);
    }
};

extern std::unordered_map<std::string, Node *> functions;

Node *parse_program(std::list<Token> &tokens);
void pretty_print(Node *node);
void free_tree(Node *node);

#endif
