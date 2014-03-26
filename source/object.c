#include "lcd.h"
#include "object.h"

void objectDraw(struct object *a, unsigned short int x, unsigned short int y)
{
	lcdSetWindow(x, x + a->height - 1, y, y + a->width - 1);
	lcdSetCursor(x, y);
	lcdPixelsDraw(a->width * a->height, a->red, a->green, a->blue);
	a->x = x;
	a->y = y;
}

void objectMoveUp(struct object *a)
{
	lcdSetWindow(a->x - 1, a->x - 1, a->y, a->y + a->width - 1);
	lcdSetCursor(a->x - 1, a->y);
	lcdPixelsDraw(a->width, a->red, a->green, a->blue);
	lcdSetWindow(a->x + a->height - 1, a->x + a->height - 1, a->y, a->y + a->width - 1);
	lcdSetCursor(a->x + a->height - 1, a->y);
	lcdPixelsDraw(a->width, BACKGROUND_COLOUR);
	a->x--;
}

void objectMoveDown(struct object *a)
{
	lcdSetWindow(a->x, a->x, a->y, a->y + a->width - 1);
	lcdSetCursor(a->x, a->y);
	lcdPixelsDraw(a->width, BACKGROUND_COLOUR);
	lcdSetWindow(a->x + a->height, a->x + a->height, a->y, a->y + a->width - 1);
	lcdSetCursor(a->x + a->height, a->y);
	lcdPixelsDraw(a->width, a->red, a->green, a->blue);
	a->x++;
}

void objectMoveRight(struct object *a)
{
	lcdSetWindow(a->x, a->x + a->height - 1, a->y, a->y);
	lcdSetCursor(a->x, a->y);
	lcdPixelsDraw(a->height, BACKGROUND_COLOUR);
	lcdSetWindow(a->x, a->x + a->height - 1, a->y + a->width, a->y + a->width);
	lcdSetCursor(a->x, a->y + a->width);
	lcdPixelsDraw(a->height, a->red, a->green, a->blue);
	a->y++;
}

void objectMoveLeft(struct object *a)
{
	lcdSetWindow(a->x, a->x + a->height - 1, a->y + a->width - 1, a->y + a->width - 1);
	lcdSetCursor(a->x, a->y + a->width - 1);
	lcdPixelsDraw(a->height, BACKGROUND_COLOUR);
	lcdSetWindow(a->x, a->x + a->height - 1, a->y - 1, a->y - 1);
	lcdSetCursor(a->x, a->y - 1);
	lcdPixelsDraw(a->height, a->red, a->green, a->blue);
	a->y--;
}

/*	detects "square" collisions	*/
int detectCollision(struct object *a, struct object *b){
	short int xDistance = b->x - a->x + (b->height - a->height)/2;
	if (xDistance < 0) xDistance = -xDistance;
	if (xDistance >= (a->height + b->height)/2) return 0;
	short int yDistance = b->y - a->y + (b->width - a->width)/2;
	if (yDistance < 0) yDistance = -yDistance;
	if (yDistance >= (a->width + b->width)/2) return 0;
	return 1;
}
