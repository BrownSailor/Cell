#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <unordered_map>

// Tokens
enum
{
    TOKEN_INT,  // integer token
    TOKEN_WORD, // word token
    LEN_TOKENS  // number of tokens
};

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

struct Token
{
    int type = -1;
    int int_val = 0;
    std::string str_val = "";
    std::string loc;
};

extern std::unordered_map<std::string, int> BUILT_IN_WORDS;

#endif