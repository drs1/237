;;; super swervy random number generator
;;; (c) 2014 d.r.smith
main:	swi	#SysGetNum
	mvn	r1,r0
	adds	r1,r1,#1
	beq	done
loop:	swi	#SysEntropy
	swi	#SysPutNum
	mov	r0,#10
	swi	#SysPutChar
	adds	r1,r1,#1
	bne	loop
done:	mov	r0,#0
	swi	#SysPutNum
	mov	r0, #10
	swi	#SysPutChar
	swi	#SysHalt