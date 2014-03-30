#include "irq.h"

volatile int irqLock;
volatile unsigned int *irqEnable1 = (unsigned int *)IRQ_ENABLE1;
volatile unsigned int *irqEnable2 = (unsigned int *)IRQ_ENABLE2;

void irqDisableSec(void){
	if(irqLock == 0){
		*(irqEnable2+3) = ((1 << 20) | (1 << 19) | (1 << 18) | (1 << 17));
		*(irqEnable1+3) = 2;
	}
	irqLock++;
}

void irqEnableSec(void){
	irqLock--;
	if(irqLock == 0){
		*irqEnable2 = ((1 << 20) | (1 << 19) | (1 << 18) | (1 << 17));
		*irqEnable1 = 2;
	}
}
