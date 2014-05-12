/*#define	BACKGROUND_COLOUR	13,2,19
#define BACKGROUND_COLOUR	15, 4, 22
#define FOREGROUND_COLOUR	10,56,19
#define	TUNNEL_COLOUR		0, 0, 0
*/
#define BACKGROUND_COLOUR	14, 22, 48
#define FOREGROUND_COLOUR	52, 54, 61
#define	TUNNEL_COLOUR		0, 0, 0
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
	int goodDir(int dir);
	map *pMap;
	int lastDir;
	int x;
	int y;
	int defRow;
	int defCol;
	char icon;
	static struct colour pb;
	static struct colour pf;
};

class pacman : public player
{
public:
	pacman(int row, int col, map *pp);
	int go(int dir);
	int lifes;
	int eaten;
protected:
	int collision();
};

class enemy : public player
{
	friend class pacman;
public:
	enemy(int row, int col, map *pp);
	~enemy();
	void go();
protected:
	static enemy * list[5];
};
void pacmanGame();
int rand();
void drawPoint(int row, int col);
