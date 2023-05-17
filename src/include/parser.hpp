#ifndef PARSER_H
#define PARSER_H

#include <cstdint>
#include "lexer.hpp"
#include "error.hpp"
#include <string>
#include <vector>
#include <stack>
#include <unordered_set>

struct TypeScheme
{
    enum Type
    {
        ALPHA,
        FUN_TYPE,
        NONE
    };
    
    Type type;
    union
    {
        /* use first 4 bits for arrays (i.e. max dimension is 15) */
        uint32_t alpha;

        /* [ input types, output types ] */
        std::pair<std::vector<uint32_t>, std::vector<uint32_t>> fun_type;
    };

    TypeScheme()
    {
        type = NONE;
    }

    TypeScheme(Type t)
    {
        if (t == FUN_TYPE)
        {
            new (&fun_type) std::pair<std::vector<uint32_t>, std::vector<uint32_t>>();
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
            new (&fun_type) std::pair<std::vector<uint32_t>, std::vector<uint32_t>>(other.fun_type);
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
            new (&fun_type) std::pair<std::vector<uint32_t>, std::vector<uint32_t>>(other.fun_type);
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

    /* 32 bits for an identifier for use with LLVM generator */
    uint32_t id = 0;
    TypeScheme type_scheme;
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
