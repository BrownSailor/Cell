	.file	"main.c"
	.text
	.p2align 4
	.globl	insertionSort
	.type	insertionSort, @function
insertionSort:
.LFB11:
	.cfi_startproc
	leaq	-1(%rsi), %rcx
	testq	%rcx, %rcx
	jle	.L1
	movq	$-1, %r8
	movq	$-1, %rsi
	.p2align 4,,10
	.p2align 3
.L5:
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L4:
	movq	(%rdi,%rax,8), %rdx
	cmpq	%rdx, %rsi
	jge	.L3
	movq	%rax, %r8
	movq	%rdx, %rsi
.L3:
	addq	$1, %rax
	cmpq	%rax, %rcx
	jg	.L4
	movq	%rcx, (%rdi,%rcx,8)
	movq	%r8, (%rdi,%r8,8)
	subq	$1, %rcx
	jne	.L5
.L1:
	ret
	.cfi_endproc
.LFE11:
	.size	insertionSort, .-insertionSort
	.p2align 4
	.globl	binarySearch
	.type	binarySearch, @function
binarySearch:
.LFB12:
	.cfi_startproc
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L16:
	leaq	(%rdx,%rsi), %r8
	movq	%r8, %rax
	shrq	$63, %rax
	addq	%r8, %rax
	sarq	%rax
	movq	%rax, %r8
	cmpq	%rcx, (%rdi,%rax,8)
	je	.L9
	jle	.L15
	movq	%rax, %rdx
.L11:
	cmpq	%rsi, %rdx
	jg	.L16
	movq	$-1, %r8
.L9:
	movq	%r8, %rax
	ret
	.p2align 4,,10
	.p2align 3
.L15:
	leaq	1(%rax), %rsi
	jmp	.L11
	.cfi_endproc
.LFE12:
	.size	binarySearch, .-binarySearch
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC10:
	.string	"%ld"
.LC11:
	.string	""
.LC12:
	.string	"%ld\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB13:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	leaq	.LC10(%rip), %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$184, %rsp
	.cfi_def_cfa_offset 240
	movdqa	.LC0(%rip), %xmm0
	movq	%fs:40, %rax
	movq	%rax, 168(%rsp)
	xorl	%eax, %eax
	movq	%rsp, %r15
	leaq	160(%rsp), %rbp
	movaps	%xmm0, (%rsp)
	movdqa	.LC1(%rip), %xmm0
	movq	%r15, %r14
	movq	%r15, %rbx
	movaps	%xmm0, 16(%rsp)
	movdqa	.LC2(%rip), %xmm0
	movaps	%xmm0, 32(%rsp)
	movdqa	.LC3(%rip), %xmm0
	movaps	%xmm0, 48(%rsp)
	movdqa	.LC4(%rip), %xmm0
	movaps	%xmm0, 64(%rsp)
	movdqa	.LC5(%rip), %xmm0
	movaps	%xmm0, 80(%rsp)
	movdqa	.LC6(%rip), %xmm0
	movaps	%xmm0, 96(%rsp)
	movdqa	.LC7(%rip), %xmm0
	movaps	%xmm0, 112(%rsp)
	movdqa	.LC8(%rip), %xmm0
	movaps	%xmm0, 128(%rsp)
	movdqa	.LC9(%rip), %xmm0
	movaps	%xmm0, 144(%rsp)
	.p2align 4,,10
	.p2align 3
.L18:
	movq	(%rbx), %rsi
	movq	%r12, %rdi
	xorl	%eax, %eax
	addq	$8, %rbx
	call	printf@PLT
	movl	$32, %edi
	call	putchar@PLT
	cmpq	%rbx, %rbp
	jne	.L18
	leaq	.LC11(%rip), %r13
	movl	$19, %ebx
	movq	%r13, %rdi
	call	puts@PLT
	movq	$-1, %rsi
	movq	$-1, %rcx
	.p2align 4,,10
	.p2align 3
.L21:
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L20:
	movq	(%r15,%rax,8), %rdx
	cmpq	%rdx, %rcx
	jge	.L19
	movq	%rax, %rsi
	movq	%rdx, %rcx
.L19:
	addq	$1, %rax
	cmpq	%rax, %rbx
	jne	.L20
	movq	%rbx, (%r15,%rbx,8)
	movq	%rsi, (%rsp,%rsi,8)
	subq	$1, %rbx
	jne	.L21
	.p2align 4,,10
	.p2align 3
.L22:
	movq	(%r14), %rsi
	movq	%r12, %rdi
	xorl	%eax, %eax
	addq	$8, %r14
	call	printf@PLT
	movl	$32, %edi
	call	putchar@PLT
	cmpq	%r14, %rbp
	jne	.L22
	movq	%r13, %rdi
	call	puts@PLT
	movl	$20, %edx
	jmp	.L24
	.p2align 4,,10
	.p2align 3
.L39:
	leaq	(%rdx,%rbx), %rax
	sarq	%rax
	cmpq	$15, (%rsp,%rax,8)
	je	.L23
	jle	.L38
	movq	%rax, %rdx
.L24:
	cmpq	%rbx, %rdx
	jg	.L39
	orq	$-1, %rax
.L23:
	movq	(%rsp,%rax,8), %rsi
	leaq	.LC12(%rip), %rdi
	xorl	%eax, %eax
	call	printf@PLT
	movq	168(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L40
	addq	$184, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L38:
	.cfi_restore_state
	leaq	1(%rax), %rbx
	jmp	.L24
.L40:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE13:
	.size	main, .-main
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.quad	20
	.quad	19
	.align 16
.LC1:
	.quad	18
	.quad	17
	.align 16
.LC2:
	.quad	16
	.quad	15
	.align 16
.LC3:
	.quad	14
	.quad	13
	.align 16
.LC4:
	.quad	12
	.quad	11
	.align 16
.LC5:
	.quad	10
	.quad	9
	.align 16
.LC6:
	.quad	8
	.quad	7
	.align 16
.LC7:
	.quad	6
	.quad	5
	.align 16
.LC8:
	.quad	4
	.quad	3
	.align 16
.LC9:
	.quad	2
	.quad	1
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
