#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

struct Node;

typedef std::unordered_map<std::string, Node *> Scope;

struct Node
{
    enum Type
    {
        NODE_NONE,
    };

    Type type = NODE_NONE;
    Token token;
    Scope scope;
    std::list<Node *> children;
};

int eat_open_parens(std::list<Token> &tokens);
void eat_close_parens(std::list<Token> &tokens, int parens);

Node *new_node(Token token);

Node *unary(Node *op, Node *node);
Node *binary(Node *left, Node *op, Node *right);

Node *parse_fact(std::list<Token> &tokens, Scope &scope);
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
Node *parse_function(std::list<Token> &tokens);
Node *parse_program(std::list<Token> &tokens);

void pretty_print_tabs(int num_tabs, std::ostream &out=std::cout);
void pretty_print_helper(Node *node, int num_tabs, std::ostream &out=std::cout);
void pretty_print(Node *node, std::ostream &out=std::cout);

void print_warning(std::string message, const Token &token);
void print_error(std::string message, const Token &token);

#endif