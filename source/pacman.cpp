#include "pacman.h"
void intToStr(int a, char *str);
int mainLoop(pacman *p, map *m, const char *win1, const char *win2, const char *fail1, const char *fail2);

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
	return mainLoop(&four, &m, "TIME FOR", "LEVEL 2", "PITIABLE", "TRY AGAIN");
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
	return mainLoop(&four, &m, "GREAT", "LEVEL 3", "PITIABLE", "TRY AGAIN");
}
int level3(){
	lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
	map m;
/*	m.putRectangle(0,4,0,4,2);
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
	*/
	m.putRectangle(1,7,0,8,2);
	m.putRectangle(7,13,0,8,2);
	m.putRectangle(5,9,8,12,2);
	m.putRectangle(1,7,12,16,2);
	m.putRectangle(7,13,12,16,2);
	m.putSpeedBonus(7,8);
	m.setup();
	enemy one(1,16,&m);
	enemy two(13,16,&m);
	enemy three(7,16,&m);
	pacman five(7,0,&m);
	wait(2000000);
	return mainLoop(&five, &m, "NOW WE WILL SEE", "LEVEL 4", "SEHR SCHLECHT", "TRY AGAIN  ");
}
int level4()
{
	lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
	map m;
	m.putRectangle(1,3,1,4,2);
	m.putRectangle(3,5,1,4,2);
	m.putRectangle(5,7,1,4,2);
	m.putRectangle(7,9,1,4,2);
	m.putRectangle(9,11,1,4,2);
	m.putRectangle(11,13,1,4,2);
	m.putRectangle(1,6,4,6,2);
	m.putRectangle(6,11,4,6,2);
	m.putRectangle(1,13,6,15,2);
	m.putRectangle(4,13,6,15,2);
	m.putRectangle(1,3,15,18,2);
	m.putRectangle(3,5,15,18,2);
	m.putRectangle(5,7,15,18,2);
	m.putRectangle(7,9,15,18,2);
	m.putRectangle(9,11,15,18,2);
	m.putRectangle(11,13,15,18,2);

	m.putSpeedBonus(7,2);
	m.putSpeedBonus(1,10);
	m.putSpeedBonus(13,18);
	m.putSpeedBonus(2,1);
	m.putSpeedBonus(1,3);
	m.setup();
	enemy one(1,9,&m);
	enemy two(1,12,&m);
	enemy three(4,9,&m);
	enemy four(4,12,&m);
	pacman five(1,1,&m);
	wait(2000000);
	return mainLoop(&five, &m, "NOW WE WILL SEE", "LEVEL 4", "SEHR SCHLECHT", "TRY AGAIN  ");
}
int mainLoop(pacman *p, map *m, const char *win1, const char *win2, const char *fail1, const char *fail2){
	int b=0;
	char tmp[10];
	struct colour black = {0, 0, 0};

	lcdPrint(0,0,"SCORE: "); 
	lcdPrint(0,176,"HP: ");
	intToStr(pacman::lifes, tmp);
	lcdPrint(0,240,tmp);

	while(1){
		enemy::goAll();
		switch (p->go(keyToDir(kBuffer))){
		case 1: // all points gathered
			lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
			lcdPrint(112, 60, win1);
			wait(1000000);
			lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
			lcdPrint(112, 60, win2);
			wait(1000000);
			return 0;
			break;
		case 2:	// collision, no lifes remaining
			lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
			lcdPrint(112, 60, fail1);
			wait(1000000);
			lcdFillWindow(0,239,0,319, BACKGROUND_COLOUR);
			lcdPrint(112, 60, fail2);
			wait(1000000);
			return 1;
			break;
		case 3:	// collision, lifes remaining
			intToStr(pacman::lifes, tmp);
			lcdPrint(0,240,tmp);
			wait(1000000);
			p->put(-1, -1, 0);
			enemy::respawnAll();
			m->redraw();
			wait(1000000);
			break;
		case 4: // speedbonus
			enemy::blink(300);
			break;
		case 5:
			(p->collided)->put(-1,-1,0);
			(p->collided)->freeze = 100;
			p->score+=20;
		default:
			break;
		}
		lcdSetBackgroundColour(BACKGROUND_COLOUR);
		intToStr(p->score, tmp);
		lcdPrint(0,112, tmp);
		if(b==0) wait(7500);
		if(kBuffer==3) b = 1;
		else if(kBuffer==7) pacman::lifes=9;
	}
}

void pacmanGame(){
	lcdSetBackgroundColour(BACKGROUND_COLOUR);
	lcdSetFontColour(FOREGROUND_COLOUR);
	lcdSetCursor(0,0);
	lcdPixelsDraw(240*320, BACKGROUND_COLOUR);
	lcdPrint(10,10, "PACMAN");
	wait(1000000);
	lcdPrint(50,10, "MOVE WITH 2,4,5,6");
	wait(1000000);
	lcdPrint(76,10, "PRESS A TO START");
	while(kBuffer!=3) continue;
	kBuffer=16;
	lcdFillWindow(0,239,0,319,BACKGROUND_COLOUR);
	lcdPrint(112, 80, "LEVEL 1");
	wait(1000000);
	pacman::lifes=3;
	while(1){
		if(level1()){
			pacman::lifes=3;
			pacman::score=0;
			continue;
		}
		if(level2()){
			pacman::lifes=3;
			pacman::score=0;
			continue;
		}

		if(level3()){
			pacman::lifes=3;
			pacman::score=0;
			continue;
		}

		if(level4()){
			pacman::lifes=3;
			pacman::score=0;
			continue;
		}
	}

}

