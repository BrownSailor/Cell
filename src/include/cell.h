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
    OP_EQUAL, // compare equality of top two values on stack
    OP_DUMP,  // print top value on stack to console
    OP_IF,    // if block
    OP_ELSE,  // else block
    OP_END,   // end block
    OP_DUP,   // duplicate top value on the stack
    OP_GT,    // compare greater than of top two values on stack
    OP_LT,    // compare less than of top two values on stack
    OP_GTE,   // compare greater than or equal of top two values on stack
    OP_LTE,   // compare less than or equal of top two values on stack
    OP_WHILE, // while condition
    OP_DO,    // do block
    OP_INC,   // increment top value on stack
    OP_DEC,   // decrement top value on stack
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
code equal();
code dump();
code iff();
code elze();
code end();
code dup();
code gt();
code lt();
code gte();
code lte();
code inc();
code dec();
code wile();
code doo();
bool string_is_int(std::string s);
code parse_op(std::string value, std::string msg);
std::vector<code> parse_blocks(std::vector<code> program);
std::vector<code> load_program(std::string input_file);
void simulate_program(std::vector<code> program);
void compile_program(std::vector<code> program, std::string output_file);
void usage();