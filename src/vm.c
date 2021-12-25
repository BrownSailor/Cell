#include "include/vm.h"

VM vm;

static void resetStack()
{
    vm.stackTop = vm.stack;
}

void initVM()
{
    resetStack();
}

void freeVM()
{
}

static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)  \
    do                 \
    {                  \
        int b = pop(); \
        int a = pop(); \
        push(a op b);  \
    } while (false)

    while (true)
    {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (Value *item = vm.stackTop - 1; item >= vm.stack; item--)
        {
            printf("[ ");
            printValue(*item);
            printf(" ]");
        }
        printf("\n");

        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif

        uint8_t instruction;
        switch (instruction = READ_BYTE())
        {

        case OP_CONSTANT:
        {
            Value constant = READ_CONSTANT();
            push(constant);
            break;
        }

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

        case OP_MOD:
            BINARY_OP(%);
            break;

        case OP_LT:
            BINARY_OP(<);
            break;

        case OP_LTE:
            BINARY_OP(<=);
            break;

        case OP_GT:
            BINARY_OP(>);
            break;

        case OP_GTE:
            BINARY_OP(>=);
            break;

        case OP_EQEQ:
            BINARY_OP(==);
            break;

        case OP_NEQ:
            BINARY_OP(!=);
            break;

        case OP_NEG:
            push(-pop());
            break;

        case OP_RETURN:
        {
            printValue(pop());
            printf("\n");
            return INTERPRET_OK;
        }

        default:
            break;
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretResult interpret(const char *source)
{
    compile(source);
    return INTERPRET_OK;
}

void push(Value value)
{
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop()
{
    vm.stackTop--;
    return *vm.stackTop;
}