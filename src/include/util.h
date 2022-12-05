#ifndef UTIL_H
#define UTIL_H

#include "error.h"

// printing functions
std::string node_type_to_str(Node *node);
void pretty_print_tabs(int num_tabs, std::ostream &out=std::cout);
void print_scope(const Scope &scope, std::ostream &out=std::cout);
void pretty_print_helper(Node *node, const Scope &scope, int num_tabs, std::ostream &out=std::cout);
void pretty_print(Node *node, std::ostream &out=std::cout);

#endif