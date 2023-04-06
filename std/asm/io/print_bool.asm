global print_bool
global println_bool

section .data
    tru db 'tru'
    fls db 'fls'

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
        call    print_bool

        mov     eax, 1
        mov     edi, 1
        mov     esi, 10
        mov     edx, 1

        syscall
        add     rsp, 8
        ret
