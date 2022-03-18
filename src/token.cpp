#include "include/token.h"

std::unordered_map<std::string, int> BUILT_IN_WORDS =
{
    { "+", OP_PLUS },
    { "-", OP_MINUS },
    { "=", OP_EQ },
    { "!=", OP_NEQ },
    { "print", OP_PRINT },
    { "if", OP_IF },
    { "else", OP_ELSE },
    { "end", OP_END },
    { "dup", OP_DUP },
    { "dup2", OP_DUP2 },
    { "drop", OP_DROP },
    { "swap", OP_SWAP },
    { "over", OP_OVER },
    { ">", OP_GT },
    { "<", OP_LT },
    { ">=", OP_GTE },
    { "<=", OP_LTE },
    { "inc", OP_INC },
    { "dec", OP_DEC },
    { "<<", OP_SHL },
    { ">>", OP_SHR },
    { "|", OP_LOR },
    { "^", OP_XOR },
    { "&", OP_LAND },
    { "while", OP_WHILE },
    { "do", OP_DO },
    { "mem", OP_MEM },
    { ",", OP_LOAD },
    { ".", OP_STORE },
    { "write", OP_WRITE },
    { "exit", OP_EXIT }
};

void preprocess_lookup()
{
    
}