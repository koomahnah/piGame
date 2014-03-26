#include "armtimer.h"
#include "gpio.h"
#include "object.h"
#include "lcd.h"

volatile unsigned int *timercs = (unsigned int *)TIMERCS;
volatile unsigned int *irqEnable1 = (unsigned int *)IRQENABLE1;
void (*extTIrqHandler)(void) = 0;
unsigned int timerMatch;
void timerInit()
{
	*irqEnable1=2;
	irqEnable();
	extTIrqHandler = 0;
//	unsigned int tmp = *timercs;
//	tmp |= (1<<5)|(1<<7);
}


void timerSetMatch(unsigned int value)
{
	unsigned int tmp = *(timercs+1);
	tmp+=value;
	*(timercs+4)=tmp;
	*timercs = 0b10;
	timerMatch = value;
}
void tIrqHandler(void){
	irqDisableSec();
	unsigned int tmp = *(timercs+1);
	tmp+=timerMatch;
	*(timercs+4)=tmp;
	*timercs=0b10;
	if(extTIrqHandler!=0) extTIrqHandler();
	irqEnableSec();
}
