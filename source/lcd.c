#include "lcd.h"

volatile int intrTrace;
struct colour background = { 0, 0, 0 };
struct colour font = { 0b111111, 0b111111, 0b111111 };

void lcdInit(int clock)
{
	unsigned int tmp;
	intrTrace = 0;
	lcdExtEntryFunct = dummy;
	lcdExtExitFunct = dummy;
	setGpioFunct(8, ALT0);
	setGpioFunct(9, ALT0);
	setGpioFunct(10, ALT0);
	setGpioFunct(11, ALT0);
	setGpioFunct(25, OUTPUT);

	setPin(25);
	wait(10000);
	clearPin(25);
	wait(1500);
	setPin(25);

	spiClearFIFO();
/*	tmp = *spiControl;			// clear FIFO
	tmp |= (1<<SPI_C_CLEAR_RX | 1<<SPI_C_CLEAR_TX);
        *spiControl=tmp;
*/
	tmp = 0;
	tmp |= (1 << SPI_C_CPOL | 1 << SPI_C_CPHA);
	*spiControl = tmp;

	*spiClock = clock;
	wait(200000);

	lcdRegWrite(0x00, 0x0000);
	lcdRegWrite(0x01, 0x0);	/* Driver Output Contral */
	lcdRegWrite(0x02, (1 << 10));	/* LCD Driver Waveform Contral */
	lcdRegWrite(0x03, (1 << 15) | (1 << 14) | (1 << 5) | (1 << 12) | (1 << 4) | (1 << 3));	/* Set the scan moe */
	lcdRegWrite(0x04, 0x0000);	/* Scalling Contral */
	lcdRegWrite(0x08, 0x0202);	/* Display Contral 2 */
	lcdRegWrite(0x09, 0x0000);	/* Display Contral 3 */
	lcdRegWrite(0x0a, 0x0000);	/* Frame Cycle Contal */
	lcdRegWrite(0x0c, (1 << 1) | (1 << 0));	/* Extern Display Interface Contral 1 */
	lcdRegWrite(0x0d, 0x0000);	/* Frame Maker Position */
	lcdRegWrite(0x0f, 0x0000);	/* Extern Display Interface Contral 2 */
	wait(50000);
	lcdRegWrite(0x07, 0x0101);	/* Display Contral */
	wait(50000);
	lcdRegWrite(0x10, (1 << 12) | (0 << 8) | (1 << 7) | (1 << 6) | (0 << 4));	/* Power Control 1 */
	lcdRegWrite(0x11, 0x0007);	/* Power Control 2 */
	lcdRegWrite(0x12, (1 << 8) | (1 << 4) | (0 << 0));	/* Power Control 3 */
	lcdRegWrite(0x13, 0x0b00);	/* Power Control 4 */
	lcdRegWrite(0x29, 0x0000);	/* Power Control 7 */
	lcdRegWrite(0x2b, (1 << 4));

	lcdRegWrite(0x50, 0);	/* Set X Start */
	lcdRegWrite(0x51, 239);	/* Set X End */
	lcdRegWrite(0x52, 0);	/* Set Y Start */
	lcdRegWrite(0x53, 319);	/* Set Y End */
	wait(50000);

	lcdRegWrite(0x60, 0x2700);	/* Driver Output Control */
	lcdRegWrite(0x61, 0x0001);	/* Driver Output Control */
	lcdRegWrite(0x6a, 0x0000);	/* Vertical Srcoll Control */

	lcdRegWrite(0x80, 0x0000);	/* Display Position? Partial Display 1 */
	lcdRegWrite(0x81, 0x0000);	/* RAM Address Start? Partial Display 1 */
	lcdRegWrite(0x82, 0x0000);	/* RAM Address End-Partial Display 1 */
	lcdRegWrite(0x83, 0x0000);	/* Displsy Position? Partial Display 2 */
	lcdRegWrite(0x84, 0x0000);	/* RAM Address Start? Partial Display 2 */
	lcdRegWrite(0x85, 0x0000);	/* RAM Address End? Partial Display 2 */

	lcdRegWrite(0x90, (0 << 7) | (16 << 0));	/* Frame Cycle Contral */
	lcdRegWrite(0x92, 0x0000);	/* Panel Interface Contral 2 */
	lcdRegWrite(0x93, 0x0001);	/* Panel Interface Contral 3 */
	lcdRegWrite(0x95, 0x0110);	/* Frame Cycle Contral */
	lcdRegWrite(0x97, (0 << 8));
	lcdRegWrite(0x98, 0x0000);	/* Frame Cycle Contral */
	lcdRegWrite(0x07, 0b100110001);
	wait(500000);
}

void lcdRegWrite(unsigned char index, unsigned short int data)
{
	lcdExtEntryFunct();
	spiStartTransfer();
	spiDataIO(INDEX_WRITE);
	spiDataIO(0);
	spiDataIO(index);
	spiStopTransfer();
	/*
	 * while(1){
	 * if((*spiControl)&(1<<SPI_C_DONE)){
	 * tmp=*spiControl;
	 * tmp &= ~(1<<SPI_C_TA);
	 * break;}
	 * }
	 * *spiControl = tmp;
	 */
	spiStartTransfer();
	spiDataIO(REG_WRITE);
	spiDataIO((unsigned char)(data >> 8));
	spiDataIO((unsigned char)data);
	spiStopTransfer();
	lcdExtExitFunct();
}

unsigned short int lcdRegRead(unsigned char index)
{
	lcdExtEntryFunct();
	int i;
	spiStartTransfer();
	spiDataIO(INDEX_WRITE);
	spiDataIO(0);
	spiDataIO(index);
	spiStopTransfer();
	spiStartTransfer();
	spiDataIO(REG_READ);
	for (i = 0; i < 4; i++)
		spiDataIO(0);
	spiWaitTilDone();
	spiDataReceive();
	spiDataReceive();
	unsigned short int reg = (unsigned short int)(spiDataReceive() << 8);
	reg += spiDataReceive();
	spiClearFIFO();
	lcdExtExitFunct();
	return reg;
}
void lcdOpenGRAM(void){
	lcdExtEntryFunct();
	spiStartTransfer();
	spiDataIO(INDEX_WRITE);
	spiDataIO(0); spiDataIO(0x22);
	spiStopTransfer();
	spiStartTransfer();
	spiDataIO(REG_WRITE);
}

void lcdCloseGRAM(){
	spiStopTransfer();
	lcdExtExitFunct();
}

void lcdSetWindow(unsigned short int hsa, unsigned short int hea, unsigned short int vsa, unsigned short int vea){
	vsa&=0x1ff;
	vea&=0x1ff;
	do{
		intrTrace = 0;
		lcdRegWrite(0x50, (char)hsa);
		lcdRegWrite(0x51, (char)hea);
		lcdRegWrite(0x52, vsa);
		lcdRegWrite(0x53, vea);
	} while (intrTrace == 1);
}

void lcdPrint(const char *str, int x, int y){
	int i;
        for(i=0;str[i]!='\0';i++){
                if(y+(WIDTH*(i+1))<=320)
                        lcdDrawChar(x, y+(WIDTH*i), str[i]);
                else return;
        }
}

/*	draws consecutively given amount of pixels
	at given colour
	values from 0 to 0x3f, least significant bit
	of red and blue is cut				*/
void lcdPixelsDraw(unsigned int amount, unsigned char red, unsigned char green, unsigned char blue)
{
	lcdOpenGRAM();
	int i = 0;
	red = red >> 1;
	blue = blue >> 1;
	unsigned char upper = (red << 3) + (green >> 3);
	unsigned char lower = (green << 5) + blue;
	for (i = 0; i < amount; i++) {
		spiDataIO(upper);
		spiDataIO(lower);
		spiWaitTilDone();
	}
	lcdCloseGRAM();
}

void lcdDrawChar(unsigned short int x, unsigned short int y, char character)
{
	char tmp;
	int z = (int)character;
	z -= 32;
	if(z>94) z='A'-32;
	/*
	 * WIDTH*HEIGHT/8 - amount of bytes describing
	 * 1 character
	 * z=z*amount - bytes to skip
	 */
	z *= WIDTH * HEIGHT / 8;

	lcdExtEntryFunct();
	lcdSetWindow(x, x + HEIGHT - 1, y, y + WIDTH - 1);
	lcdSetCursor(x, y);
	lcdOpenGRAM();
	int i, k;
	for (i = 0; i < (WIDTH * HEIGHT / 8); i++) {
		tmp = FONT_NAME[OFFSET + z + i];
		for (k = 7; k >= 0; k--) {
			if (!(tmp & (1 << k)))
				lcdPixelsWrite(background.red, background.green, background.blue);
			else
				lcdPixelsWrite(font.red, font.green, font.blue);
		}
	}
	lcdCloseGRAM();
	lcdExtExitFunct();
}

void lcdFillWindow(unsigned short int hsa, unsigned short int hea, unsigned short int vsa, unsigned short int vea, unsigned char red, unsigned char green, unsigned char blue){
	do{
		intrTrace = 0;
		lcdSetWindow(hsa, hea, vsa, vea);
		lcdSetCursor(hsa, vsa);
		lcdPixelsDraw((hea - hsa + 1) * (vea - vsa + 1), red, green, blue);
	} while (intrTrace == 1);
}

/*	secondary IRQ disable (not in CPSR)	*/
void dummy(void){
	return;
}
