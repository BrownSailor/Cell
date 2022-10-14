#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "parser.h"
#include "builtin.h"

std::string assemble_unary(Node *root, const Scope &scope);
std::string assemble_post_unary(Node *root, const Scope &scope);
std::string assemble_binary(Node *root, const Scope &scope);
std::string assemble_type(Node *root, size_t size = 0);
std::string assemble_expr(Node *root, const Scope &scope, Node *parent = nullptr, size_t arr_size = 0);
std::string assemble_if(Node *root);
std::string assemble_loop(Node *root);
std::string assemble_body(std::list<Node *>::iterator &it, const std::list<Node *>::iterator &end, const Scope &scope);
std::string assemble_function(Node *root);
std::string assemble_program(Node *root);
void assemble(std::string filename, Node *root);
void print_assembled(Node *root);

#endif
