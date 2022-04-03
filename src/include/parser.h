#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

/*
 * Node
 * contains a token and a list of children to then generate the Abstract Syntax Tree
 */
struct Node
{
    Token token;
    std::list<Node *> children;
};

Node *newNode(Token token);

Node *unary(const Token &op, const Token &token);
Node *binary(Node *left, Node *op, Node *right);

Node *parse_fact(std::list<Token> &tokens);
Node *parse_term(std::list<Token> &tokens);
Node *parse_expr(std::list<Token> &tokens);
Node *parse_statement(std::list<Token> &tokens);
Node *parse_function(std::list<Token> &tokens);
Node *parse_program(std::list<Token> &tokens);

void pretty_print_tabs(int num_tabs, std::ostream &out=std::cout);
void pretty_print_helper(Node *node, int num_tabs, std::ostream &out=std::cout);
void pretty_print(Node *node, std::ostream &out=std::cout);

void print_error(std::string message, int row, int col);

#endif