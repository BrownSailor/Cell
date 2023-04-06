#ifndef COMPILER_UTILS
#define COMPILER_UTILS

typedef unsigned long uint64_t;

enum Type
{
    NONE,
    TMP,
    VAR,
    LAB
};

uint64_t store_id(uint64_t word, Type type, uint64_t id);
uint64_t get_id(uint64_t word);

#endif
