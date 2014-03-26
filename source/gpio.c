#include "gpio.h"
volatile unsigned int* gpioBase = (unsigned int*) GPIO_BASE_ADDRESS;
volatile unsigned int* gpset0 = (unsigned int*) GPSET0;
volatile unsigned int* gpclear0 = (unsigned int*) GPCLEAR0;
volatile unsigned int* gppud = (unsigned int*) GPPUD;
volatile unsigned int* gpren0 = (unsigned int*) GPREN0;
volatile unsigned int* gplev0 = (unsigned int*) GPLEV0;

void setGpioFunct(int pinNumber, int function){
//	if(pinNumber>53) return;
	unsigned int tmp = *(gpioBase+(pinNumber/10));
	tmp &= ~(7<<((pinNumber%10)*3));
	tmp |= (function<<((pinNumber%10)*3));
	*(gpioBase+(pinNumber/10))=tmp;
}

void setPull(int pinNumber, int pull){
	*gppud=(pull&0x3);
	/*  in case pull is more than 2 */
	waitCycles(150);
	*(gppud+1+(pinNumber/32))=(1<<(pinNumber%32));
	/* worth to say that it is possible to clock
	   many lines at once				*/
	waitCycles(150);
	*gppud=0; *(gppud+1)=0; *(gppud+2)=0;
	/* maybe it is necessary, maybe it is not */
}

void flashValue(int value, char length){
	int i;
	for(i=0;i<length;i++){
                if(value%2==1) flashTime(LONG_INTERVAL);
                else flashTime(SHORT_INTERVAL);
                value=value>>1;
                }
}
