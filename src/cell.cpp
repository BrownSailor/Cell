/* Cell:    a stack-based programming language written in C++
 * Author:  Shreyas Ravi
 * Created: September 24 2021
 */

#include <iostream>
#include <cassert>
#include <stack>
#include <cstring>
#include <fstream>
#include <vector>

// Operations
enum
{
    OP_PUSH,  // push value to stack
    OP_PLUS,  // add top two values on stack
    OP_MINUS, // subtrace top two values on stack
    OP_DUMP,  // print top value on stack to console
    LEN_OPS   // number of valid operations
};

void push(int ret[2], int num)
{
    ret[0] = OP_PUSH;
    ret[1] = num;
}

void plus(int ret[2])
{
    ret[0] = OP_PLUS;
    ret[1] = 0;
}

void minus(int ret[2])
{
    ret[0] = OP_MINUS;
    ret[1] = 0;
}

void dump(int ret[2])
{
    ret[0] = OP_DUMP;
    ret[1] = 0;
}

void simulate_program(int program[][2], int size)
{
    std::stack<int> st;
    for (int i = 0; i < size; i++)
    {
        if (program[i][0] == OP_PUSH)
        {
            int a = program[i][1];
            st.push(a);
        }

        else if (program[i][0] == OP_PLUS)
        {
            int a = st.top();
            st.pop();

            int b = st.top();
            st.pop();

            st.push(a + b);
        }

        else if (program[i][0] == OP_MINUS)
        {
            int a = st.top();
            st.pop();

            int b = st.top();
            st.pop();

            st.push(b - a);
        }

        else if (program[i][0] == OP_DUMP)
        {
            int a = st.top();
            st.pop();

            std::cout << a << "\n";
        }
    }
}

void compile_program(int program[][2], int size)
{
    std::ofstream out;
    out.open("output.asm");

    // Assembly code for printing a number to the console, aka dumping
    // Written in C and translated to assembly using GodBolt

    out << "dump:\n";
    out << "    mov     r9, -3689348814741910323\n";
    out << "    sub     rsp, 40\n";
    out << "    mov     BYTE [rsp+31], 10\n";
    out << "    lea     rcx, [rsp+30]\n";
    out << ".L2:\n";
    out << "    mov     rax, rdi\n";
    out << "    lea     r8, [rsp+32]\n";
    out << "    mul     r9\n";
    out << "    mov     rax, rdi\n";
    out << "    sub     r8, rcx\n";
    out << "    shr     rdx, 3\n";
    out << "    lea     rsi, [rdx+rdx*4]\n";
    out << "    add     rsi, rsi\n";
    out << "    sub     rax, rsi\n";
    out << "    add     eax, 48\n";
    out << "    mov     BYTE [rcx], al\n";
    out << "    mov     rax, rdi\n";
    out << "    mov     rdi, rdx\n";
    out << "    mov     rdx, rcx\n";
    out << "    sub     rcx, 1\n";
    out << "    cmp     rax, 9\n";
    out << "    ja      .L2\n";
    out << "    lea     rax, [rsp+32]\n";
    out << "    mov     edi, 1\n";
    out << "    sub     rdx, rax\n";
    out << "    lea     rsi, [rsp+32+rdx]\n";
    out << "    mov     rdx, r8\n";
    out << "    mov     rax, 0x02000004\n";
    out << "    syscall\n";
    out << "    add     rsp, 40\n";
    out << "    ret\n";

    out << "\n        global      _main\n";
    out << "        section     .text\n";
    out << "_main:\n";

    for (int i = 0; i < size; i++)
    {
        // Push a value to the top of the stack
        if (program[i][0] == OP_PUSH)
        {
            out << "\n        ;; push " << program[i][1] << "\n";
            out << "        push \t\t" << program[i][1] << "\n";
        }

        // Add top two values on the stack
        else if (program[i][0] == OP_PLUS)
        {
            out << "\n        ;; plus\n";
            out << "        pop \t\trax\n";
            out << "        pop \t\trbx\n";
            out << "        add \t\trax, rbx\n";
            out << "        push \t\trax\n";
        }

        // Subtract top two values on the stack
        // TODO: handle negatives
        else if (program[i][0] == OP_MINUS)
        {
            out << "\n        ;; minus\n";
            out << "        pop \t\trax\n";
            out << "        pop \t\trbx\n";
            out << "        sub rbx, rax\n";
            out << "        push rbx\n";
        }

        // Dump value in stack to the rdi register and print
        else if (program[i][0] == OP_DUMP)
        {
            out << "\n        ;; dump\n";
            out << "        pop \t\trdi\n";
            out << "        call \t\tdump\n";
        }
    }

    // Exit the program without any issues
    out << ("        mov         rax, 0x02000001\n");
    out << ("        mov         rdi, 0\n");
    out << ("        syscall\n");

    out.close();
}

void usage()
{
    std::cout << "Usage: cell <SUBCOMMAND> [ARGS]\n";
    std::cout << "SUBCOMMANDS:\n";
    std::cout << "    sim <file>       Simulate the program\n";
    std::cout << "    com <file>       Compile the program\n";
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        usage();
        std::cerr << "ERROR: no subcommands found\n";
        exit(1);
    }

    int program[4][2];

    push(program[0], 13);
    push(program[1], 56);
    minus(program[2]);
    dump(program[3]);

    char *subcommand = argv[1];
    if (strcmp(subcommand, "sim") == 0)
    {
        simulate_program(program, 4);
    }

    else if (strcmp(subcommand, "com") == 0)
    {
        compile_program(program, 4);
        system("nasm -fmacho64 -o output.o output.asm");
        system("ld output.o -o output -macosx_version_min 11.0 -L /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem");
    }

    else
    {
        usage();
        std::cerr << "ERROR: unknown subcommand " << subcommand << "\n";
        exit(1);
    }
}