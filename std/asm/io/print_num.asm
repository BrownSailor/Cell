global print_num
global println_num

; take a signed 64-bit integer and print it to console
print_num:
        sub     rsp, 40
        mov     rcx, rdi
        xor     r11d, r11d
        test    rdi, rdi
        jns     .L2
        neg     rcx
        mov     r11d, 1
.L2:
        xor     edi, edi
        mov     r8d, 31
        mov     r9, -3689348814741910323
.L3:
        mov     rax, rcx
        mov     r10, rdi
        add     rdi, 1
        mul     r9
        mov     rax, rcx
        shr     rdx, 3
        lea     rsi, [rdx+rdx*4]
        add     rsi, rsi
        sub     rax, rsi
        mov     rsi, rcx
        mov     rcx, rdx
        add     eax, 48
        mov     BYTE [rsp+r8], al
        mov     rax, r8
        lea     r8, [r8-1]
        cmp     rsi, 9
        ja      .L3
        test    r11b, r11b
        je      .L4
        lea     rdi, [r10+2]
        mov     eax, 32
        sub     rax, rdi
        mov     BYTE [rsp+rax], 45
.L4:
        lea     rsi, [rsp+rax]
        mov     rdx, rdi
        xor     eax, eax
        mov     edi, 1
        mov     rax, 1
        syscall
        add     rsp, 40
        ret

; take a signed 64-bit integer and print it to console
; along with a trailing new line
println_num:
        sub     rsp, 40
        mov     rcx, rdi
        xor     r11d, r11d
        mov     BYTE  [rsp+31], 10
        test    rdi, rdi
        jns     .L2
        neg     rcx
        mov     r11d, 1
.L2:
        mov     edi, 1
        mov     r8d, 30
        mov     r9, -3689348814741910323
        jmp     print_num.L3
