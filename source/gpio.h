#define	GPIO_BASE_ADDRESS	0x20200000
#define GPSET0			0x2020001c
#define GPCLEAR0		0x20200028
#define	GPPUD			0x20200094
#define	GPREN0			0x2020004c
#define	GPLEV0			0x20200034

#define GPINPUT   0
#define GPOUTPUT  1
#define GPALT0    0b100
#define GPALT1    0b101
#define GPALT2    0b110
#define GPALT3    0b111
#define GPALT4    0b011
#define GPALT5    0b010
#define GPNO      0
#define GPDOWN    1
#define GPUP      2
#define GPRISE    0
#define GPFALL    1
#define GPHIGH    2
#define GPLOW     3
#define GPARISE   4
#define GPAFALL   5
#define GPENABLE	1
#define	GPDISABLE	0

#define	LONG_INTERVAL	1500000
#define	SHORT_INTERVAL	250000

void setGpioFunct(int, int);
void setPull(int pinNumber, int pull);
void flashValue(int value, char length);
void wait(unsigned int interval); //us = 10^(-6) s
void pushCounter(unsigned int interval);
int pollCounter();

extern volatile unsigned int* gpioBase;
extern volatile unsigned int* gpset0;
extern volatile unsigned int* gpclear0;
extern volatile unsigned int* gppud;
extern volatile unsigned int* gpren0;
extern volatile unsigned int* gplev0;

static inline void setPin(int pinNumber){
	*(gpset0+(pinNumber/32))=(1<<(pinNumber%32));
}

static inline void clearPin(int pinNumber){
        *(gpclear0+(pinNumber/32))=(1<<(pinNumber%32));
}

/*	enable is bool value,
	as for detect mode use: RISE, FALL, HIGH, LOW, ARISE, AFALL
*/
static inline void setDetect(int pinNumber, short int enable, short int mode){
	unsigned int tmp = *(gpren0+(mode*3)+(pinNumber/32));
	tmp |= enable<<(pinNumber%32);
	*(gpren0+(mode*3)+(pinNumber/32))=tmp;
}

static inline int getPinLevel(int pinNumber){
	return (((*(gplev0+(pinNumber/32)))>>pinNumber%32)&1);
}

static inline void flash(void){
	setGpioFunct(16, GPOUTPUT);
	clearPin(16); wait(1000000); setPin(16); wait(1000000);
}

static inline void flashTime(unsigned int time){
        setGpioFunct(16, GPOUTPUT);
        clearPin(16); wait(time); setPin(16); wait(750000);
}




