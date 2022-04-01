#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include <iostream>
#include "assembler.h"

void compile_and_link(std::string filename, Node *root, std::ostream &out=std::cout);

#endif