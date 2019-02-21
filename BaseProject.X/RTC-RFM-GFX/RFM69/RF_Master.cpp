

#include "RFM69.h"
#include "RFM69_ATC.h"
#include "RF_Master.h"

#ifdef ENABLE_ATC
      RFM69_ATC radio;
#else
      RFM69 radio;
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
    
    LED_TRIS = 0;
    
    #ifdef ENABLE_ATC
      radio.initialize(FREQUENCY,NODEID,NETWORKID);
      radio.enableAutoPower(ATC_RSSI);
    #else
      radio.initialize(FREQUENCY,NODEID,NETWORKID);
    #endif  
    
}

void RF_SEND(uint8_t node, char * message, uint8_t attempts)
{
    radio.sendWithRetry(node, message, attempts);
}

void RX_Handler(void)
{
    //Handle Getting data and ACK
    radio.receiveDone();
    int data = radio.DATA[0];
    int length = radio.DATALEN;
    //Data exists, now do something with it
    if (radio.DATALEN > 0 && radio.DATA[0] == 'S')
    {
        //Toggle LED
        LED_LAT = !LED_LAT;
    }
    
    if(radio.ACKRequested()) radio.sendACK();
    
}

uint8_t * RX_Get_Raw(void)
{
    //Return last data, whatever it was
    return radio.DATA;
}

void __ISR_AT_VECTOR(_CHANGE_NOTICE_E_VECTOR, IPL7SRS) RX_IRQ() { 
  radio.haveData(true);
  IFS3bits.CNEIF = 0;
  RX_Handler();
}