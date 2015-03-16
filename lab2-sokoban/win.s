	.file	"win.c"
	.globl	BDay
	.data
	.align 4
	.type	BDay, @object
	.size	BDay, 12
BDay:
	.long	12
	.long	15
	.long	1960
	.text
	.globl	win
	.type	win, @function
win:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	width
	movl	%eax, -12(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	height
	movl	%eax, -8(%rbp)
	movl	$0, -20(%rbp)
	jmp	.L2
.L7:
	movl	$0, -16(%rbp)
	jmp	.L3
.L6:
	movl	-16(%rbp), %edx
	movl	-20(%rbp), %ecx
	movq	-40(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	get
	movb	%al, -1(%rbp)
	movsbl	-1(%rbp), %eax
	andl	$2, %eax
	testl	%eax, %eax
	je	.L4
	movsbl	-1(%rbp), %eax
	andl	$4, %eax
	testl	%eax, %eax
	jne	.L4
	movl	$0, %eax
	jmp	.L5
.L4:
	addl	$1, -16(%rbp)
.L3:
	movl	-16(%rbp), %eax
	cmpl	-12(%rbp), %eax
	jl	.L6
	addl	$1, -20(%rbp)
.L2:
	movl	-20(%rbp), %eax
	cmpl	-8(%rbp), %eax
	jl	.L7
	movl	$1, %eax
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	win, .-win
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
