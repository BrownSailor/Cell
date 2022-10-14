#ifndef BUILTIN_H
#define BUILTIN_H

#include "parser.h"

#define arr_size(arr, type) (sizeof(arr) / sizeof(type))

std::string builtin_print(Node *root, const Scope &scope, std::string expr);
std::string builtin_println(Node *root, const Scope &scope, std::string expr);
std::string builtin_size(Node *root, std::string expr);
std::string builtin_pushb(Node *root, std::string expr1, std::string expr2);
std::string builtin_popf(Node *root, std::string expr);

#endif
