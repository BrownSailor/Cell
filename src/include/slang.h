#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <string.h>
#include <algorithm>
#include "lexer.h"

#define byte uint8_t

// Space allocated for memory purposes, split into halves for string and data
const int STR_CAPACITY = 640000;
const int MEM_CAPACITY = 640000;

struct Op
{
    int type = -1;                  // type of operation
    unsigned long long value = 0;   // value (int for push, jump address for conditionals)
    std::string data = "";          // data (string for push)
    int addr = -1;                  // addr (string data)
    std::string loc;                // location in file
};

struct Macro
{
    std::string loc;          // location of macro definition
    std::vector<Token> body;  // body of macro
};

std::string unescape(const std::string& s);
std::vector<Op> parse_blocks(std::vector<Op> program);
std::vector<Op> load_program(std::string input_file);
void compile_program(std::vector<Op> program, std::string output_file);
void usage();

#endif