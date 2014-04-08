#include "timer.h"
#include "object.h"
#include "lcd.h"
#include "green_n_red.h"
#include "keyboard.h"

struct object brick = { 0, 0, 16, 16, 10, 56, 19 };
struct object flier = { 0, 0, 100, 10, 62, 10, 10 };

static volatile unsigned int greenDirection;
static volatile int greenCounter;
static volatile int redCounter;
static volatile int interval;
static volatile int level;

void greenAndRed(void){
	timerInit();
	greenDirection = 1;
	interval = 9800;
	level = 0;
	extTIrqHandler = gnrTIrqHandler;
	extKIrqHandler = gnrKIrqHandler;
	lcdSetCursor(0,0);
	lcdPixelsDraw(240*320, BACKGROUND_COLOUR);
	lcdSetBackgroundColour(BACKGROUND_COLOUR);
	lcdSetFontColour(BRICK_COLOUR);
	lcdPrint("LEVEL 0", 10, 10);
	objectDraw(&brick, 50, 152);
	objectDraw(&flier, 150, 10);
	greenCounter = 0;
	redCounter = 0;
	//timerSetMatch(interval);

	int r = 1;
	while(1){
		
		irqDisableSec();
		lcdSetFontColour(BRICK_COLOUR);
		lcdDrawChar(10, 246, (char)(greenCounter+48));
		lcdSetFontColour(63, 0, 0);
		lcdDrawChar(10, 294, (char)(redCounter+48));
		irqEnableSec();
		waitCycles(0x6fffff);	
		lcdFillWindow(10, 26, 246, 310, BACKGROUND_COLOUR);
		waitCycles(0x6fffff);	
	}	


}
void gnrTIrqHandler()
{
	if(greenDirection==0) objectMoveLeft(&flier);
	else objectMoveRight(&flier);
	if(flier.y==10) greenDirection=1;
	else if(flier.y== (310-flier.width) && greenDirection!=0) greenDirection=0;
	
	if(detectCollision(&brick, &flier)){
		greenCounter++;
		wait(500000);
		lcdFillWindow(brick.x, brick.x+brick.width - 1, brick.y, brick.y + brick.width - 1, BACKGROUND_COLOUR);
		objectDraw(&brick, 50, 152);
	}
	if(greenCounter>=5 || redCounter >= 5){
		lcdSetFontColour(63, 0, 0);
		if(greenCounter>=redCounter){
			lcdPrint("SEHR GUT", 112, 96);
			lcdFillWindow(10, 26, 106, 122, BACKGROUND_COLOUR);
			lcdSetFontColour(BRICK_COLOUR);
			if(flier.width >= 11){
				flier.width-=10;
				level++;
			}
			lcdDrawChar(10, 106, (char)(level+48));
		}
		else
			lcdPrint("SEHR SCHLECHT", 112, 56);
		wait(3000000);
		lcdFillWindow(112, 128, 56, 264, BACKGROUND_COLOUR);
		redCounter = 0;
		greenCounter = 0;
	}
	
	return;
}

void gnrKIrqHandler(){
	if (kBuffer == 1 && brick.x > 0) {
		objectMoveUp(&brick);
		wait(1500);
	} else if (kBuffer == 5 && brick.x < 224) {
		objectMoveDown(&brick);
		wait(1500);
	} 

	if(brick.x>=150){
		redCounter++;
		wait(500000);
		lcdFillWindow(brick.x, brick.x+brick.width - 1, brick.y, brick.y + brick.width - 1, BACKGROUND_COLOUR);
		objectDraw(&brick, 50, 152);
	}
	if(kBuffer == 0) lcdDrawChar(30, 10, (char)150);
	if(kBuffer == 4) lcdFillWindow(30, 46, 10, 26, BACKGROUND_COLOUR);
	if(kBuffer == 8){
		timerSetMatch(9800);	
	}

	kBuffer = 16;	
	/*else if (kBuffer == 6 && brick.y < 304) {
		objectMoveRight(&brick);
		wait(750);
	} else if (kBuffer == 4 && brick.y > 0) {
		objectMoveLeft(&brick);
		wait(750);
	}*/
}

