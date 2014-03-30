.section .text
.globl irqEnable
irqEnable:
	mrs r0, cpsr
	bic r1, r0, $0b10000000
	msr cpsr, r1
	bx lr
.globl irqDisable
irqDisable:
	mrs r0, cpsr
	mov r2, $0b10000000
	orr r2, r2, r0
	msr cpsr, r0
	bx lr

