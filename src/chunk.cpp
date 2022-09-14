#include "include/chunk.h"

void add_chunk(Chunk *chunk, Byte code, int line)
{
    chunk->code.push_back(code);
    chunk->lines.push_back(line);
}

size_t add_constant(Chunk *chunk, Value val)
{
    chunk->constants.push_back(val);
    return chunk->constants.size() - 1;
}