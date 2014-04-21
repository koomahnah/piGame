#include "gpio.h"
#include "keyboard.h"
#include "object.h"

volatile unsigned int kBuffer = 16;
short int pinToGpio[9] = { 12, 22, 23, 24, 14, 15, 17, 18, 27 };

/* pin 24 was pin 4 primarily here, but it has pull up instead of down	*/
char keyboardArray[16] = { '1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', 42, '0', 35, 'D' };

void (*extKIrqHandler)(void) = 0;
void keyboardInit()
{
	extKIrqHandler = 0;
	kBuffer = 16;
	int x;
	/*
	 * set 5-8 lines to input with pull down,
	 * detecting rising edge
	 */
	for (x = 5; x <= 8; x++) {
		setGpioFunct(pinToGpio[x], GPINPUT);
		setPull(pinToGpio[x], GPDOWN);
		setDetect(pinToGpio[x], GPENABLE, GPHIGH);
	}
	/*
	 * turn 1-4 lines high to allow interrupt
	 * to be generated
	 */
	for (x = 1; x <= 4; x++) {
		setGpioFunct(pinToGpio[x], GPOUTPUT);
		setPin(pinToGpio[x]);
		setPull(pinToGpio[x], GPDOWN);
	}
	/*
	 * sets interrupt controller
	 */
	*irqEnable2 = ((1 << 20) | (1 << 19) | (1 << 18) | (1 << 17));
	/*
	 * takes care of CPSR
	 */
	irqEnable();
	return;
}

void kIrqHandler()
{
	unsigned int tmp;
	int a, x;
	for (a = 1; a <= 4; a++) {
		/*
		 * only one line from 1-4 should be high
		 * at one moment                                
		 */

		for (x = 1; x <= 4; x++) {
			if (x != a) {
				setGpioFunct(pinToGpio[x], GPINPUT);
			}
		}
		setGpioFunct(pinToGpio[a], GPOUTPUT);
		setPin(pinToGpio[a]);

		waitCycles(30);
		/*
		 * for every output line going high, check
		 * which input line goes high; store in kBuffer 
		 */
		for (x = 5; x <= 8; x++) {
			if (getPinLevel(pinToGpio[x]) == 1) {
				kBuffer = (4 * (a - 1)) + (x - 5);
				//      very sorry - it's
				//      probably the best way
				goto exit;
			}
		}
		clearPin(pinToGpio[a]);
	}

 exit:	for (x = 1; x <= 4; x++) {
		setGpioFunct(pinToGpio[x], GPOUTPUT);
		setPin(pinToGpio[x]);
	}

	if(extKIrqHandler!=0) extKIrqHandler();
	return;
}
