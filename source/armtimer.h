#define	TIMERCS		0x20003000

void timerInit();
void timerSetMatch(unsigned int value);
void tIrqHandler(void);

extern void (*extTIrqHandler)(void);
extern volatile unsigned int *timercs;
