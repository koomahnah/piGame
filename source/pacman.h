#define	BACKGROUND_COLOUR	13,2,19
extern "C" {
#ifndef _LCD_H
#define _LCD_H
#include "lcd.h"
#endif
}

#define	RIGHT	0
#define LEFT	2
#define	UP	1
#define	DOWN	3

#define X_OFFSET	7
#define	Y_OFFSET	7
class map{
public:
	map();
	unsigned int getInfo(int row, int col);
	void putInfo(int row, int col, unsigned char info);
	int gameOver();
private:
	unsigned char terrain[15][5];
	int points;
};


class player{
public:
	void go(int dir);
	void put(int row, int col, int dir);
	void move(int dir);
	int lastDir;
protected:
	int x;
	int y;
	static char icon;
	map *pMap;
};

class pacman : public player
{
public:
	pacman(int row, int col, map *pp);
	void go(int dir);
	int eaten;
protected:
	static char icon;
	int detectCollision();
};

void pacmanGame();
