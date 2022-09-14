#include "include/chunk.h"
#include "include/debug.h"
#include "include/vm.h"

int main(int argc, char **argv)
{
    (void)(argc);
    (void)(argv);

    Chunk *chunk = new Chunk;

    Byte c = add_constant(chunk, 1.2);
    add_chunk(chunk, OP_CONSTANT, 123);
    add_chunk(chunk, c, 123);

    c = add_constant(chunk, 3.4);
    add_chunk(chunk, OP_CONSTANT, 123);
    add_chunk(chunk, c, 123);

    add_chunk(chunk, OP_ADD, 123);

    c = add_constant(chunk, 5.6);
    add_chunk(chunk, OP_CONSTANT, 123);
    add_chunk(chunk, c, 123);

    add_chunk(chunk, OP_DIV, 123);

    add_chunk(chunk, OP_NEG, 123);
    add_chunk(chunk, OP_RETURN, 123);
    disassemble_chunk(chunk, "test chunk");

    interpret(chunk);

    return 0;
}