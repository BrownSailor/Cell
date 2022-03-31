#include "include/token.h"

std::unordered_map<std::string, int> BUILT_IN_WORDS =
{
    { "dump", OP_DUMP },
    { "dup", OP_DUP },
    { "drop", OP_DROP },
    { "swap", OP_SWAP },
    { "over", OP_OVER },

    { "+", OP_PLUS },
    { "-", OP_MINUS },
    { "*", OP_MUL },
    { "divmod", OP_DIVMOD },
    { "shl", OP_SHL },
    { "shr", OP_SHR },
    { "not", OP_NOT },
    { "or", OP_OR },
    { "xor", OP_XOR },
    { "and", OP_AND },

    { "=", OP_EQ },
    { "!=", OP_NEQ },
    { ">", OP_GT },
    { "<", OP_LT },
    { ">=", OP_GTE },
    { "<=", OP_LTE },

    { "if", OP_IF },
    { "elif", OP_ELIF },
    { "else", OP_ELSE },
    { "end", OP_END },
    { "while", OP_WHILE },
    { "do", OP_DO },
    { "def", OP_DEF },
    { "fn", OP_FN },

    { "mem", OP_MEM },
    { "@8", OP_LOAD_8 },
    { "!8", OP_STORE_8 },
    { "@16", OP_LOAD_16 },
    { "!16", OP_STORE_16 },
    { "@32", OP_LOAD_32 },
    { "!32", OP_STORE_32 },
    { "@64", OP_LOAD_64 },
    { "!64", OP_STORE_64 },

    { "sys0", OP_SYS_0 },
    { "sys1", OP_SYS_1 },
    { "sys2", OP_SYS_2 },
    { "sys3", OP_SYS_3 },
    { "sys4", OP_SYS_4 },
    { "sys5", OP_SYS_5 },
    { "sys6", OP_SYS_6 },

    { "use", OP_USE }
};