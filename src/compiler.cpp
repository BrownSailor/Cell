#include "compiler.h"

static void compile_built_ins(std::ostream &out)
{
    out << "print:\n";
    out << "    mov   r8, -3689348814741910323\n";
    out << "    sub   rsp, 40\n";
    out << "    lea   rcx, [rsp+31]\n";
    out << ".L2:\n";
    out << "    mov   rax, rdi\n";
    out << "    mul   r8\n";
    out << "    mov   rax, rdi\n";
    out << "    shr   rdx, 3\n";
    out << "    lea   rsi, [rdx+rdx*4]\n";
    out << "    add   rsi, rsi\n";
    out << "    sub   rax, rsi\n";
    out << "    mov   rsi, rcx\n";
    out << "    sub   rcx, 1\n";
    out << "    add   eax, 48\n";
    out << "    mov   BYTE [rcx+1], al\n";
    out << "    mov   rax, rdi\n";
    out << "    mov   rdi, rdx\n";
    out << "    cmp   rax, 9\n";
    out << "    ja    .L2\n";
    out << "    lea   rdx, [rsp+32]\n";
    out << "    mov   edi, 1\n";
    out << "    xor   eax, eax\n";
    out << "    sub   rdx, rsi\n";
    out << "    mov   rax, 1\n";
    out << "    syscall\n";
    out << "    add   rsp, 40\n";
    out << "    ret\n\n";

    out << "println:\n";
    out << "    mov   r8, -3689348814741910323\n";
    out << "    sub   rsp, 40\n";
    out << "    mov   BYTE [rsp+31], 10\n";
    out << "    lea   rcx, [rsp+30]\n";
    out << "    jmp   print.L2\n";
    out << "    ret\n\n";
}

void compile_program(Node *root)
{
    (void)root;
    std::cout << "segment .text\n";

    /*
     * add pre-written assembly to file
     * TODO: hold all of these in a relocatable .o file to be linked
     */
    compile_built_ins(std::cout);

    std::cout << "global _start\n";
    std::cout << "_start:\n";

    /* TODO: iterate through program operations and generate assembly */

    std::cout << "    mov   rdi, 69\n";
    std::cout << "    extern println\n";
    std::cout << "    call  println\n";
    std::cout << "    mov   rax, 60\n";
    std::cout << "    mov   rdi, 0\n";
    std::cout << "    syscall\n";
    std::cout << "    ret\n";
}
