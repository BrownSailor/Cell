#ifndef PARSER_H
#define PARSER_H

#include <cstdint>
#include "lexer.hpp"
#include <string>
#include <vector>
#include <stack>
#include <unordered_set>
#include <memory>

struct Node
{
    enum Type
    {
        NODE_NONE,
        NODE_LIT,
        NODE_OP,
        NODE_KEY,
        NODE_VAR,
        NODE_VAR_ASN,   // SET
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

    std::vector<std::unique_ptr<Node>> children;
};

extern std::unordered_set<std::string> functions;
std::unique_ptr<Node> parse_program(std::list<Token> &tokens);

#endif
