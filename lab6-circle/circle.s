#a routine to give pairs of coordinates to draw a bresnham's circle
# (c) 2014 d.r.smith
	
	.globl	DrawCircle
	.type 	DrawCircle, @function

	# call frame
	# diwas	   -32
	# y        -28
	# x	   -24
	# e	   -20
	# display  -16
	# i	   -8
	# r	   -4
	# old bp   +0

	.equ	y, -28
	.equ	x, -24
	.equ	e, -20
	.equ	display, -16
	.equ	i, -8
	.equ	r, -4
	.equ	ptr_x, 0
	.equ	ptr_y, 4

DrawCircle:
	pushq	%rbp		#stack setup
	movq	%rsp, %rbp	
	subq	$32, %rsp

	movl	%edi, r(%rbp)	# r = edi
	movl 	%esi, i(%rbp)	# i = esi
	movq	%rdx, display(%rbp)	# display = rdx

	negl	%edi		# -r
	movl	%edi, e(%rbp)	# e = -r
	negl	%edi		# +r
	movl	%edi, x(%rbp)	# x = r
	movl	$0, y(%rbp)	# y = 0
	
while:	movl	y(%rbp), %r8d
	movl	x(%rbp), %esi
	cmpl	%r8d, %esi	# while x >= y
	jl	exit
	
	movq	display(%rbp), %rax	# display array in rax
	movl	i(%rbp), %r9d		# i in r9d
	movl	%esi, ptr_x(%rax,%r9,8)	# display[i].x = x
	movl	%r8d, ptr_y(%rax,%r9,8)	# display [i].y = y
	incl	%r9d		# i++
	movl	%r9d, i(%rbp)	# r9d -> i
	sall	$1, %r8d	# 2y		
	movl	e(%rbp), %ecx	# e -> ecx
	incl	%ecx		# e+1
	addl	%r8d, %ecx	# e+1+2y
	movl	%ecx, e(%rbp)	# e = e+1+2y
	movl	y(%rbp), %r8d	# y - r8d
	incl	%r8d		# y++
	movl	%r8d, y(%rbp)	# y = y++
	cmpl	$0, %ecx	# if e >= 0
	jl	over		# else
	
	incl	%ecx		# e+1	
	movl	x(%rbp), %edi	# x -> edi
	sall	$1, %edi	# 2x
	subl	%edi, %ecx	# e = e+1 - 2x
	movl	%ecx, e(%rbp)
	movl	x(%rbp), %edi
	decl	%edi		# x--
	movl	%edi, x(%rbp)
over:	jmp	while

exit:	movl	i(%rbp), %eax
	movq	%rbp, %rsp
	popq	%rbp
	ret
	
