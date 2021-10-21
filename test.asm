	;; -- main --
	global _main
	section .text
_main:
	push rbp
	mov rbp, rsp
	mov [rbp - 0], rdi
	mov [rbp - 0], rsi

	;; -- return --
	mov rax, [rbp - 0]
	add rax, 0
	mov rax, [rax]
	pop rbp
	ret
