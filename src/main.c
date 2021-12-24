/* Cell:    a programming language written in C
 * Author:  Shreyas Ravi
 * Created: September 24 2021
 */

#include "include/chunk.h"
#include "include/debug.h"

int main(int argc, char *argv[])
{
    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 7);
    writeChunk(&chunk, OP_CONSTANT, 1);
    writeChunk(&chunk, constant, 1);

    writeChunk(&chunk, OP_RETURN, 1);

    disassembleChunk(&chunk, "test");
    freeChunk(&chunk);

    return 0;
}