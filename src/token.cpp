#include "include/token.h"

std::unordered_map<std::string, int> BUILT_IN_WORDS =
{
    { "dump", OP_DUMP },
    { "dup", OP_DUP },
    { "dup2", OP_DUP2 },
    { "drop", OP_DROP },
    { "swap", OP_SWAP },
    { "over", OP_OVER },

    { "+", OP_PLUS },
    { "-", OP_MINUS },
    { "*", OP_MUL },
    { "++", OP_INC },
    { "--", OP_DEC },
    { "<<", OP_SHL },
    { ">>", OP_SHR },
    { "|", OP_LOR },
    { "^", OP_XOR },
    { "&", OP_LAND },

    { "=", OP_EQ },
    { "!=", OP_NEQ },
    { ">", OP_GT },
    { "<", OP_LT },
    { ">=", OP_GTE },
    { "<=", OP_LTE },

    { "if", OP_IF },
    { "else", OP_ELSE },
    { "end", OP_END },
    { "while", OP_WHILE },
    { "do", OP_DO },
    { "def", OP_DEF },

    { "mem", OP_MEM },
    { "load", OP_LOAD },
    { "store", OP_STORE },

    { "sys_write", OP_SYS_WRITE },
    { "sys_read", OP_SYS_READ },
    { "sys_exit", OP_SYS_EXIT },

    { "use", OP_USE }
};