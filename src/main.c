/* Cell:    a programming language written in C
 * Author:  Shreyas Ravi
 * Created: September 24 2021
 */

#include "include/chunk.h"
#include "include/debug.h"
#include "include/vm.h"

int main(int argc, char *argv[])
{
    Chunk chunk;

    initVM();
    initChunk(&chunk);

    // -((2 * (7 + 12)) / 19)
    int constant = addConstant(&chunk, 7);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 1);

    constant = addConstant(&chunk, 12);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 1);

    writeChunk(&chunk, OP_ADD, 1);

    constant = addConstant(&chunk, 2);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 1);

    writeChunk(&chunk, OP_MUL, 1);

    constant = addConstant(&chunk, 19);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 1);
    
    writeChunk(&chunk, OP_DIV, 1);

    writeChunk(&chunk, OP_NEG, 1);
    writeChunk(&chunk, OP_RETURN, 1);

    interpret(&chunk);

    freeVM();
    freeChunk(&chunk);

    return 0;
}