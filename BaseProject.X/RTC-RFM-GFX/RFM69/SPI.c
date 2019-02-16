

#include <xc.h>
#include <sys/attribs.h>
#include "SPI.h"

int millis()
{
    return _CP0_GET_COUNT();
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