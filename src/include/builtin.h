#ifndef BUILTIN_H
#define BUILTIN_H

#include "parser.h"

std::string builtin_print(Node *root, const Scope &scope, std::string expr);
std::string builtin_println(Node *root, const Scope &scope, std::string expr);

#endif
