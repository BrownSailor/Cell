#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

struct Node;

#define Scope std::unordered_map<std::string, Node *>

extern Scope functions;
extern Scope types;

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
        NODE_VAR_DEC,
        NODE_VAR_DEC_ASN,
        NODE_LIST,
        NODE_TYPE,
        NODE_TYPE_DEF,
        NODE_FUNC_CALL,
        NODE_FUNC_DEC
    };

    Type type = NODE_NONE;
    Token token;
    Scope scope;
    size_t arr_dim = 0;
    bool semi = false;
    bool ptr = false;
    std::string datatype = "";
    std::list<Node *> children;
};

int eat_open_parens(std::list<Token> &tokens, char p='(');
void eat_close_parens(std::list<Token> &tokens, int parens, char p=')');

Node *new_node(Token token);

Node *unary(Node *op, Node *node);
Node *binary(Node *left, Node *op, Node *right);

Node *parse_fact(std::list<Token> &tokens, Scope &scope, Node *parent = nullptr);
Node *parse_term(std::list<Token> &tokens, Scope &scope);
Node *parse_add_sub(std::list<Token> &tokens, Scope &scope);
Node *parse_lt_gt(std::list<Token> &tokens, Scope &scope);
Node *parse_eq_neq(std::list<Token> &tokens, Scope &scope);
Node *parse_and(std::list<Token> &tokens, Scope &scope);
Node *parse_or(std::list<Token> &tokens, Scope &scope);
Node *parse_expr(std::list<Token> &tokens, Scope &scope);
Node *parse_statement(std::list<Token> &tokens, Scope &scope);
Node *parse_if(std::list<Token> &tokens, Scope &scope);
Node *parse_loop(std::list<Token> &tokens, Scope &scope);
Node *parse_body(Node *node, std::list<Token> &tokens, Scope &scope);
Node *parse_type_def(std::list<Token> &tokens);
Node *parse_function(std::list<Token> &tokens);
Node *parse_program(std::list<Token> &tokens);

void print_warning(std::string message, const Token &token);
void print_error(std::string message, const Token &token);

std::string node_type_to_str(Node *node);
void pretty_print_tabs(int num_tabs, std::ostream &out=std::cout);
void print_scope(const Scope &scope, std::ostream &out=std::cout);
void pretty_print_helper(Node *node, const Scope &scope, int num_tabs, std::ostream &out=std::cout);
void pretty_print(Node *node, std::ostream &out=std::cout);

#endif
