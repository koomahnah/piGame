#ifndef _GPIO_H
#define _GPIO_H
#include "gpio.h"
#endif
#ifndef _SPI_H
#define _SPI_H
#include "spi.h"
#endif

/*	LCD START CODES     */
#define INDEX_WRITE     0x70	//0b01110000
#define REG_WRITE       0x72	//0b01110010
#define REG_READ        0x73	//0b01110011

/*      FONT SETTINGS   */
#define HEIGHT  	16	//32
#define WIDTH   	16	//24
#define OFFSET  	4
#define FONT_NAME	Sinclair_Inverted_M

struct colour{
        unsigned char red;
        unsigned char green;
        unsigned char blue;
};

extern struct colour background;
extern struct colour font;

extern void (*lcdExtEntryFunct)(void);
extern void (*lcdExtExitFunct)(void);
/*
	These will be called
	on entry or exit of critical segments
	of code.
*/
extern volatile int intrTrace;
/*	If it gets set to 1 while some functions
	work, all procedure will be repeated.
*/

extern unsigned char Sinclair_Inverted_M[3044];

void lcdInit(int);
void lcdRegWrite(unsigned char, unsigned short int);
unsigned short int lcdRegRead(unsigned char index);
void lcdDrawChar(unsigned short int x, unsigned short int y, char character);
void lcdDrawCharC(unsigned short int x, unsigned short int y, char character, struct colour *b, struct colour *f); 
void lcdOpenGRAM(void);
void lcdCloseGRAM(void);
void lcdSetWindow(unsigned short int hsa, unsigned short int hea, unsigned short int vsa, unsigned short int vea);
void lcdPrint(int x, int y, const char *str);
void lcdFillWindow(unsigned short int hsa, unsigned short int hea, unsigned short int vsa, unsigned short int vea, unsigned char red, unsigned char green, unsigned char blue);
void lcdPixelsDraw(unsigned int amount, unsigned char red, unsigned char green, unsigned char blue);
void dummy(void);
static inline void lcdDisplayON(void);
static inline void lcdSetCursor(unsigned short int, unsigned short int);
static inline void lcdPixelsWrite(unsigned char red, unsigned char green, unsigned char blue);
static inline void lcdSetBackgroundColour(unsigned char red, unsigned char green, unsigned char blue);
static inline void lcdSetFontColour(unsigned char red, unsigned char green, unsigned char blue);

static inline void lcdDisplayON(void){
	unsigned int tmp = lcdRegRead(0x07);
	tmp |= 0b11;
	lcdRegWrite(0x07, (unsigned short int)tmp);
}

static inline void lcdSetCursor(unsigned short int x, unsigned short int y){
	do{
	intrTrace = 0; lcdRegWrite(0x20, x); lcdRegWrite(0x21, y);
	} while (intrTrace == 1);
}

/*	it is inlined because it's always invoked in a loop	*/
static inline void lcdPixelsWrite(unsigned char red, unsigned char green, unsigned char blue){
	red=red>>1; blue=blue>>1;
	unsigned char upper = (red<<3) + (green>>3);
        unsigned char lower = (green<<5) + blue;
	spiDataIO(upper);
	spiDataIO(lower);
	spiWaitTilDone();
}

static inline void lcdSetBackgroundColour(unsigned char red, unsigned char green, unsigned char blue){
	background.red = red;
	background.green=green;
	background.blue=blue;
}
static inline void lcdSetFontColour(unsigned char red, unsigned char green, unsigned char blue){
	font.red=red;
	font.green=green;
	font.blue=blue;
}



