#define IRQ_ENABLE2     0x2000B214

extern void irqEnable();
extern void irqDisable();
void keyboardInit();
void kIrqHandler();

extern void (*extKIrqHandler)(void);
extern volatile unsigned int kBuffer;
extern volatile unsigned int *irqEnable2;
