.extern entry
//.extern __stack_chk_guard_setup
.section .init
.globl _start
_start:
	ldr pc, reset_handler
	ldr pc, undefined_handler
	ldr pc, swi_handler
	ldr pc, pref_abort_handler
	ldr pc, data_abort_handler
	ldr pc, dummy_handler
	ldr pc, irq_handler
	ldr pc, fiq_handler
reset_handler:		.word	reset
undefined_handler:	.word	dummy
swi_handler:		.word	dummy
pref_abort_handler:	.word   dummy
data_abort_handler:	.word   dummy
dummy_handler:		.word   dummy
irq_handler:		.word   irq
fiq_handler:		.word   dummy
reset:
	mov r0, $0x0000
	mov r1, $0x8000
	ldmia r1!, {r2,r3,r4,r5,r6,r7,r8,r9}
	stmia r0!, {r2,r3,r4,r5,r6,r7,r8,r9}
        ldmia r1!, {r2,r3,r4,r5,r6,r7,r8,r9}
        stmia r0!, {r2,r3,r4,r5,r6,r7,r8,r9}

	mov sp, $0x8000

@ set stack in irq mode
	mrs r0, cpsr
	bic r0, $0b11111111
	orr r0, $0b11010010
	msr cpsr_c, r0
	mov sp, $0x4000

@ come back to svc mode
	mrs r0, cpsr
        bic r0, $0b11111111
        orr r0, $0b11010011
        msr cpsr_c, r0

@ fill .bss section with zeros
	ldr r0, =_bss_start
	ldr r1, =_bss_end
	mov r2, $0
loop:
	cmp r0, r1
	strlo r2, [r0], $4
	blo loop

@	b main
@	bl __stack_chk_guard_setup
	bl entry
dummy:
	b dummy

irq:
        push {r0-r12, lr}
	ldr r0, =intrTrace
	mov r1, $1
	str r1, [r0]
	ldr r0, =0x20003000	@ check timer cs register
	ldr r1, [r0]		@ if it's not timer, go to keyboard
	ands r1, r1, $0b10	@ if result is zero, Z flag is set
	beq keyboard		@ eq <=> Z==1
timer:
	bl tIrqHandler
	pop {r0-r12, lr}
	subs pc, lr, $4
keyboard:
	ldr r0, =0x20200040
	ldr r1, =0x8068000	@ clear event statuses
	str r1, [r0]
	bl kIrqHandler
	pop {r0-r12, lr}
	subs pc, lr, $4
/*.globl main
main:
mainLoop:
	b mainLoop*/
