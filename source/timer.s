.text

.globl getTimerAddr
getTimerAddr:
	ldr r0, =0x20003000
	mov pc, lr
.globl waitCycles
waitCycles:		@ waits given number of cycles - r0
	cmp r0, $0
	subhi r0, r0, $1
	bhi waitCycles
	mov pc, lr
.globl wait	@ takes /*pointer to*/ interval (word value) in r0
/*	what is it doing? actually, it just adds interval
	to the stored somewhere value of counter, then
	repeatedly compares it with it...  :)
*/
wait:
	lowCounter	.req	r1
	highCounter	.req	r2
	interval	.req	r3
	push {lr}
@	ldr interval, [r0]
	mov interval, r0
	bl getTimerAddr
	ldr lowCounter, [r0, $4]
	ldr highCounter, [r0, $8]
	adds lowCounter, lowCounter, interval
	.unreq interval
	actualCounter	.req	r3
	addcs highCounter, highCounter, $1
	bcc lowLoop
highLoop:
	ldr actualCounter, [r0, $8]
	cmp actualCounter, highCounter
	blo highLoop
lowLoop:
	ldr actualCounter, [r0, $4]
	cmp actualCounter, lowCounter
	blo lowLoop
	pop {lr}
	mov pc, lr
.unreq actualCounter
.unreq highCounter
.unreq lowCounter
.globl pushCounter
pushCounter:
        lowCounter      .req    r1
        highCounter     .req    r2
        interval        .req    r3
        push {lr}
@       ldr interval, [r0]
        mov interval, r0
        bl getTimerAddr
        ldr lowCounter, [r0, $4]
        ldr highCounter, [r0, $8]
        adds lowCounter, lowCounter, interval
	addcs highCounter, highCounter, $1
.unreq interval

	ldr r0, =highCounterSave
	str highCounter, [r0]
	ldr r0, =lowCounterSave
	str lowCounter, [r0]

	pop {lr}
	mov pc, lr
.globl pollCounter
pollCounter:
        lowCounter      .req    r1
        highCounter     .req    r2
        actualCounter   .req    r3
	push {lr}
	ldr r0, =highCounterSave
	ldr highCounter, [r0]
	ldr r0, =lowCounterSave
	ldr lowCounter, [r0]
	bl getTimerAddr

        ldr actualCounter, [r0, $8]
        cmp actualCounter, highCounter
        blo notEnough

        ldr actualCounter, [r0, $4]
        cmp actualCounter, lowCounter
        blo notEnough

enough:
	mov r0, $1
        pop {lr}
        mov pc, lr

notEnough:
	mov r0, $0
	pop {lr}
	mov pc, lr

.data
lowCounterSave:
	.word 0
highCounterSave:
	.word 0
/*
.globl wait2
wait2:
	push {lr}
	channel		.req	r1
	interval	.req	r2
	csreg		.req	r3	@ control/status register
	lowCounter	.req	r4
	ldr interval, [r0]
	bl getTimerAddr
	mov csreg, $0
	str csreg, [r0]
	ldr lowCounter, [r0, $4]
	adds lowCounter, lowCounter, interval
	push {r0-r5}
	blcs flash2			@ low counter overloaded
	pop {r0-r5}
	add channel, channel, $3
@	str lowCounter, [r0, channel]
	str lowCounter, [r0, $12]
	sub channel, channel, $2
loop:
	ldr csreg, [r0]
@	lsr csreg, channel
	lsrs csreg, $1
	bcc loop
	pop {lr}
	mov pc, lr
*/
