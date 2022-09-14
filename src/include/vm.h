#ifndef VM_H
#define VM_H

#include "chunk.h"

const int STACK_MAX = 256;

struct VM
{
    Chunk *chunk;
    Byte *ip;
    Value stack[STACK_MAX];
    Value *stack_top;

    VM()
    {
        stack_top = stack;
    }

    void push(Value val)
    {
        *stack_top = val;
        stack_top++;
    }

    Value pop()
    {
        stack_top--;
        return *stack_top;
    }
};

enum InterpretResult
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

InterpretResult run();
InterpretResult interpret(Chunk *chunk);

#endif