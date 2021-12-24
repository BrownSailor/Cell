#ifndef CELL_CHUNK_H
#define CELL_CHUNK_H

#include "common.h"
#include "value.h"

typedef enum OpCode
{
    OP_CONSTANT,
    OP_RETURN,
} OpCode;

typedef struct Chunk
{
    int size;
    int capacity;
    uint8_t *code;
    int *lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);
void freeChunk(Chunk *chunk);
int addConstant(Chunk *chunk, Value value);

#endif