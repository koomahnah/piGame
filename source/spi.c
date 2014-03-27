#include "spi.h"

volatile unsigned int *spiControl = (unsigned int *)SPI_BASE_ADDRESS;
volatile unsigned int *spiFIFO = (unsigned int *)SPI_BASE_ADDRESS + 1;
volatile unsigned int *spiClock = (unsigned int *)SPI_BASE_ADDRESS + 2;

volatile int irqLock;
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

