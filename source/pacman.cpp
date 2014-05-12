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
		return 0;
		break;
	}
}

void pacmanGame(){
	lcdSetCursor(0,0);
	lcdPixelsDraw(240*320, BACKGROUND_COLOUR);
	lcdSetBackgroundColour(BACKGROUND_COLOUR);
	lcdSetFontColour(FOREGROUND_COLOUR);
	lcdPrint(10,10, "PACMAN");
	wait(1000000);
	lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
	map myMap;
	myMap.putRectangle(1,10,0,10,2);
	myMap.putRectangle(1,10,10,14,2);
	myMap.putRectangle(1,4,10,14,2);
	myMap.putRectangle(1,6,0,10,2);
	myMap.putRectangle(10,13,0,5,2);
	myMap.putRectangle(10,13,5,14,2);
	myMap.setup();
	lcdSetBackgroundColour(TUNNEL_COLOUR);
	enemy one(1,0,&myMap);
	enemy two(1,14,&myMap);
	enemy three(5,0,&myMap);
	pacman four(1,8,&myMap);
	wait(3000000);
	int b=0;
	char tmp[10];
	while(1){
		one.go();
		two.go();
		three.go();
		switch (four.go(keyToDir(kBuffer))){
		case 1:
			lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
			lcdPrint(112, 60, "NOT THAT BAD");
			wait(1000000);
			lcdPrint(112, 60, "WIN WIN WIN ");
			while(1) continue;
			return;
			break;
		case 2:
			lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
			lcdPrint(112, 60, "U RETARDED");
			wait(1000000);
			lcdPrint(112, 60, " GAME OVER ");
			while(1) continue;
			return;
			break;
		case 3:
			wait(1000000);
			four.put(-1, -1, 0);
			one.put(-1,-1, 0);
			two.put(-1,-1, 0);
			three.put(-1,-1, 0);
			wait(1000000);
			break;
		default:
			break;
		}
		lcdSetBackgroundColour(BACKGROUND_COLOUR);
		intToStr(four.eaten, tmp);
		lcdPrint(10,272,tmp);
		intToStr(four.lifes, tmp);
		lcdPrint(30,300,tmp);
		if(b==0) wait(8000);
		if(kBuffer==3) b = 1;
	}
}
