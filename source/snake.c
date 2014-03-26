#include "armtimer.h"
#include "lcd.h"
#include "snake.h"
#include "keyboard.h"
#include "gpio.h"

static volatile unsigned short int a[768];
static volatile int snLength;
static volatile int snHead;
static volatile int snTail;
static volatile int snDirection;
/*	 1
	0 2
	 3
*/

void snake(void){
	snLength = 1;
	int i;
	for(i=0;i<768;i++) a[i] = 0;
	a[0] = 999;	/*	HEAD	*/
	snHead = 0;
	snTail = 0;
	snDirection = 2;
	extTIrqHandler = snTIrqHandler;
	extKIrqHandler = snKIrqHandler;
	lcdSetCursor(0,0);
	lcdPixelsDraw(240*320, BACKGROUND_COLOUR);
}

void snTIrqHandler(void){
	lcdFillWindow((snTail/32)*10, ((snTail/32)*10)+9, (snTail%32)*10, ((snTail%32)*10)+9, BACKGROUND_COLOUR);
	if(snDirection == 0){
		a[snHead]=snHead - 1;
		snHead--;
		lcdFillWindow(((snHead/32)*10), ((snHead/32)*10)+9, ((snHead%32)*10)-10, ((snHead%32)*10)-1, BRICK_COLOUR);
	}
	if(snDirection == 1){
		a[snHead] = snHead - 32;
		snHead-=32;
		lcdFillWindow(((snHead/32)*10)-10, ((snHead/32)*10)-1, (snHead/32*10), ((snHead%32)*10)+9, BRICK_COLOUR);
	}
	if(snDirection == 2){
		a[snHead] = snHead + 1;
		snHead++;
		lcdFillWindow(((snHead/32)*10), ((snHead/32)*10)+9, ((snHead%32)*10)+10, ((snHead%32)*10)+19, BRICK_COLOUR);
	}
	if(snDirection == 3){
		a[snHead] = snHead + 32;
		snHead+=32;
		lcdFillWindow(((snHead/32)*10)+10, ((snHead/32)*10)+19, ((snHead%32)*10), ((snHead%32)*10)+9, BRICK_COLOUR);
	}
	snTail = a[snTail];
}

void snKIrqHandler(void){
	if (kBuffer == 1) snDirection = 1;
	else if (kBuffer == 4) snDirection = 0;
	else if (kBuffer == 5) snDirection = 3;
	else if (kBuffer == 6) snDirection = 2;
	snTIrqHandler();
}

