#define IRQ_ENABLE1      0x2000B210
#define IRQ_ENABLE2     0x2000B214

extern void irqEnable(void);
extern void irqDisable(void);
void irqDisableSec(void);
void irqEnableSec(void);
extern volatile unsigned int *irqEnable2;
extern volatile unsigned int *irqEnable1;
extern volatile int irqLock;
