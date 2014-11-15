#include "timer.h"
#include "gpio.h"
#include "object.h"
#include "irq.h"

volatile unsigned int *timercs = (unsigned int *)TIMERCS;
void (*extTIrqHandler)(void);
void (*tIrqHandler)(void);
static unsigned int timerMatch;
void timerInit()
{
	*irqEnable1=2;
	irqEnable();
	extTIrqHandler = 0;
	tIrqHandler = tCircHandler;
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

void timerOneShot(unsigned int value, void (*pFunct)(void)){
	unsigned int tmp = *(timercs+1);
	tmp+=value;
	*(timercs+4)=tmp;
	*timercs = 0b10;
	__timerPFunct = pFunct;
	tIrqHandler = tOneShotHandler;
}
void tOneShotHandler(void){
	if(__timerPFunct!=0) __timerPFunct();
	tIrqHandler=tCircHandler;
	__timerPFunct = 0;
}
void tCircHandler(void){
	unsigned int tmp = *(timercs+1);
	tmp+=timerMatch;
	*(timercs+4)=tmp;
	*timercs=0b10;
	if(extTIrqHandler!=0) extTIrqHandler();
}
