

#include <xc.h>
#include <sys/attribs.h>
#include "SPI.h"

#define PBCLK 40000000 // peripheral bus clock
#define dTime_ms PBCLK/2000

int millis()
{
    return (_CP0_GET_COUNT() * dTime_ms);
}

int SPI_Transfer(int data)
{
    int toReturn = 1;
    SPI1BUF = data;
    toReturn = SPI1BUF;
    while(SPI1STATbits.SPIBUSY);
    toReturn = SPI1BUF;
    return toReturn;
}

