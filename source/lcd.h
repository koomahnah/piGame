#define SPI_BASE_ADDRESS        0x20204000
/*
        SPI control bit places, thanks to ggkinuthia@bitbucket
*/

#define SPI_C_LEN_LONG          25
#define SPI_C_DMA_LEN           24
#define SPI_C_CSPOL2            23
#define SPI_C_CSPOL1            22
#define SPI_C_CSPOL0            21
#define SPI_C_RX                20
#define SPI_C_RXR               19
#define SPI_C_TXD               18
#define SPI_C_RXD               17
#define SPI_C_DONE              16
#define SPI_C_LEN               13
#define SPI_C_REN               12
#define SPI_C_ADCS              11
#define SPI_C_INTR              10
#define SPI_C_INTD               9
#define SPI_C_DMAEN              8
#define SPI_C_TA                 7
#define SPI_C_CSPOL              6
#define SPI_C_CLEAR_RX           5
#define SPI_C_CLEAR_TX           4
#define SPI_C_CPOL               3
#define SPI_C_CPHA               2
#define SPI_C_CS1                1
#define SPI_C_CS0                0

extern volatile unsigned int* spiControl;
extern volatile unsigned int* spiFIFO;
extern volatile unsigned int* spiClock;
extern volatile int irqTrace;

extern volatile unsigned int *irqEnable2;
extern volatile unsigned int *irqEnable1;
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
        unsigned char blue;
        unsigned char green;
};

extern struct colour background;
extern struct colour font;

extern unsigned char Sinclair_Inverted_M[3044];

void lcdInit(int);
void lcdRegWrite(unsigned char, unsigned short int);
unsigned short int lcdRegRead(unsigned char index);
void lcdDrawChar(unsigned short int x, unsigned short int y, char character);
void lcdOpenGRAM(void);
void lcdCloseGRAM(void);
void lcdSetWindow(unsigned short int hsa, unsigned short int hea, unsigned short int vsa, unsigned short int vea);
void lcdPrint(const char *str, int x, int y);
void irqDisableSec(void);
void irqEnableSec(void);
static inline void lcdDisplayON(void);
static inline void lcdSetCursor(unsigned short int, unsigned short int);
static inline void lcdPixelsWrite(unsigned char red, unsigned char green, unsigned char blue);
static inline void lcdSetBackgroundColour(unsigned char red, unsigned char green, unsigned char blue);
static inline void lcdSetFontColour(unsigned char red, unsigned char green, unsigned char blue);
static inline void spiStartTransfer(void);
static inline void spiWaitTilDone(void);
static inline void spiStopTransfer(void);
static inline void spiClearFIFO(void);
static inline unsigned char spiDataIO(unsigned char);
static inline void spiDataSend(unsigned char);
static inline unsigned char spiDataReceive(void);

static inline void lcdDisplayON(void){
	unsigned int tmp = lcdRegRead(0x07);
	tmp |= 0b11;
	lcdRegWrite(0x07, (unsigned short int)tmp);
}

static inline void lcdSetCursor(unsigned short int x, unsigned short int y){
	do{
	irqTrace = 0; lcdRegWrite(0x20, x); lcdRegWrite(0x21, y);
	} while (irqTrace == 1);
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


static inline void spiStartTransfer(void){
        unsigned int tmp = *spiControl;
        tmp |= (1<<SPI_C_TA);
        *spiControl = tmp;
}

static inline void spiWaitTilDone(void){
	while(!((*spiControl)&(1<<SPI_C_DONE))) continue;
}

static inline void spiStopTransfer(void){
	while(!((*spiControl)&(1<<SPI_C_DONE))) continue;
        unsigned int tmp = *spiControl;
        tmp &= ~(1<<SPI_C_TA);
        *spiControl = tmp;
}

static inline void spiClearFIFO(void){
	unsigned int tmp = *spiControl;                      // clear FIFO
        tmp |= (1<<SPI_C_CLEAR_RX | 1<<SPI_C_CLEAR_TX);
        *spiControl=tmp;
	return;
}

static inline unsigned char spiDataIO(unsigned char data){
	*spiFIFO=(unsigned int)data;
	return (unsigned char)(*spiFIFO);
}

static inline void spiDataSend(unsigned char data){
	*spiFIFO=(unsigned int)data;
}

static inline unsigned char spiDataReceive(void){
	return (unsigned char)(*spiFIFO);
}


