#ifndef TYPES_H
#define TYPES_H

#include "parser.h"

// type checking
Node *set_types(Node *node, const Scope &scope);
void print_type_error(Token::Type lhs, Token::Type rhs, const Token &loc, int left_arr_size = 0, int right_arr_size = 0);
bool check_types(Node *root, const Scope &scope);

#endif