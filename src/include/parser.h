#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <unordered_map>

/*
 * Node
 * contains a token, list of children, and variable scope and offset to generate the Abstract Syntax Tree
 */
struct Node
{
    Token token;
    std::list<Node *> children;
    std::unordered_map<std::string, Node *> scope;
    int offset = 0;
};

Node *newNode(Token token);

Node *unary(const Token &op, const Token &token);
Node *binary(Node *left, Node *op, Node *right);

Node *parse_fact(std::list<Token> &tokens);
Node *parse_term(std::list<Token> &tokens);
Node *parse_add_sub(std::list<Token> &tokens);
Node *parse_lt_gt(std::list<Token> &tokens);
Node *parse_eq_neq(std::list<Token> &tokens);
Node *parse_and(std::list<Token> &tokens);
Node *parse_or(std::list<Token> &tokens);
Node *parse_expr(std::list<Token> &tokens, std::unordered_map<std::string, Node *> &scope);
Node *parse_statement(std::list<Token> &tokens, std::unordered_map<std::string, Node *> &scope);
Node *parse_function(std::list<Token> &tokens);
Node *parse_program(std::list<Token> &tokens);

void pretty_print_tabs(int num_tabs, std::ostream &out=std::cout);
void pretty_print_helper(Node *node, int num_tabs, std::ostream &out=std::cout);
void pretty_print(Node *node, std::ostream &out=std::cout);

void print_warning(std::string message, const Token &token);
void print_error(std::string message, const Token &token);

#endif