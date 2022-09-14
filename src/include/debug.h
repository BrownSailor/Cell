#ifndef DEBUG_H
#define DEBUG_H

#include "chunk.h"

size_t disassemble_instruction(Chunk *chunk, size_t idx);
void disassemble_chunk(Chunk *chunk, std::string name);

#endif