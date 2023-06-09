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
    union
    {
        /* use first 4 bits for arrays (i.e. max dimension is 15) */
        uint32_t alpha;

        /* [ input types, output type ] */
        std::pair<std::vector<uint32_t>, uint32_t> fun_type;
    };

    TypeScheme()
    {
        type = NONE;
    }

    TypeScheme(Type t)
    {
        if (t == FUN_TYPE)
        {
            new (&fun_type) std::pair<std::vector<uint32_t>, uint32_t>();
        }

        type = t;
    }

    TypeScheme(const TypeScheme &other)
    {
        type = other.type;
        if (type == ALPHA)
        {
            alpha = other.alpha;
        }
        else if (type == FUN_TYPE)
        {
            new (&fun_type) std::pair<std::vector<uint32_t>, uint32_t>(other.fun_type);
        }
    }

    TypeScheme operator=(const TypeScheme &other)
    {
        type = other.type;
        if (type == ALPHA)
        {
            alpha = other.alpha;
        }
        else if (type == FUN_TYPE)
        {
            new (&fun_type) std::pair<std::vector<uint32_t>, uint32_t>(other.fun_type);
        }

        return *this;
    }

    ~TypeScheme()
    {
        if (type == FUN_TYPE)
        {
            fun_type.~pair();
        }
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
