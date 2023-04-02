segment .text
global _start
_start:
    mov     rdi, 420
    extern  println
    call    println
    mov     rax, 60
    mov     rdi, 0
    syscall
    ret
