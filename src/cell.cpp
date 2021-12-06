/* Cell:    a stack-based programming language written in C++
 * Author:  Shreyas Ravi
 * Created: September 24 2021
 */

#include <iostream>
#include <fstream>
#include <stack>
#include <vector>

// Operations
enum
{
    OP_PUSH,  // push value to stack
    OP_PLUS,  // add top two values on stack
    OP_MINUS, // subtract top two values on stack
    OP_DUMP,  // print top value on stack to console
    LEN_OPS   // number of valid operations
};

struct Code
{
    int op_type;
    int value;
};

Code push(int x)
{
    return { OP_PUSH, x };
}

Code plus()
{
    return { OP_PLUS, };
}

Code minus()
{
    return { OP_MINUS, };
}

Code dump()
{
    return { OP_DUMP, };
}

void simulate_program(std::vector<Code> program)
{
    std::stack<int> stack;

    for (auto code : program)
    {
        if (code.op_type == OP_PUSH)
        {
            stack.push(code.value);
        }

        else if (code.op_type == OP_PLUS)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            stack.push(a + b);
        }

        else if (code.op_type == OP_MINUS)
        {
            int a = stack.top();
            stack.pop();

            int b = stack.top();
            stack.pop();

            stack.push(-a + b);
        }

        else if (code.op_type == OP_DUMP)
        {
            std::cout << stack.top() << "\n";
            stack.pop();
        }

        else
        {
            std::cerr << "Unreachable\n";
        }
    }
}

void compile_program(std::vector<Code> program, std::string out_file_path)
{
    std::ofstream out;
    out.open(out_file_path);

    out << "dump:\n";
    out << "    sub     rsp, 40\n";
    out << "    mov     BYTE [rsp+31], 10\n";
    out << "    lea     rdx, [rsp+30]\n\n";

    out << ".L2:\n";
    out << "    movsx   rax, edi\n";
    out << "    mov     ecx, edi\n";
    out << "    lea     r8, [rsp+32]\n";
    out << "    imul    rax, rax, 1717986919\n";
    out << "    sar     ecx, 31\n";
    out << "    sub     r8, rdx\n";
    out << "    sar     rax, 34\n";
    out << "    sub     eax, ecx\n";
    out << "    lea     ecx, [rax+rax*4]\n";
    out << "    add     ecx, ecx\n";
    out << "    sub     edi, ecx\n";
    out << "    add     edi, 48\n";
    out << "    mov     BYTE [rdx], dil\n";
    out << "    mov     edi, eax\n";
    out << "    mov     rax, rdx\n";
    out << "    sub     rdx, 1\n";
    out << "    test    edi, edi\n";
    out << "    jne     .L2\n";
    out << "    lea     rsi, [rsp+32]\n";
    out << "    mov     rdx, r8\n";
    out << "    mov     edi, 1\n";
    out << "    sub     rax, rsi\n";
    out << "    lea     rsi, [rsp+32+rax]\n";
    out << "    mov     rax, 0x2000004\n";
    out << "    syscall\n";
    out << "    add     rsp, 40\n";
    out << "    ret\n\n";

    out << "global _main\n";
    out << "section .text\n";
    out << "_main:\n";

    for (auto code : program)
    {
        if (code.op_type == OP_PUSH)
        {
            out << "\n    ;; push " << code.value << "\n";
            out << "    push " << code.value << "\n";
        }

        else if (code.op_type == OP_PLUS)
        {
            out << "\n    ;; plus\n";
            out << "    pop rax\n";
            out << "    pop rbx\n";
            out << "    add rax, rbx\n";
            out << "    push rax\n";
        }

        else if (code.op_type == OP_MINUS)
        {
            out << "\n    ;; minus\n";
            out << "    pop rbx\n";
            out << "    pop rax\n";
            out << "    sub rax, rbx\n";
            out << "    push rax\n";
        }

        else if (code.op_type == OP_DUMP)
        {
            out << "\n    ;; dump\n";
            out << "    pop rdi\n";
            out << "    call dump\n";
        }
    }
    out << "    mov rax, 0x2000001\n";
    out << "    mov rdi, 0\n";
    out << "    syscall\n";
}

void usage()
{
    std::cout << "Usage: cell <SUBCOMMAND> [FILENAME]\n";
    std::cout << "SUBCOMMANDS:\n";
    std::cout << "    sim <file>       Simulate the program\n";
    std::cout << "    com <file>       Compile the program\n";
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        usage();
        std::cerr << "ERROR: no subcommand provided\n";
        exit(EXIT_FAILURE);
    }

    std::vector<Code> program = {
        push(2),
        push(3),
        plus(),
        dump(),
        push(500),
        push(80),
        minus(),
        dump(),

    };

    if (strcmp(argv[1], "sim") == 0)
    {
        simulate_program(program);
    }

    else if (strcmp(argv[1], "com") == 0)
    {
        compile_program(program, "output.asm");
        system("nasm -fmacho64 -o output.o output.asm");
        system("ld output.o -o output -macosx_version_min 11.0 -L /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem");
        system("rm output.asm output.o");
    }

    else
    {
        usage();
        std::cerr << "ERROR: unknown subcommand " << argv[1] << "\n";
        exit(EXIT_FAILURE);
    }
}