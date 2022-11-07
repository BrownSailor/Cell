#ifndef TYPE_H
#define TYPE_H

#include "parser.h"

void print_type_error(Token::Type lhs, Token::Type rhs, const Token &loc, size_t arr_size = 0);
void type_checker(Node *root, const Scope &scope);
bool type_check(Node *root, const Scope &scope);

#endif
