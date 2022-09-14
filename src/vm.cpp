#include "include/vm.h"

VM vm;

InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONST() (vm.chunk->constants[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
        double b = vm.pop(); \
        double a = vm.pop(); \
        vm.push(a op b); \
    } while (false)

    while (true)
    {
        Byte instr;
        switch (instr = READ_BYTE())
        {
            case OP_CONSTANT:
                vm.push(READ_CONST());
                break;

            case OP_ADD:
                BINARY_OP(+);
                break;
            
            case OP_SUB:
                BINARY_OP(-);
                break;

            case OP_MUL:
                BINARY_OP(*);
                break;

            case OP_DIV:
                BINARY_OP(/);
                break;

            case OP_NEG: 
                vm.push(-vm.pop());
                break;

            case OP_RETURN:
                std::cout << vm.pop() << "\n";
                return INTERPRET_OK;
        }
    }

#undef READ_BYTE
#undef READ_CONST
#undef BINARY_OP
}

InterpretResult interpret(Chunk *chunk)
{
    vm.chunk = chunk;
    vm.ip = vm.chunk->code.data();

    return run();
}