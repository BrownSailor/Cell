global print_bool
global println_bool

section .data
    tru db 'tru', 10
    fls db 'fls', 10

section .text
print_bool:
        sub     rsp, 8
        test    dil, dil
        je      .L2

        mov     eax, 1
        mov     edi, 1
        lea     rsi, [rel tru]
        mov     edx, 3

        syscall
        add     rsp, 8
        ret

.L2:
        mov     eax, 1
        mov     edi, 1
        lea     rsi, [rel fls]
        mov     edx, 3

        syscall
        add     rsp, 8
        ret

println_bool:
        sub     rsp, 8
        test    dil, dil
        je      .L2

        mov     eax, 1
        mov     edi, 1
        lea     rsi, [rel tru]
        mov     edx, 4
        
        syscall
        add     rsp, 8
        ret

.L2:
        mov     eax, 1
        mov     edi, 1
        lea     rsi, [rel fls]
        mov     edx, 4

        syscall
        add     rsp, 8
        ret
