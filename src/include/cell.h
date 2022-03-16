#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <string.h>
#include "lexer.h"

#define byte uint8_t

// Operations
enum
{
    OP_PUSH,    // push value to stack
    OP_PLUS,    // add top two values on stack
    OP_MINUS,   // subtract top two values on stack
    OP_EQUAL,   // compare equality of top two values on stack
    OP_PRINT,   // print top value on stack to console
    OP_IF,      // if block
    OP_ELSE,    // else block
    OP_END,     // end block
    OP_DUP,     // duplicate top value on the stack
    OP_DUP2,    // duplicate top 2 values on the stack
    OP_DROP,    // pop top value off the stack
    OP_SWAP,    // swap top two values on the stack
    OP_OVER,    // push second value on stack to top again
    OP_GT,      // compare greater than of top two values on stack
    OP_LT,      // compare less than of top two values on stack
    OP_GTE,     // compare greater than or equal of top two values on stack
    OP_LTE,     // compare less than or equal of top two values on stack
    OP_WHILE,   // while condition
    OP_DO,      // do block
    OP_INC,     // increment top value on stack
    OP_DEC,     // decrement top value on stack
    OP_SHL,     // bitwise hift left
    OP_SHR,     // bitwise shift right
    OP_LOR,     // bitwise or
    OP_XOR,     // bitwise xor
    OP_LAND,    // bitwise and
    OP_MEM,     // memory keyword
    OP_LOAD,    // load memory
    OP_STORE,   // store memory
    OP_WRITE,   // write syscall
    OP_EXIT,    // exit syscall
    LEN_OPS     // number of valid operations
};

// 1MB max for memory purposes
const int MEM_CAPACITY = (1ULL << 20);

struct code
{
    int type = -1;      // type of operation
    int value = -1;     // value (int for push, jump address for conditionals)
    std::string loc;
};

code parse_op(std::string value, std::string msg);
std::vector<code> parse_blocks(std::vector<code> program);
std::vector<code> load_program(std::string input_file);
void simulate_program(std::vector<code> program);
void compile_program(std::vector<code> program, std::string output_file);
void usage();