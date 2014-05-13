#define	TIMERCS		0x20003000

void timerInit();
void timerSetMatch(unsigned int value);
void timerOneShot(unsigned int value, void (*pFunct)(void));
void (*__timerPFunct)(void);
void tCircHandler();
void tOneShotHandler();
extern void (*tIrqHandler)(void);

extern void (*extTIrqHandler)(void);
extern volatile unsigned int *timercs;
