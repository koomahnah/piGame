#include "lcd.h"
#include "keyboard.h"
#include "object.h"
#include "timer.h"
#include "irq.h"

void intToStr(int a, char *str);
void entry(){
	irqLock = 0;
	lcdInit(32);
	lcdExtEntryFunct = irqDisableSec;
	lcdExtExitFunct = irqEnableSec;
	keyboardInit();
//	timerInit();
	lcdDisplayON();
	greenAndRed();
//	snake();
	return;
}

void intToStr(int a, char *str){
	if(a>9999) a=666;
	int len=0, tmp=a;
	do{
		tmp/=10;
		len++;
	} while (tmp>0);
	int i;
	for (i=0;i<len;i++){
		str[len-i-1]=a%10 + 48;
		a/=10;
	}
	str[len]='\0';
}
