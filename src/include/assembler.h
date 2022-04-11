#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "parser.h"

std::string assemble_unary(Node *root, const std::unordered_map<std::string, Node *> &scope);
std::string assemble_binary(Node *root, const std::unordered_map<std::string, Node *> &scope);
std::string assemble_expr(Node *root, const std::unordered_map<std::string, Node *> &scope);
std::string assemble_if(Node *root);
std::string assemble_loop(Node *root);
std::string assemble_function(Node *root);
std::string assemble_program(Node *root);
void assemble_nasm(std::string filename, Node *root);

#endif