#include "pacman.h"

char pacman::icon = 'V';
char player::icon = '@';

pacman::pacman(int row, int col, map *pp){
	x=row*16+8;
	y=col*16+8;
	lastDir = 0;
	pMap=pp;
	eaten = 0;
	lcdDrawChar(x-X_OFFSET, y-Y_OFFSET, icon);
}

void pacman::go(int dir){
	if( ((x-8)%16)!=0 || (y-8)%16!=0 ){
		this->move(lastDir);
		return;
	}
	int row = (x-8)/16;
	int col = (y-8)/16;
	int node = pMap->getInfo(row, col);
	if(node == 2){
		eaten++;
		pMap->putInfo(row,col, 1);
	}
	switch(dir)
	{
	case 0:
		if(col == 19) return;
		node = pMap->getInfo(row, col + 1);
		if(node==0) return;
		break;
	case 1:
		if(row == 0) return;
		node = pMap->getInfo(row - 1, col);
		if(node==0) return;
		break;
	case 2:
		if(col == 0) return;
		node = pMap->getInfo(row, col - 1);
		if(node==0) return;
		break;
	case 3:
		if(row == 14) return;
		node = pMap->getInfo(row + 1, col);
		if(node==0) return;
		break;
	default:
		return;
		break;
		this->lastDir = dir;
		this->move(dir);
	}
}
void player::move(int dir){
	switch(dir)
	{
	case 0:
		lcdRegWrite(0x03, (1 << 15) | (1 << 14) | (1 << 12) | (2 << 3));
		lcdFillWindow(x-X_OFFSET, x-X_OFFSET + 15, y-Y_OFFSET, y-Y_OFFSET, BACKGROUND_COLOUR);
		lcdDrawChar(x-X_OFFSET, y-Y_OFFSET + 1, icon);
		y++;
		break;
	case 1:
		lcdRegWrite(0x03, (1 << 15) | (1 << 14) | (1 << 12) | (7 << 3));
		lcdFillWindow(x-X_OFFSET + 15, x-X_OFFSET + 15, y-Y_OFFSET, y-Y_OFFSET + 15, BACKGROUND_COLOUR);
		lcdDrawChar(x-X_OFFSET - 1, y-Y_OFFSET, icon);
		x--;
		break;
	case 2:
		lcdRegWrite(0x03, (1 << 15) | (1 << 14) | (1 << 12) | (6 << 3));
		lcdFillWindow(x-X_OFFSET, x-X_OFFSET + 15, y-Y_OFFSET + 15, y-Y_OFFSET + 15, BACKGROUND_COLOUR);
		lcdDrawChar(x-X_OFFSET, y-Y_OFFSET - 1, icon);
		y--;
		break;
	case 3:
		lcdRegWrite(0x03, (1 << 15) | (1 << 14) | (1 << 12) | (5 << 3));
		lcdFillWindow(x-X_OFFSET, x-X_OFFSET, y-Y_OFFSET, y-Y_OFFSET + 15, BACKGROUND_COLOUR);
		lcdDrawChar(x-X_OFFSET + 1, y-Y_OFFSET, icon);
		x++;
		break;

	default:
		break;
		return;
	}
}

void player::put(int row, int col, int dir){
	lcdFillWindow(x-X_OFFSET, x-X_OFFSET+15, y-Y_OFFSET, y-Y_OFFSET+15, BACKGROUND_COLOUR);
	switch(dir)
	{
	case 0:
		lcdRegWrite(0x03, (1 << 15) | (1 << 14) | (1 << 12) | (2 << 3));
		break;
	case 1:
		lcdRegWrite(0x03, (1 << 15) | (1 << 14) | (1 << 12) | (7 << 3));
		break;
	case 2:
		lcdRegWrite(0x03, (1 << 15) | (1 << 14) | (1 << 12) | (6 << 3));
		break;
	case 3:
		lcdRegWrite(0x03, (1 << 15) | (1 << 14) | (1 << 12) | (5 << 3));
		break;
	default:
		break;
	}
	x=row*16+8;
	y=col*16+8;
	lcdDrawChar(x-X_OFFSET, y-Y_OFFSET, icon);
	lastDir=dir;
}	
map::map(){
	for(int i=0;i<75;i++) terrain[i/5][i%5] = 0;
	points = 0;
}

unsigned int map::getInfo(int row, int col){
	return (unsigned int)(terrain[row][col/4] & (3<<((col%4)*2)))>>((col%4)*2);	
}

void map::putInfo(int row, int col, unsigned char info){
	unsigned char tmp = terrain[row][col/4];
	tmp&=(~(3<<((col%4)*2)));
	tmp|=(info<<((col%4)*2));
	terrain[row][col/4] = tmp;
	if(info==2) points++;
}

int map::gameOver(){
	if(points==0) return 1;
	else return 0;
}
