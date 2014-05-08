#define	BACKGROUND_COLOUR	13,2,19
#define FOREGROUND_COLOUR	10,56,19
extern "C" {
#ifndef _LCD_H
#define _LCD_H
#include "lcd.h"
#endif
#include "keyboard.h"
}
#define	RIGHT	0
#define LEFT	2
#define	UP	1
#define	DOWN	3

#define X_OFFSET	8
#define	Y_OFFSET	8
class map{
public:
	map();
	unsigned int getInfo(int row, int col);
	void putInfo(int row, int col, unsigned char info);
	void putRectangle(int vs, int ve, int hs, int he, unsigned char info);
	int points;
	void setup();
private:
	unsigned char terrain[15][5];
};


class player{
public:
	void go(int dir);
	void put(int row, int col, int dir);
protected:
	void move(int dir);
	int lastDir;
	int x;
	int y;
	map *pMap;
	int goodDir(int dir);
	char icon;
};

class pacman : public player
{
public:
	pacman(int row, int col, map *pp);
	void go(int dir);
	int eaten;
protected:
	int detColl();
};

class enemy : public player
{
public:
	enemy(int row, int col, map *pp);
	void go();
};
void pacmanGame();
int rand();
void drawPoint(int row, int col);
