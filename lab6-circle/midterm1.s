	# call frame
	# diwas    -24
	# hlp      -19
	# dst	   -18
	# src      -17
	# s	   -16
	# l	   -8
	# g	   -4  
	# old bp   +0


	.equ	hlp, -19
	.equ	dst, -18
	.equ	src, -17
	.equ	s, -16
	.equ	l, -8
	.equ	g, -4


g2toh:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$24, %rsp


	movl	%edi, g(%rbp)
	movl	%esi, l(%rbp)
	movq	%rdx, s(%rbp)
	mov	%cl,src(%rbp)
	mov	%r8b, dst(%rbp)
	mov	%r9b, hlp(%rbp)
	
	
	cmpl	$0, %esi
	jz	endzero
else:	decl	%esi
	movl	%esi, l(%rbp)
	movl	$1, %r9d	#hlp must be restore
	sall	%r9d, %esi
	andl	%edi, %r9d
	jz	over		#g must be restored
	
	leaq	dst(%rbp), %rdx
	movq	%rdx, s(%rbp)
	incl	%rdx

	movl	g(%rbp),%edi
	movl	i(%rbp),%esi
	movl	dst(%rbp),%cl
	movl	hlp(%rbp),%r8b
	movl	src(%rbp),%r9b
	
	jmp	recur
	
over:	leaq	src(%rbp),%rdx
	movq	%rdx, s(%rbp)
	incl 	%rdx

	movl	g(%rbp),%edi
	movl	i(%rbp),%esi
	movl	src(%rbp),%cl
	movl	hlp(%rbp),%r8b
	movl	dst(%rbp),%r9b

	jmp	recur
	
recur:	call	g2toh
	

ezero:	movq	$0, %rax
	ret	
	
	