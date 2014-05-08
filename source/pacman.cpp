#include "pacman.h"
void intToStr(int a, char *str);
int keyToDir(int key){
	switch(key){
	case 1:
		return 1;
		break;
	case 4:
		return 2;
		break;
	case 5:
		return 3;
		break;
	case 6:
		return 0;
		break;
	default:
		return 4;
		break;
	}
}

void pacmanGame(){
	char pointen[5];
	lcdSetCursor(0,0);
	lcdPixelsDraw(240*320, BACKGROUND_COLOUR);
	lcdSetBackgroundColour(BACKGROUND_COLOUR);
	lcdSetFontColour(10,56,19);
	lcdPrint(10,10, "PACMAN");
	wait(1000000);
	lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
	map myMap;
	myMap.putRectangle(1,10,0,10,2);
	myMap.putRectangle(1,10,10,14,2);
	myMap.putRectangle(10,13,0,5,2);
	myMap.putRectangle(10,13,5,14,2);
	myMap.setup();
	enemy one(1,0,&myMap);
	enemy two(1,14,&myMap);
	enemy three(5,0,&myMap);
	pacman four(1,8,&myMap);
	int e = 0;
	while(1){
		one.go();
		e++;
		lcdDrawChar(30,300,(char)(e%10)+48);
		two.go();
		three.go();
		four.go(keyToDir(kBuffer));
//		lcdRegWrite(0x03, (1 << 15) | (1 << 14) | (1 << 12) | (7 << 3));
		intToStr(153, pointen);
		lcdPrint(50,272,pointen);
		if(four.eaten==myMap.points){
			lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
			lcdPrint(112, 50, "WIN WIN WIN");
			return;
		}
		wait(10000);
	}
}
