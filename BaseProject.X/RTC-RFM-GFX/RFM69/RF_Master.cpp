

#include "RFM69.h"
#include "RFM69_ATC.h"
#include "RF_Master.h"

#ifdef ENABLE_ATC
      RFM69_ATC radio;
#else
      RFM69 radio;
      radio.initialize(FREQUENCY,NODEID,NETWORKID);
#endif

void RF_Init()
{
    //Change PPS to needed MISO & MOSI Pins
    SDI1R = 0b0101; //RB9
    RPB10R = 0b0101; //RB10;
    
    ANSELBbits.ANSB9 = 0;
    
    SPI1BRG = 0x8;
    SPI1CONCLR = 0x10000300;
    SPI1CONSET = 0x8060;
    
    LATGbits.LATG1 = 1;
    TRISGbits.TRISG1 = 0;
    
    #ifdef ENABLE_ATC
      radio.initialize(FREQUENCY,NODEID,NETWORKID);
      radio.enableAutoPower(ATC_RSSI);
    #else
      RFM69 radio;
      radio.initialize(FREQUENCY,NODEID,NETWORKID);
    #endif
    
    radio.encrypt(ENCRYPTKEY);
  
    if (NODEID == 1) //target node Id, message as string or byte array, message length
    {
       radio.sendWithRetry(RECEIVER, "Hi", 2);
    }    
       
}

void RF_SEND(uint8_t node, char * message, uint8_t attempts)
{
    radio.sendWithRetry(node, message, attempts);
}

void RX_Handler(void)
{
    //Data exists, now do something with it
    if (radio.DATALEN==2 && radio.DATA[0]=='H' && radio.DATA[1]=='i')
    {
        //Toggle LED to show good data
        LED_LAT = 1;
    }
}

//Radio ISR
void __ISR_AT_VECTOR(_CHANGE_NOTICE_G_VECTOR, IPL7SOFT) RXISR(void)
{
    //Calls Interrupt Handler, and sleeps radio once data recieved
    LED_LAT = 0; //Turn off LED since data is coming in
    radio.receiveDone();
    RX_Handler();
}