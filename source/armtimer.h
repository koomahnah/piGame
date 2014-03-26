#define	TIMERCS		0x20003000
#define IRQENABLE1	0x2000B210

void timerInit();
void timerSetMatch(unsigned int value);
void tIrqHandler(void);
void basicTIrqHandler(void);

extern void (*extTIrqHandler)(void);
extern volatile unsigned int *timercs;
extern volatile unsigned int *irqEnable1;
extern unsigned int timerMatch;
