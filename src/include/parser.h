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
Node *parse_expr(std::list<Token> &tokens);
Node *parse_statement(std::list<Token> &tokens);
Node *parse_function(std::list<Token> &tokens);
Node *parse_program(std::list<Token> &tokens);

void pretty_print(Node *node, int depth);
void print_error(std::string message, int row, int col);

#endif