# a routine to return from a variable number of processes
# (c) d.r.smith

Unwind:	cmpl	%0, %edi
	je	end
	movq	%rbp, %rsp
	popq	%rbp
	decl	%edi
	
end:	movl	$0, %rax
	popq	%rsi
	jmp	%rsi