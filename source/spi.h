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

static inline void spiStartTransfer(void);
static inline void spiWaitTilDone(void);
static inline void spiStopTransfer(void);
static inline void spiClearFIFO(void);
static inline unsigned char spiDataIO(unsigned char);
static inline void spiDataSend(unsigned char);
static inline unsigned char spiDataReceive(void);

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


