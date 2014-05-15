#include "pacman.h"
void intToStr(int a, char *str);
int mainLoop(pacman *p, const char *win1, const char *win2, const char *fail1, const char *fail2);

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
int level1(){
	lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
	map m;
	m.putRectangle(2,12,1,6,2);
	m.putRectangle(2,12,6,11,2);
	m.putSpeedBonus(2,6);
	m.setup();
	enemy one(2,1,&m);
	pacman four(2,11,&m);
	wait(2000000);
	return mainLoop(&four, "GREAT", "LEVEL 2", "PITIABLE", "TRY AGAIN");
}
int level2(){
	lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
	map m;
	m.putRectangle(1,10,0,10,2);
	m.putRectangle(1,10,10,14,2);
	m.putRectangle(1,4,10,14,2);
	m.putRectangle(1,6,0,10,2);
	m.putRectangle(10,13,0,5,2);
	m.putRectangle(10,13,5,14,2);
	m.putSpeedBonus(1,0);
	m.putSpeedBonus(13,10);
	m.setup();
	enemy one(1,0,&m);
	enemy two(2,0,&m);
	pacman four(1,8,&m);
	wait(2000000);
	return mainLoop(&four, "GREAT", "LEVEL 3", "PITIABLE", "TRY AGAIN");
}
int level3(){
	lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
	map m;
	m.putRectangle(0,4,0,4,2);
	m.putRectangle(4,8,0,4,2);
	m.putRectangle(8,12,0,7,2);
	m.putRectangle(0,6,4,7,2);
	m.putRectangle(0,8,7,12,2);
	m.putRectangle(8,12,7,12,2);
	m.putRectangle(0,4,12,16,2);
	m.putRectangle(4,8,12,16,2);
	m.putRectangle(8,12,12,16,2);
	m.putRectangle(4,6,12,16,2);
	m.putSpeedBonus(4,14);
	m.putSpeedBonus(0,0);
	m.setup();
	enemy one(1,0,&m);
	enemy two(2,0,&m);
	enemy three(3,0,&m);
	pacman five(8,12,&m);
	wait(2000000);
	return mainLoop(&five, "NOW WE WILL SEE", "LEVEL 4  ", "SEHR SCHLECHT", "TRY AGAIN  ");
}
int level4(){
	lcdFillWindow(0,239,0,319,BACKGROUND_COLOUR);
	map m;
	m.putRectangle(1,6,1,6,2);
	m.putRectangle(8,13,10,15,2);
	m.putSpeedBonus(13,15);
	m.setup();
	enemy one(1,1,&m);
	pacman two(8,10,&m);
	wait(2000000);
	return mainLoop(&two, " ", " ", " ", " ");
}
int mainLoop(pacman *p, const char *win1, const char *win2, const char *fail1, const char *fail2){
	int b=0;
	char tmp[10];
	struct colour black = {0, 0, 0};
	while(1){
		enemy::goAll();
		switch (p->go(keyToDir(kBuffer))){
		case 1:
			lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
			lcdPrint(112, 60, win1);
			wait(1000000);
			lcdPrint(112, 60, win2);
			wait(1000000);
			return 0;
			break;
		case 2:
			lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
			lcdPrint(112, 60, fail1);
			wait(1000000);
			lcdPrint(112, 60, fail2);
			wait(1000000);
			return 1;
			break;
		case 3:
			wait(1000000);
			p->put(-1, -1, 0);
			enemy::respawnAll();
			wait(1000000);
			break;
		case 4:
			enemy::blink(300);
			break;
		default:
			break;
		}
		lcdSetBackgroundColour(BACKGROUND_COLOUR);
		intToStr(p->eaten, tmp);
		lcdPrint(10,272,tmp);
		intToStr(p->lifes, tmp);
		lcdPrint(30,300,tmp);
		if(b==0) wait(7500);
		if(kBuffer==3) b = 1;
	}
}

void pacmanGame(){
	lcdSetBackgroundColour(BACKGROUND_COLOUR);
	lcdSetFontColour(FOREGROUND_COLOUR);
	lcdSetCursor(0,0);
	lcdPixelsDraw(240*320, BACKGROUND_COLOUR);
	lcdPrint(10,10, "PACMAN");
	wait(1000000);
	lcdSetCursor(0,0);
	lcdPixelsDraw(240*320, BACKGROUND_COLOUR);
	lcdPrint(112, 80, "LEVEL 1");
	wait(1000000);
	while(level1());
	while(level2());
	while(level3());
	while(level4());
}

