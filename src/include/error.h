#ifndef ERROR_H
#define ERROR_H

#include "parser.h"

// unbound checking
void check_bound(Node *root, const Scope &scope);

// type checking
std::string get_type(Node *node, const Scope &scope);
void print_type_error(Token::Type lhs, Token::Type rhs, const Token &loc, int left_arr_size = 0, int right_arr_size = 0);
void check_types(Node *root, const Scope &scope);

// check all errors
bool check_errors(Node *root, const Scope &scope);

#endif
