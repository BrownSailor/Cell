#ifndef ERROR_H
#define ERROR_H

#include "parser.h"
#include "types.h"

// unbound checking
bool check_bound(Node *root, const Scope &scope);

// check all errors
bool check_errors(Node *root, const Scope &scope);

#endif
