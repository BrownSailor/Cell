#ifndef PARSER_H
#define PARSER_H

#include <cstdint>
#include "lexer.hpp"
#include <string>
#include <vector>
#include <stack>
#include <unordered_set>
#include <memory>

#include "error.hpp"

struct TypeScheme
{
    enum Type
    {
        NONE,
        ALPHA,
        FUN_TYPE
    };
    
    Type type;
    uint32_t alpha;
    std::vector<uint32_t> params;

    TypeScheme()
    {
        type = NONE;
    }

    TypeScheme(Type t)
    {
        if (t == FUN_TYPE)
        {
            new (&params) std::vector<uint32_t>();
        }

        type = t;
    }

    TypeScheme(const TypeScheme &other)
    {
        type = other.type;
        alpha = other.alpha;
        if (type == FUN_TYPE)
        {
            new (&params) std::vector<uint32_t>(other.params);
        }
    }

    TypeScheme operator=(const TypeScheme &other)
    {
        type = other.type;
        alpha = other.alpha;
        if (type == FUN_TYPE)
        {
            new (&params) std::vector<uint32_t>(other.params);
        }

        return *this;
    }
};

struct Node
{
    enum Type
    {
        NODE_NONE,
        NODE_LIT,
        NODE_OP,
        NODE_KEY,
        NODE_BUILT_IN,
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
    TypeScheme type_scheme;
    Token token;

    std::vector<std::unique_ptr<Node>> children;
};

std::unique_ptr<Node> parse_program(std::list<Token> &tokens);

#endif
