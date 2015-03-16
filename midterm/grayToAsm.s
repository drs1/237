

	movl	n(%rbp), %edi
	movl	%edi, test(%rbp)
	movl	$0, gi(%rbp)
loop:	movl	test(%rbp), %esi
	cmpl	$0, %esi
	je	over
	movl	i(%rbp), %edi
	shrl	%esi, %edi
	andl	$1, %edi
	movl	%edi, on(%rbp)
	movl	i(%rbp), %edi
	decl	%edi
	andl	$1, %edi
	movl	%edi, next(%rbp)
	movl	gi(%rbp), %edx
	cmpl	$0, %edx
	jne	out
	movl	on(%rbp), %ecx
	cmpl	$0, %ecx
	je	out
	movl	test(%rbp) %esi
	sall	$1, %esi
	movl	gi(%rbp),%r8d
	addl	%r8d,%esi
	movl	%esi, gi(%rbp)
out:	movl	gi(%rbp), %esi
	movl	on(%rbp), %r8d
	movl	next(%rbp), %r9d
	movl	test(%rbp), %edi
	decl	%edi
	xorl	%r9d,%r8d
	sall	%edi,%r8d
	addl	%r8d,%esi
	movl	%esi, gi(%rbp)
	jmp	loop
over:	
	