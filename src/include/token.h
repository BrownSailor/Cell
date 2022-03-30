#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <unordered_map>
#include <vector>

// Tokens
enum
{
    TOKEN_INT,  // integer token
    TOKEN_WORD, // word token
    TOKEN_STR,  // string token
    TOKEN_CHAR, // char token
};

// Operations
enum
{
    OP_PUSH_INT,    // push int value to stack
    OP_PUSH_STR,    // push string value to stack
    OP_PUSH_MEM,    // push relative memory address to stack
    OP_DUMP,        // print top value on stack to console
    OP_DUP,         // duplicate top value on the stack
    OP_DROP,        // pop top value off the stack
    OP_SWAP,        // swap top two values on the stack
    OP_OVER,        // push second value on stack to top again

    OP_PLUS,        // add top two values on stack
    OP_MINUS,       // subtract top two values on stack
    OP_MUL,         // multiply top two values on stack
    OP_DIVMOD,      // divmod operation on top two values on stack
    OP_SHL,         // bitwise shift left
    OP_SHR,         // bitwise shift right
    OP_NOT,         // bitwise not
    OP_OR,          // bitwise or
    OP_XOR,         // bitwise xor
    OP_AND,         // bitwise and

    OP_EQ,          // compare equality of top two values on stack
    OP_NEQ,         // compare non-equality of top two values on stack
    OP_GT,          // compare greater than of top two values on stack
    OP_LT,          // compare less than of top two values on stack
    OP_GTE,         // compare greater than or equal of top two values on stack
    OP_LTE,         // compare less than or equal of top two values on stack
    
    OP_IF,          // if block
    OP_ELIF,        // elif block
    OP_ELSE,        // else block
    OP_END,         // end block
    OP_WHILE,       // while condition
    OP_DO,          // do block
    OP_DEF,         // macro block
    OP_FN,          // function block

    OP_MEM,         // memory keyword
    OP_LOAD_8,      // load 8 bits from memory
    OP_STORE_8,     // store 8 bits in memory
    OP_LOAD_16,     // load 16 bits from memory
    OP_STORE_16,    // store 16 bits in memory
    OP_LOAD_32,     // load 32 bits from memory
    OP_STORE_32,    // store 32 bits in memory
    OP_LOAD_64,     // load 64 bits from memory
    OP_STORE_64,    // store 64 bits in memory

    OP_SYS_0,       // syscall with 0 arguments
    OP_SYS_1,       // syscall with 1 argument
    OP_SYS_2,       // syscall with 2 arguments
    OP_SYS_3,       // syscall with 3 arguments
    OP_SYS_4,       // syscall with 4 arguments
    OP_SYS_5,       // syscall with 5 arguments
    OP_SYS_6,       // syscall with 6 arguments

    OP_USE,         // use mechanism for including external files
};

struct Token
{
    int type = -1;
    uint64_t int_val = 0;
    std::string str_val = "";
    std::string loc;
};

extern std::unordered_map<std::string, int> BUILT_IN_WORDS;

#endif