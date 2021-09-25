#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include "lexer.h"
#include "io.h"

void compile(std::string src);

void compile_file(std::string filename);

#endif