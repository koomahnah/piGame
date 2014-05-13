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
	enemy three(3,0,&m);
	pacman four(1,8,&m);
	wait(2000000);
	return mainLoop(&four, "GREAT", "LEVEL 2", "PITIABLE", "TRY AGAIN");
}

int mainLoop(pacman *p, const char *win1, const char *win2, const char *fail1, const char *fail2){
	int b=0;
	char tmp[10];
	int i = 0;
	struct colour pfOld;
	while(1){
		if(i>0) i--;
		if((i%4)==0){
			if((i%8)==4){
				pfOld=player::pf;
				player::pf = { 0, 0, 0 };
				enemy::goAll();
				player::pf = pfOld;
			}
			else enemy::goAll();
		}
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
			i = 0;
			wait(1000000);
			break;
		case 4:
			i=400;
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
}

