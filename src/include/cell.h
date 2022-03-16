#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <string.h>
#include "lexer.h"

#define byte uint8_t

// 1MB max for memory purposes
const int MEM_CAPACITY = (1ULL << 20);

struct Code
{
    int type = -1;      // type of operation
    int value = 0;     // value (int for push, jump address for conditionals)
    std::string loc;
};

Code parse_op(std::string value, std::string msg);
std::vector<Code> parse_blocks(std::vector<Code> program);
std::vector<Code> load_program(std::string input_file);
void simulate_program(std::vector<Code> program);
void compile_program(std::vector<Code> program, std::string output_file);
void usage();

#endif