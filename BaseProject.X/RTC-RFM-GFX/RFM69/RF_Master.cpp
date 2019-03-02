

#include "RFM69.h"
#include "RFM69_ATC.h"
#include "RF_Master.h"

RFM69_ATC radio;

void RF_Init()
{
    //Change PPS to needed MISO & MOSI Pins
    SDI1R = 0b0101; //RB9
    RPB10R = 0b0101; //RB10;
    
    ANSELBbits.ANSB9 = 0;
    
    SPI1BRG = 0xF;
    SPI1CONCLR = 0x10000300;
    SPI1CONSET = 0x8060;
    
    LATGbits.LATG1 = 1;
    TRISGbits.TRISG1 = 0;
    
    LED_TRIS = 0;
    
    //Turn ON LED to say its doing stuff
    LED_LAT = 1;
     
    radio.initialize(FREQUENCY,NODEID,NETWORKID);
    radio.setHighPower();
    radio.enableAutoPower(ATC_RSSI);
    radio.promiscuous(false);
    
    
    //Setup Radio
    
    LED_LAT = 0;
}


void RF_SEND(uint8_t node, char * message, uint8_t attempts)
{
    //Send Radio Packet
    asm("di");
    CNCONEbits.ON = 0;
    radio.send(node, message, attempts);
    radio.sleep();
    IFS3bits.CNEIF = 0;
    CNCONEbits.ON = 1;
    asm("ei");
    //Check for data after send
    radio.haveData(true);
    if (!(RX_Handler())) RX_Handler();
    
} 

bool RX_Handler(void)
{
   
    if(radio.receiveDone())
    {
        //Got Some Data
        if(radio.DATALEN > 0 && radio.DATA[0] == 'S')
        {
            LED_LAT = !LED_LAT;
        }
        
        if(radio.ACKRequested())
        {
            radio.sendACK();
        }
        return true;
    }
    else
    {
        return false;
    }
}


void __ISR_AT_VECTOR(_CHANGE_NOTICE_E_VECTOR, IPL7SRS) RX_IRQ() { 
  IFS3bits.CNEIF = 0;
  if(PORTEbits.RE0 == 1)radio.haveData(true);
  //Do Something?
  RX_Handler();
}
