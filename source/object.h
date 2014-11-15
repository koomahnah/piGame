#define	BACKGROUND_COLOUR	13,2,19
#define	BRICK_COLOUR		10,56,19

struct object{
	unsigned short int x;
	unsigned short int y;
	unsigned short int width;
	unsigned short int height;
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

void objectDraw(struct object *a, unsigned short int x, unsigned short int y);
void objectMoveUp(struct object *a);
void objectMoveDown(struct object *a);
void objectMoveRight(struct object *a);
void objectMoveLeft(struct object *a);
int detectCollision(struct object *a, struct object *b);

