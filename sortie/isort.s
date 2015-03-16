# A method to perform insertion sort, based on "insertionSort" of sortie.c
# (c) 2014 d.r.smith
	.text
	.globl	isort
	.type	isort,@function
	
	# The Stack
	# size   -40
	# n      -36
	# right  -32		
	# left   -24
	# cf     -16
	# data   -8
	# old bp +0
	
	.equ	data, -8
	.equ	cf, -16
	.equ	left, -24
	.equ	right, -32
	.equ	n, -36
	.equ	size, -40
	
isort:
	pushq	%rbp			#stack setup
	movq	%rsp, %rbp
	subq	$40, %rsp
	
	movq	%rdi, data(%rbp)
	movl	%esi, n(%rbp)
	movl 	%edx, size(%rbp)
	movq	%rcx, cf(%rbp)
	
	movl	n(%rbp), %esi
	subl	$1, %esi		# n - 1  n>1 -< n-1 > 0
	jle	end			# if n > 1 else exit routine
	call	isort
	movl	n(%rbp), %esi
	subl	$1, %esi
	movl	size(%rbp),%eax		# move size to rax to mul
	mull	%esi			# nm1*size -> rax
	addq	data(%rbp),%rax		# (char*)data + nm1*size
	movq	%rax, right(%rbp)	# right = data + nm1* size
	movl	size(%rbp), %r8d
	subl	%r8d, %eax	
	movq	%rax, left(%rbp)

wloop:	movq	data(%rbp), %rax
	cmpq	%rax, left(%rbp)        
	jl	end
	movq	left(%rbp), %rdi
	movq	right(%rbp), %rsi
	call	*cf(%rbp)
	subl	$0, %eax
	jle	end
	movl	size(%rbp), %edi
	movq	left(%rbp), %rsi
	movq	right(%rbp), %rdx
	call	memswap
	movq	size(%rbp), %rax
	subq	%rax, left(%rbp)
	subq	%rax, right(%rbp)
	jmp	wloop

end:	movq	%rbp, %rsp
	popq	%rbp
	ret

	
	.size	isort,.-isort

