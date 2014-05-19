.globl getGpioAddr
getGpioAddr:
	ldr r0, =0x20200000
	mov pc, lr

.globl setGpioFunct		@ INPUT:
setGpioFunct:			@ r0: pin number
	cmp r0, $53
	cmpls r1, $7
	movhi r0, $1
	movhi pc, lr

	push {r4, r5}
	mov r2, r1
	mov r1, r0		@ r1: desired function
	push {lr}		@ functions are: (binary)
	bl getGpioAddr		@ 000 - input, 001 - output
	pop {lr}		@ 010-111 - alt
	mov r4, $0
subTen:
	cmp r1, $10
	subhs r1, r1, $10
	addhs r4, r4, $1
	bhs subTen

	mov r5, $4
	mul r3, r4, r5
	add r0, r0, r3		@ offset for the address
	mov r5, $3
	mul r3, r1, r5
	lsl r2, r3		@  destination value to left side
	ldr r4, =0b11000111111111111111111111111111
	mov r5, $9		@ that thing above is the mask
	sub r1, r5, r1
	mov r5, $3
	mul r3, r1, r5
	ror r4, r3
	ldr r5, [r0]
	and r5, r4
	orr r5, r2
	str r5, [r0]
	pop {r4, r5}
	mov r0, $0
	mov pc, lr

.globl setPin
setPin:			@ takes PIN number in r0
	cmp r0, $53
	movhi r0, $1
	movhi pc, lr
	push {r5}
	cmp r0, $32
	subhs r1, r0, $32	@ offset for value (for lsl) is in r1
	movlo r1, r0
	movhs r2, $32		@ offset for address
	movlo r2, $28		@ as above
	push {lr}
	bl getGpioAddr
	pop {lr}
	mov r5, $1
	lsl r5, r1
	str r5, [r0, r2]
	mov r0, $0
	pop {r5}
	mov pc, lr

.globl clearPin
clearPin:
        cmp r0, $53
        movhi r0, $1
        movhi pc, lr
	push {r5}
        cmp r0, $32
        subhs r1, r0, $32       @ offset for value (for lsl) is in r1
        movlo r1, r0
        movhs r2, $44           @ offset for address
        movlo r2, $40           @ as above
        push {lr}
        bl getGpioAddr
        pop {lr}
        mov r5, $1
        lsl r5, r1
        str r5, [r0, r2]
        mov r0, $0
	pop {r5}
        mov pc, lr
.globl flashTime
flashTime:		@ interval in r0
	push {r0-r5, lr}
	mov r5, r0
        mov r0, $16
        mov r1, $1
        bl setGpioFunct

        mov r0, $16
        bl clearPin
        mov r0, r5
        bl wait

        mov r0, $16
        bl setPin
        ldr r0, =waitInterval
	ldr r0, [r0]
        bl wait

        pop {r0-r5, lr}
        mov pc, lr
.globl flash
flash:
	push {r0, r1, r2, r4, lr}
	mov r0, $16
	mov r1, $1
	bl setGpioFunct

	mov r0, $16
	bl clearPin
	ldr r0, =waitInterval
	ldr r0, [r0]
	bl wait

	mov r0, $16
	bl setPin
	ldr r0, =waitInterval
	ldr r0, [r0]
	bl wait

	pop {r0, r1, r2, r4, lr}
	mov pc, lr

.globl getPinLevel
getPinLevel:		@ pin number in r0, please
	push {lr}
	cmp r0, $53
	movhi r0, $2
	movhi pc, lr

	cmp r0, $32
	subhs r1, r0, $31
	movhs r2, $0x38	@ address offset
	addlo r1, r0, $1
	movlo r2, $0x34
	bl getGpioAddr
	ldr r3, [r0, r2]
@	ldr r3, =0b01000000000
	lsrs r3, r1
	movcs r0, $1
	movcc r0, $0
	pop {lr}
	mov pc, lr
.globl setPull
setPull:		@ r0: pin number, r1: pull (0-no,1-down,2-up)
	cmp r0, $53
	cmpls r1, $2
	movhi r0, $1
	movhi pc, lr
	push {lr}

shift	.req	r2
offset	.req	r3
	cmp r0, $32
	movlo shift, r0
	subhs shift, r0, $32
	movlo offset, $0x98
	movhs offset, $0x9c

	bl getGpioAddr
	str r1, [r0, $0x94]
	mov r0, $150
	bl waitCycles

	bl getGpioAddr
	mov r1, $1
	lsl r1, shift
	str r1, [r0, offset]
	mov r0, $150
	bl waitCycles

	mov r1, $0
	bl getGpioAddr
	str r1, [r0, $0x94]
	str r1, [r0, $0x98]

	.unreq	shift
	.unreq	offset
	pop {lr}
	mov r0, $0
	mov pc, lr
.globl getEventDetSt		@ gets (and clears)  event detect status,
getEventDetSt:			@ takes pin number in r0,
	cmp r0, $53		@ returns value in r0
	movhi pc, lr

	push {r4-r7, lr}
	mov r4, r0
	bl getGpioAddr
	cmp r4, $32
	movlo r5, $0x40
	movhs r5, $0x44
	subhs r4, r4, $31
	addlo r4, r4, $1
	ldr r6, [r0, r5]
@	ldr r6, =0b10000100100
	lsrs r6, r4
	movcc r0, $0
	bcc end1
	movcs r1, $1

clear:	mov r6, $1
	sub r4, r4, $1
	lsl r6, r4
	str r6, [r0, r5]
	mov r0, r1
end1:	pop {r4-r7, lr}
	mov pc, lr
.globl setDetect		@ enables or disables events detect.
setDetect:			@ r0: pin number, r1: 1-enable, 0-disable
	@	r2:
	@	0 - rising edge detect
	@	1 - falling edge detect
	@	2 - high detect
	@	3 - low detect
	@	4 - async. rising edge detect
	@	5 - async. falling edge detect
	cmp r0, $53
	cmpls r1, $1
	cmpls r2, $5
	movhi r0, $1
	movhi pc, lr

	push {r4-r7, lr}
	mov r6, $0x4c
	mov r3, $12
	mov r5, r2
	mul r2, r5, r3
	add r6, r6, r2
	mov r4, r0
	bl getGpioAddr
	cmp r4, $32
	movlo r5, r6
	addhs r5, r6, $0x4
	subhs r4, r4, $31
	addlo r4, r4, $1
	ldr r6, [r0, r5]
	mov r7, r6
	lsrs r6, r4
	movcc r2, $0
	movcs r2, $1
	mov r6, r7
	cmp r2, r1
	beq end2		@ if it already fits desired, end

	mov r7, $1
	sub r4, r4, $1
	lsl r7, r4
	cmp r1, $1
	addeq r6, r6, r7
	subne r6, r6, r7
	str r6, [r0, r5]

end2:	pop {r4-r7, lr}
	mov r0, $0
	mov pc, lr
.section .data
waitInterval:	.word	800000
