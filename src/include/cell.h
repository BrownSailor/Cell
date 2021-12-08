#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include "lexer.h"

// Operations
enum
{
    OP_PUSH,  // push value to stack
    OP_PLUS,  // add top two values on stack
    OP_MINUS, // subtract top two values on stack
    OP_MUL,   // subtract top two values on stack
    OP_DIV,   // subtract top two values on stack
    OP_MOD,   // subtract top two values on stack
    OP_DUMP,  // print top value on stack to console
    LEN_OPS   // number of valid operations
};

struct code
{
    int op_type;
    int value;
};

code push(int x);
code plus();
code minus();
code dump();
bool string_is_int(std::string s);
code parse_op(std::string value, std::string msg);
std::vector<code> load_program(std::string input_file);
void simulate_program(std::vector<code> program);
void compile_program(std::vector<code> program, std::string output_file);
void usage();