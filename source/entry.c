#include "lcd.h"
#include "gpio.h"
#include "keyboard.h"
#include "object.h"
#include "armtimer.h"

void intToStr(int a, char *str);
void entry(){
	lcdInit(32);
	keyboardInit();
	timerInit();
	lcdDisplayON();
//	greenAndRed();
	snake();
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
