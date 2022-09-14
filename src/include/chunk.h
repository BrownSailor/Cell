#ifndef CHUNK_H
#define CHUNK_H

#include "include.h"
#include "value.h"

enum OpCode
{
    OP_CONSTANT,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_NEG,
    OP_RETURN,
};

typedef uint8_t Byte;

struct Chunk
{
    std::vector<Byte> code;
    std::vector<Value> constants;
    std::vector<int> lines;
};

void add_chunk(Chunk *chunk, Byte code, int line);
size_t add_constant(Chunk *chunk, Value val);

#endif