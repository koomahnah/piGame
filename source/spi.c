#include "spi.h"
#include "gpio.h"

volatile unsigned int *spiControl = (unsigned int *)SPI_BASE_ADDRESS;
volatile unsigned int *spiFIFO = (unsigned int *)SPI_BASE_ADDRESS + 1;
volatile unsigned int *spiClock = (unsigned int *)SPI_BASE_ADDRESS + 2;

void spiInit(int cpol, int cpha, int clock){
	unsigned int tmp;
	setGpioFunct(8, GPALT0);
	setGpioFunct(9, GPALT0);
	setGpioFunct(10, GPALT0);
	setGpioFunct(11, GPALT0);
	spiClearFIFO();
	tmp = 0;
	tmp |= (cpol << SPI_C_CPOL | cpha << SPI_C_CPHA);
	*spiControl = tmp;
	*spiClock = clock;
}
