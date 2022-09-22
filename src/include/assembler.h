#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "parser.h"
#include "builtin.h"

std::string assemble_unary(Node *root, const Scope &scope);
std::string assemble_binary(Node *root, const Scope &scope);
std::string assemble_type(Node *root);
std::string assemble_expr(Node *root, const Scope &scope);
std::string assemble_function(Node *root);
std::string assemble_program(Node *root);
void assemble(std::string filename, Node *root);
void print_assembled(Node *root);

#endif