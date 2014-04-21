#include "pacman.h"

void pacmanGame(){
	lcdSetCursor(0,0);
	lcdPixelsDraw(240*320, BACKGROUND_COLOUR);
	lcdSetBackgroundColour(BACKGROUND_COLOUR);
	lcdSetFontColour(10,56,19);
	map myMap;
	for(int i=0;i<10;i++){
		myMap.putInfo(1, i, 2);
		myMap.putInfo(i+1, 0, 2);
		myMap.putInfo(10, i, 2);
		myMap.putInfo(i+1, 9, 2);
	}
	pacman myPacman(1,0,&myMap);
	for(int i=0;i<64;i++){
		myPacman.go(0);
		lcdDrawChar(10, 294, (myPacman.eaten%10)+48);
		wait(100000);
	}
	while(1){
		myPacman.go((myPacman.lastDir+3)%4);
		myPacman.go(myPacman.lastDir);
		lcdDrawChar(10, 294, (myPacman.eaten%10)+48);
		wait(100000);
	}
	/*int i;
	for(int a=0;a<32;a++){
		for(i=0;i<40;i++){
			myPacman.move(RIGHT);
			wait(10000);
		}
		for(i=0;i<40;i++){
			myPacman.move(DOWN);
			wait(10000);
		}
		for(i=0;i<40;i++){
			myPacman.move(LEFT);
			wait(10000);
		}
		for(i=0;i<40;i++){
			myPacman.move(UP);
			wait(10000);
		}
	}
	myMap.putInfo(0,0,1);
	myMap.putInfo(0,1,1);
	myMap.putInfo(0,2,1);
	myPacman.put(0,0,0);
	while(1) myPacman.go(RIGHT);
	*/
}
