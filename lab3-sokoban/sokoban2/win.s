# A translation of my version of win().
# (c) 2014 danny r. smith

# first, allocate room in the "data" area of the program
# for a globally accessable array, BDay.  It consists of 3 longs.
	.data
	.align	4  # start on a long-word boundary
	.globl	BDay
# .type and .size gdb a hint as to the meaning of "BDay"
	.type	BDay,@object
BDay:	
	.long	12,15,1960	# your birthday
	.size	BDay,.-BDay

# these are constants we might use in the code;
# ripped shamelessly from sokoban.h
	.equ	BOX,2
	.equ	STORE,4
	
# standard prolog for functions we write:
	.text
	.globl	win
	.type	win,@function
win:
	push	%rbx	#clear some space
	push	%rbp
	push	%r12
	push	%r13
	push	%r14
	push	%r15

	movq	%rdi, %rbx 	#l -> rbx
	call	width
	movq	%rax, %rbp	#w -> rbp
	movq	%rbx, %rdi	#put l back in params
	call 	height
	movq	%rax, %r12	#h -> r12d
	movq	$0, %r13	#r -> r13d
	movq	$0, %r14	#c ->r14d

f1:	cmpq	%r13, %r12	#if c<h
	jle	hilit		
f2:	cmpq	%r14, %rbp	#if r<w
	jle	endIN		#else end inner for loop
	movq	%rbx, %rdi	# move l into first parms
	movq	%r13, %rsi	#r into second params
	movq	%r14, %rdx	#c into third params
	call	get		#get(lrc)
	incq	%r14		# r++	
	movq	%rax, %rdi	#cell -> rdi
	testq	$BOX, %rdi	# cell & box
	jz	f2		
	testq	$STORE, %rdi	# cell & store
	jnz	f2		# !(cell & store)
	movq	$0, %rax	# return 0
	jmp	done
	
endIN:	incq	%r13
	movq	$0,%r14
	jmp 	f1
	
hendIN:	incq	%r13
	movq	$0,%r14
	jmp	hf1
	
hilit:	movq	$0, %r14
	movq	$0, %r13
hf1:	cmpq	%r13, %r12
	movq	$1, %rax
	jle	done
hf2:	cmpq	%r14, %rbp
	jle	hendIN
	movq	%rbx, %rdi
	movq	%r13, %rsi
	movq	%r14, %rdx
	call	get
	movq	%rax, %rdi
	testq 	$BOX, %rdi
	jz	incC
	movq	%rbx, %rdi
	movq	%r13, %rsi
	movq	%r14, %rdx
	call 	highlight
incC:	incq	%r14
	jmp	hf2

done:	popq	%r15	#we only highlight on win's so lets
	popq	%r14	#just do our end shit here
	popq	%r13
	popq	%r12
	popq	%rbp
	popq	%rbx
	ret	

# this computes the size of win.  "." is the current address
# this is a help to the debugger (should you need it)
	.size	win,.-win
