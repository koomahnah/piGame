#include "pacman.h"

pacman::pacman(int row, int col, map *pp){
	x=row*16+8;
	y=col*16+8;
	lastDir = 0;
	pMap=pp;
	eaten = 0;
	icon = 'V';
	lcdDrawChar(x-X_OFFSET, y-Y_OFFSET, icon);
}

enemy::enemy(int row, int col, map *pp){
	x=row*16+8;
	y=col*16+8;
	lastDir = 0;
	pMap=pp;
	icon = 'O';
	lcdDrawChar(x-X_OFFSET, y-Y_OFFSET, icon);
}

void pacman::go(int dir){
	if( (x%16)!=8 || (y%16)!=8 ){
		if( (dir&1)==(lastDir&1)){
			this->move(dir);
			lastDir=dir;
		}
		else
			this->move(lastDir);
		return;
	}

	int row = x/16;
	int col = y/16;
	if(pMap->getInfo(row, col) == 2){
		eaten++;
		pMap->putInfo(row,col, 1);
	}
	if(goodDir(dir)){
		this->lastDir = dir;
		this->move(dir);
	} else if(goodDir(lastDir)){
		this->move(lastDir);
	} else return;
}
int player::goodDir(int dir){
	int row = x/16;
	int col = y/16;
	switch(dir)
	{
	case 0:
		if(col == 19) return 0;
		if(pMap->getInfo(row, col + 1)==0) return 0;
		break;
	case 1:
		if(row == 0) return 0;
		if(pMap->getInfo(row - 1, col)==0) return 0;
		break;
	case 2:
		if(col == 0) return 0;
		if(pMap->getInfo(row, col - 1)==0) return 0;
		break;
	case 3:
		if(row == 14) return 0;
		if(pMap->getInfo(row + 1, col)==0) return 0;
		break;
	default:
		return 0;
		break;
	}
	return 1;
}
void player::move(int dir){
	switch(dir)
	{
	case 0:
		lcdRegWrite(0x03, (1 << 15) | (1 << 14) | (1 << 12) | (2 << 3));
		lcdFillWindow(x-X_OFFSET, x-X_OFFSET + 15, y-Y_OFFSET, y-Y_OFFSET, TUNNEL_COLOUR);
		lcdDrawChar(x-X_OFFSET, y-Y_OFFSET + 1, icon);
		y++;
		break;
	case 1:
		lcdRegWrite(0x03, (1 << 15) | (1 << 14) | (1 << 12) | (7 << 3));
		lcdFillWindow(x-X_OFFSET + 15, x-X_OFFSET + 15, y-Y_OFFSET, y-Y_OFFSET + 15, TUNNEL_COLOUR);
		lcdDrawChar(x-X_OFFSET - 1, y-Y_OFFSET, icon);
		x--;
		break;
	case 2:
		lcdRegWrite(0x03, (1 << 15) | (1 << 14) | (1 << 12) | (6 << 3));
		lcdFillWindow(x-X_OFFSET, x-X_OFFSET + 15, y-Y_OFFSET + 15, y-Y_OFFSET + 15, TUNNEL_COLOUR);
		lcdDrawChar(x-X_OFFSET, y-Y_OFFSET - 1, icon);
		y--;
		break;
	case 3:
		lcdRegWrite(0x03, (1 << 15) | (1 << 14) | (1 << 12) | (5 << 3));
		lcdFillWindow(x-X_OFFSET, x-X_OFFSET, y-Y_OFFSET, y-Y_OFFSET + 15, TUNNEL_COLOUR);
		lcdDrawChar(x-X_OFFSET + 1, y-Y_OFFSET, icon);
		x++;
		break;

	default:
		break;
	}
	return;
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

void enemy::go(){
	int row, col;
	int xm = x%16;
	int ym = y%16;
	if( xm!=8 || ym!=8 ){
		switch(lastDir){
		case 0:
			if(ym!=1){
				this->move(lastDir);
				return;
			}
			row = x/16;
			col = (y-17)/16;
			break;
		case 1:
			if(xm!=15){
				this->move(lastDir);
				return;
			}
			row = (x+9)/16;
			col = y/16;
			break;
		case 2:
			if(ym!=15){
				this->move(lastDir);
				return;
			}
			row = x/16;
			col = (y+9)/16;
			break;
		case 3:
			if(xm!=1){
				this->move(lastDir);
				return;
			}
			row = (x-17)/16;
			col = y/16;
			break;
		default:
			return;
			break;
		}
		if(pMap->getInfo(row,col)==2)
			drawPoint(row,col);
		this->move(lastDir);
		return;
	}
	row = x/16;
	col = y/16;
	int dir;
	while(1){
		dir=rand()%4;
		wait(100);
		if(dir==(lastDir+2)%4) continue;
		else if(goodDir(dir)){
			this->move(dir);
			this->lastDir = dir;
			return;
		}
		else continue;
	}
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
}
void map::putRectangle(int vs, int ve, int hs, int he, unsigned char info){
	lcdFillWindow(vs*16,(ve*16)+16,hs*16,(hs*16)+16, TUNNEL_COLOUR);
	lcdFillWindow(vs*16,(ve*16)+16,he*16,(he*16)+16, TUNNEL_COLOUR);
	lcdFillWindow(vs*16,(vs*16)+16,hs*16,(he*16)+16, TUNNEL_COLOUR);
	lcdFillWindow(ve*16,(ve*16)+16,hs*16,(he*16)+16, TUNNEL_COLOUR);

	for(int i=0;i<=(he-hs);i++){
		this->putInfo(vs,i+hs,info);
		this->putInfo(ve,i+hs,info);
		drawPoint(vs, i+hs);
		drawPoint(ve, i+hs);
	}
	for(int i=0;i<=(ve-vs);i++){
		this->putInfo(i+vs,hs,info);
		this->putInfo(i+vs,he,info);
		drawPoint(i+vs,hs);
		drawPoint(i+vs,he);
	}
}

void map::setup(){
	points = 0;
	for(int i=0;i<300;i++)
		if(this->getInfo(i%15,i/15)==2) points++;
}
/* value returned between 0 and 45	*/
int rand(){
	unsigned int *c = (unsigned int*) 0x20003004; // timer CLO
	unsigned int a = *c;
	int v = 0;
	for(int i=0;i<5;i++){
		v+=(a%10);
		a/=10;
	}
	return v;
}

void drawPoint(int row, int col){
	int x = row*16+8;
	int y = col*16+8;
	lcdFillWindow(x-1,x,y-1,y,FOREGROUND_COLOUR);
}
