#include "pacman.h"
enemy *enemy::list[] = { 0, 0, 0, 0, 0 };
struct colour enemy::eb = { TUNNEL_COLOUR };
struct colour enemy::ef = { FOREGROUND_COLOUR };
struct colour pacman::pacb = { TUNNEL_COLOUR };
struct colour pacman::pacf = { PACMAN_COLOUR };

int enemy::blinks = 0;
int pacman::score = 0;
int pacman::lifes = 0;

map::map()
{
	for (int i = 0; i < 300; i++)
		terrain[i / 20][i % 20] = 0;
//      for(int i=0;i<75;i++) terrain[i/5][i%5] = 0;
	points = 0;
}

unsigned int map::getInfo(int row, int col)
{
	return (unsigned int) terrain[row][col];
//      return (unsigned int)(terrain[row][col/4] & (3<<((col%4)*2)))>>((col%4)*2);     
}

void map::putInfo(int row, int col, unsigned char info)
{
/*	unsigned char tmp = terrain[row][col/4];
	tmp&=(~(3<<((col%4)*2)));
	tmp|=(info<<((col%4)*2));
	terrain[row][col/4] = tmp;
	*/
	terrain[row][col] = info;
}

void map::putRectangle(int vs, int ve, int hs, int he, unsigned char info)
{
	lcdFillWindow(vs * 16, (ve * 16) + 16, hs * 16, (hs * 16) + 16,
		      TUNNEL_COLOUR);
	lcdFillWindow(vs * 16, (ve * 16) + 16, he * 16, (he * 16) + 16,
		      TUNNEL_COLOUR);
	lcdFillWindow(vs * 16, (vs * 16) + 16, hs * 16, (he * 16) + 16,
		      TUNNEL_COLOUR);
	lcdFillWindow(ve * 16, (ve * 16) + 16, hs * 16, (he * 16) + 16,
		      TUNNEL_COLOUR);

	for (int i = 0; i <= (he - hs); i++) {
		this->putInfo(vs, i + hs, info);
		this->putInfo(ve, i + hs, info);
		drawPoint(vs, i + hs);
		drawPoint(ve, i + hs);
	}
	for (int i = 0; i <= (ve - vs); i++) {
		this->putInfo(i + vs, hs, info);
		this->putInfo(i + vs, he, info);
		drawPoint(i + vs, hs);
		drawPoint(i + vs, he);
	}
}

void map::putSpeedBonus(int row, int col)
{
	this->putInfo(row, col, 6);
	drawSpeedBonus(row, col);
}

void map::redraw()
{
	int tmp;
	for (int i = 0; i < 300; i++) {
		tmp = this->getInfo(i % 15, i / 15);
		if (tmp == 2)
			drawPoint(i % 15, i / 15);
		else if (tmp == 6)
			drawSpeedBonus(i % 15, i / 15);
	}
}

void map::setup()
{
	points = 0;
	for (int i = 0; i < 300; i++) {
		int tmp = this->getInfo(i % 15, i / 15);
		if (tmp == 2 | tmp == 6)
			points++;
	}
}

int player::goodDir(int dir)
{
	int row = x / 16;
	int col = y / 16;
	switch (dir) {
	case 0:
		if (col == 19)
			return 0;
		if (pMap->getInfo(row, col + 1) == 0)
			return 0;
		break;
	case 1:
		if (row == 0)
			return 0;
		if (pMap->getInfo(row - 1, col) == 0)
			return 0;
		break;
	case 2:
		if (col == 0)
			return 0;
		if (pMap->getInfo(row, col - 1) == 0)
			return 0;
		break;
	case 3:
		if (row == 14)
			return 0;
		if (pMap->getInfo(row + 1, col) == 0)
			return 0;
		break;
	default:
		return 0;
		break;
	}
	return 1;
}

void player::move(int dir)
{
	switch (dir) {
	case 0:
		lcdRegWrite(0x03,
			    (1 << 15) | (1 << 14) | (1 << 12) | (2 << 3));
		lcdFillWindow(x - X_OFFSET, x - X_OFFSET + 15,
			      y - Y_OFFSET, y - Y_OFFSET, TUNNEL_COLOUR);
		lcdDrawCharC(x - X_OFFSET, y - Y_OFFSET + 1, icon, pb, pf);
		y++;
		break;
	case 1:
		lcdRegWrite(0x03,
			    (1 << 15) | (1 << 14) | (1 << 12) | (7 << 3));
		lcdFillWindow(x - X_OFFSET + 15, x - X_OFFSET + 15,
			      y - Y_OFFSET, y - Y_OFFSET + 15,
			      TUNNEL_COLOUR);
		lcdDrawCharC(x - X_OFFSET - 1, y - Y_OFFSET, icon, pb, pf);
		x--;
		break;
	case 2:
		lcdRegWrite(0x03,
			    (1 << 15) | (1 << 14) | (1 << 12) | (6 << 3));
		lcdFillWindow(x - X_OFFSET, x - X_OFFSET + 15,
			      y - Y_OFFSET + 15, y - Y_OFFSET + 15,
			      TUNNEL_COLOUR);
		lcdDrawCharC(x - X_OFFSET, y - Y_OFFSET - 1, icon, pb, pf);
		y--;
		break;
	case 3:
		lcdRegWrite(0x03,
			    (1 << 15) | (1 << 14) | (1 << 12) | (5 << 3));
		lcdFillWindow(x - X_OFFSET, x - X_OFFSET, y - Y_OFFSET,
			      y - Y_OFFSET + 15, TUNNEL_COLOUR);
		lcdDrawCharC(x - X_OFFSET + 1, y - Y_OFFSET, icon, pb, pf);
		x++;
		break;

	default:
		break;
	}
	return;
}

void player::put(int row, int col, int dir)
{
	if (row == -1)
		row = defRow;
	if (col == -1)
		col = defCol;
	lcdFillWindow(x - X_OFFSET, x - X_OFFSET + 15, y - Y_OFFSET,
		      y - Y_OFFSET + 15, TUNNEL_COLOUR);
	switch (dir) {
	case 0:
		lcdRegWrite(0x03,
			    (1 << 15) | (1 << 14) | (1 << 12) | (2 << 3));
		break;
	case 1:
		lcdRegWrite(0x03,
			    (1 << 15) | (1 << 14) | (1 << 12) | (7 << 3));
		break;
	case 2:
		lcdRegWrite(0x03,
			    (1 << 15) | (1 << 14) | (1 << 12) | (6 << 3));
		break;
	case 3:
		lcdRegWrite(0x03,
			    (1 << 15) | (1 << 14) | (1 << 12) | (5 << 3));
		break;
	default:
		break;
	}
	x = row * 16 + 8;
	y = col * 16 + 8;
	lcdDrawCharC(x - X_OFFSET, y - Y_OFFSET, icon, pb, pf);
	lastDir = dir;
}

pacman::pacman(int row, int col, map * pp)
{
	x = row * 16 + 8;
	y = col * 16 + 8;
	lastDir = 0;
	pMap = pp;
	eaten = 0;
	icon = 'V';
	defRow = row;
	defCol = col;
	pb = &pacb;
	pf = &pacf;
	lcdDrawCharC(x - X_OFFSET, y - Y_OFFSET, icon, pb, pf);
}

int pacman::go(int dir)
{
	if (this->eaten == pMap->points)
		return 1;
	if (this->collision()) {
		if (enemy::blinks > 0)
			return 5;
		pacman::lifes--;
		if (pacman::lifes == -1) {
			return 2;
		} else {
			return 3;
		}
	}
	if ((x % 16) != 8 || (y % 16) != 8) {
		if ((dir & 1) == (lastDir & 1)) {
			this->move(dir);
			lastDir = dir;
		} else
			this->move(lastDir);
		return 0;
	}
	int speedBonus = 0;
	int row = x / 16;
	int col = y / 16;
	int tmp = pMap->getInfo(row, col);
	if (tmp == 6) {
		speedBonus = 1;
		pMap->putInfo(row, col, 1);
		score += 10;
		eaten++;
	} else if (tmp == 2) {
		score++;
		eaten++;
		pMap->putInfo(row, col, 1);
	}
	if (goodDir(dir)) {
		this->lastDir = dir;
		this->move(dir);
	} else if (goodDir(lastDir))
		this->move(lastDir);
	if (speedBonus)
		return 4;
	else
		return 0;
}

int pacman::collision()
{
	for (int i = 0; i < 5; i++) {
		if (enemy::list[i] != 0) {
			if ((enemy::list[i])->freeze > 0)
				continue;
			int xe = (enemy::list[i])->x;
			int ye = (enemy::list[i])->y;
			int sqDist = (this->x - xe) * (this->x - xe);
			sqDist += (this->y - ye) * (this->y - ye);
			if (sqDist <= 150) {
				this->collided = enemy::list[i];
				return 1;
			}
		}
	}
	return 0;
}

enemy::enemy(int row, int col, map * pp)
{
	x = row * 16 + 8;
	y = col * 16 + 8;
	lastDir = 0;
	pMap = pp;
	icon = 'O';
	defRow = row;
	defCol = col;
	pb = &eb;
	pf = &ef;
	freeze = 0;
	lcdDrawCharC(x - X_OFFSET, y - Y_OFFSET, icon, pb, pf);
	for (int i = 0; i < 5; i++)
		if (list[i] == 0) {
			list[i] = this;
			break;
		}
}

enemy::~enemy()
{
	//lcdFillWindow(x-X_OFFSET, x-X_OFFSET+15, y-Y_OFFSET, y-Y_OFFSET+15, TUNNEL_COLOUR);
	for (int i = 0; i < 5; i++)
		if (list[i] == this) {
			list[i] = 0;
			break;
		}
}

void enemy::go()
{
	if (this->freeze > 0) {
		this->freeze--;
		lcdDrawCharC(x - X_OFFSET, y - Y_OFFSET + 1, icon, pb, pf);
		return;
	}
	int row, col;
	int xm = x % 16;
	int ym = y % 16;
	if (xm != 8 || ym != 8) {
		switch (lastDir) {
		case 0:
			if (ym != 1) {
				this->move(lastDir);
				return;
			}
			row = x / 16;
			col = (y - 17) / 16;
			break;
		case 1:
			if (xm != 15) {
				this->move(lastDir);
				return;
			}
			row = (x + 9) / 16;
			col = y / 16;
			break;
		case 2:
			if (ym != 15) {
				this->move(lastDir);
				return;
			}
			row = x / 16;
			col = (y + 9) / 16;
			break;
		case 3:
			if (xm != 1) {
				this->move(lastDir);
				return;
			}
			row = (x - 17) / 16;
			col = y / 16;
			break;
		default:
			return;
			break;
		}
		int tmp = pMap->getInfo(row, col);
		if (tmp == 6)
			drawSpeedBonus(row, col);
		else if (tmp == 2)
			drawPoint(row, col);
		this->move(lastDir);
		return;
	}
	row = x / 16;
	col = y / 16;
	int dir;
	while (1) {
		dir = rand() % 4;
		wait(100);
		if (dir == (lastDir + 2) % 4)
			continue;
		else if (goodDir(dir)) {
			this->move(dir);
			this->lastDir = dir;
			return;
		} else
			continue;
	}
}

void enemy::blink(int b)
{
	enemy::blinks = b;
}

void enemy::goAll()
{
	struct colour black = { 0, 0, 0 };
	if (enemy::blinks > 0)
		enemy::blinks--;
	for (int i = 0; i < 5; i++) {
		if (enemy::list[i] != 0 && (enemy::blinks % 8) == 4) {
			(enemy::list[i])->pf = &black;
			(enemy::list[i])->go();
			(enemy::list[i])->pf = &enemy::ef;
		} else if (enemy::list[i] != 0 && (enemy::blinks % 4) == 0)
			(enemy::list[i])->go();
	}
}

void enemy::respawnAll()
{
	for (int i = 0; i < 5; i++)
		if (enemy::list[i] != 0)
			(enemy::list[i])->put(-1, -1, 0);
}


/* value returned between 0 and 45	*/
