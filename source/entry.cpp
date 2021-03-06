extern "C" {
#ifndef _LCD_H
#define _LCD_H
#include "lcd.h"
#endif
#include "keyboard.h"
//#include "object.h"
#include "timer.h"
#include "irq.h"
}
#include "pacman.h"
extern "C" void greenAndRed(void);
void intToStr(int a, char *str);
extern "C" void entry(){
	irqLock = 0;
	lcdInit(32);
	lcdExtEntryFunct = irqDisableSec;
	lcdExtExitFunct = irqEnableSec;
	keyboardInit();
	timerInit();
	lcdDisplayON();
//	greenAndRed();
	pacmanGame();
	while(1) continue;
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
