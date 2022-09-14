#include "include/debug.h"

size_t dissassemble_instruction(Chunk *chunk, size_t idx)
{
    std::cout << idx << "\t";
    std::cout << chunk->lines[idx] << "\t";

    Byte instr = chunk->code[idx];
    switch (instr)
    {
        case OP_CONSTANT:
            std::cout << "OP_CONSTANT\t";
            std::cout << chunk->constants[chunk->code[idx + 1]] << "\n";
            return idx + 2;

        case OP_ADD:
            std::cout << "OP_ADD\n";
            return idx + 1;
        
        case OP_SUB:
            std::cout << "OP_SUB\n";
            return idx + 1;
        
        case OP_MUL:
            std::cout << "OP_MULTIPLY\n";
            return idx + 1;

        case OP_DIV:
            std::cout << "OP_DIVIDE\n";
            return idx + 1;

        case OP_NEG:
            std::cout << "OP_NEGATE\n";
            return idx + 1;

        case OP_RETURN:
            std::cout << "OP_RETURN\n";
            return idx + 1;

        default:
            std::cerr << "Unknown OpCode " << instr << "\n";
            return idx + 1;
    }
}

void disassemble_chunk(Chunk *chunk, std::string name)
{
    std::cout << "== " << name << " ==\n";

    for (size_t i = 0; i < chunk->code.size();)
    {
        i = dissassemble_instruction(chunk, i);
    }
}