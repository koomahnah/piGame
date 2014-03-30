#include "spi.h"

volatile unsigned int *spiControl = (unsigned int *)SPI_BASE_ADDRESS;
volatile unsigned int *spiFIFO = (unsigned int *)SPI_BASE_ADDRESS + 1;
volatile unsigned int *spiClock = (unsigned int *)SPI_BASE_ADDRESS + 2;


