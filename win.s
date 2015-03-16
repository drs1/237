#translation of Danny Smith's version of win()
#(c) 2014 d. r. smith
	.data
	.align
	.globl
	.type 	BDay,@object
BDay:
	.long
	.size

	.equ 	BOX,2
	.equ 	STORE,4

#standard prolog for functions
	.text
	.globl 	win
	.type 	win,@function

win:	#w = width(l)
	#h = height(l)
	
	pushq	%rbx	#clear some space
	pushq	%rbp
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

	#copy l. we use it again so we gotta make
	#sure it doesn't get overwritten on the first
	#call to width
	movl	%rdi, %rbx	#l -> rbx
	call 	width
	movl	%rax, %rbp    	#w -> rbp
	movl	%rbx, %rdi	#put l back in params
	call 	height
	movl	%rax, %r12d	#h -> r12d
	movl	$0, %r13d	#r -> r13d
	movl	$0, %r14d	#c -> r14d
	
f1:	cmpl	%r13d, %r12d
	jge	hilit		#we got through all the cells
				#without returning, so highlight & win
	
f2:	cmpl	%r14d, %rbp	# w - c -> greater than zero if
	jge	endIN		# w > c, so jge
	movl	%rbx, %rdi	# move l into first parms
	movl	%r13d, %rsi	#r into second params
	movl	%r14d, %rdx	#c into third params
	call	cell		
	movl	%rax, %rdi	#cell -> rdi
	andl	$BOX, %rax	 
	jz	f2
	andl	$STORE, %rdi
	incl	%r14d
	jnz	f2
	movl	$0, %rax
	jg	done
	
endIN:	incl	%r13d
	jmp	f1
	
hilit:	popq	%r15	#we only highlight on win's so lets
	popq	%r14	#just do our end shit here
	popq	%r13
	popq	%r12
	popq	%rbp
	popq	%rbx
	movl 	$1,%rax
	jmp	done

done:	ret
	.size win,.-win
