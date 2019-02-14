

#include <xc.h>
#include <sys/attribs.h>
#include "SPI.h"

int millis()
{
    return _CP0_GET_COUNT();
}

int SPI_Transfer(int data)
{
    SPI1BUF = data;
    while(SPI1STATbits.SPIBUSY);
    return SPI1BUF;
}