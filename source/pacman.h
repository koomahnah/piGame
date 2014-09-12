#define BACKGROUND_COLOUR	12, 17, 38
#define FOREGROUND_COLOUR	52, 54, 61
#define	TUNNEL_COLOUR		0, 0, 0
#define BONUS_COLOUR		60, 5, 5
#define PACMAN_COLOUR		53,63,0
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
class enemy;
class map {
      public:
	map();
//	unsigned int getInfo(int row, int col);
//	void putInfo(int row, int col, unsigned char info);
	void putRectangle(int vs, int ve, int hs, int he,
			  unsigned char info);
	void putSpeedBonus(int row, int col);
	void redraw();
	int points;
	void setup();
      private:
//      unsigned char terrain[15][5];
	unsigned char terrain[15][20];
};


class player {
      public:
	void go(int dir);
	void put(int row, int col, int dir);
	struct colour *pb;
	struct colour *pf;
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
};

class pacman:public player {
      public:
	pacman(int row, int col, map * pp);
	int go(int dir);
	static int score;
	static int lifes;
	int eaten;
	enemy *collided;
      protected:
	static struct colour pacb;
	static struct colour pacf;
	int collision();
};

class enemy:public player {
	friend class pacman;
      public:
	 enemy(int row, int col, map * pp);
	~enemy();
	void go();
	static void goAll();
	static void respawnAll();
	static void blink(int b);
	int freeze;
      protected:
	static enemy *list[5];
	static int blinks;
	static struct colour eb;
	static struct colour ef;
};
void pacmanGame();
int rand();
void drawPoint(int row, int col);
void drawSpeedBonus(int row, int col);
void intToStr(int a, char *str);
int mainLoop(pacman * p, map * m, const char *win1, const char *win2,
	     const char *fail1, const char *fail2);
int keyToDir(int key);
int level1();
int level2();
int level3();
int level4();
