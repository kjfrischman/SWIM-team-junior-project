
#include <xc.h>
#include <sys/attribs.h>
#include <string.h>
#include "RFM69.h"
#include "RFM69_ATC.h"
#include "RF_Master.h"
#include "../HallEffect/HALL_EFFECT.h"
#include "../HallEffect/PUMP.h"

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
    if (attempts == 0) attempts = 1;
    asm("di");
    CNCONEbits.ON = 0;
    //sendWithRetry(uint8_t toAddress, const void* buffer, uint8_t bufferSize, uint8_t retries, uint8_t retryWaitTime)
    int size = strlen(message)+1;
    radio.sendWithRetry(node, message, size, attempts, 40);
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
        if(radio.DATA[0] == 'H')
        {
            LED_LAT = !LED_LAT;
        }
        else if(radio.DATA[0] == 'P')
        {
            if (radio.DATA[1] == 'T')
            {
                //Set pump state
                PUMP_TOGGLE(radio.DATA[2]);
            }
            else if(radio.DATA[1] == 'D')
            {
                //Delay for the pump
            }
            
        }
        else if(radio.DATA[0] == 'T')
        {
            if(radio.DATA[1] == 'H')
            {
                //Tank Height
            }
            else if(radio.DATA[1] == 'D')
            {
                //Tank Diameter
            }
        }
        else if (radio.DATA[0] == 'S' && radio.DATA[1] == 'H')
        {
            //Surface Height
        }
        
        if(radio.ACKRequested())
        {
            radio.sendACK();
        }
    }
    
    return true;
}

uint8_t DataParse (void)
{
	uint8_t toReturn = 0;
	uint8_t count = 1;
	//Loop & Shift Data together
	while (radio.DATA[count] != NULL && count < 8)
	{
		//Shift Bits and create int
		toReturn = radio.DATA[count];
		toReturn << 1;
	}	
	
	return toReturn; 
}




void __ISR_AT_VECTOR(_CHANGE_NOTICE_E_VECTOR, IPL7SRS) RX_IRQ() { 
  IFS3bits.CNEIF = 0;
  if(PORTEbits.RE0 == 1)radio.haveData(true);
  //Do Something?
  RX_Handler();
}

//Radio Transmit Setup
void RF_Data_Init(void)
{
    //Setup Timer 5
    T5CONbits.TCKPS = 6;
    //1025
    PR5 = 2050;
    
    IEC0bits.T5IE = 1;
    IFS0bits.T5IF = 0;
    IPC6bits.T5IP = 5;
    
    //100ms Delay
    T5CONbits.ON = 1;
}

//Sensor Data Transmit Function
void __ISR_AT_VECTOR (_TIMER_5_VECTOR, IPL5SRS)RF_Data_Prep(void)
{
    static sensor_list sensor = Flow;
    char dataBuffer[50];
    static int cycles = 0;
    
    //Turn off Timer
    T5CONbits.ON = 0;
    IFS0bits.T5IF = 0;
    //Clear Val
    TMR5 = 0;
    
    switch (sensor)
    {
        case Flow:
        {
            float hallRate = HALL_EFFECT_GET_RATE(0);
            snprintf(dataBuffer, 50,"FR%02.2f", hallRate);
            sensor = Conductive;
            break;
        }
        
        case Conductive:
        {
            //snprintf(dataBuffer, 50, "CP%1d", );
            sensor = Tank;
            break;
        }
        
        case Tank:
        {
            //snprintf(dataBuffer, 50,"TL%02.2f%02.2f", , );
            sensor = Height;
            break;
        }
        
        case Height:
        {
            //snprintf(dataBuffer, 50, "SH%02.2f",);
            sensor = Heart;
            break;
        }
        
        case Heart:
        {
            if(cycles >= 5)
            {
                dataBuffer[0] = 'H';
                dataBuffer[1] = '\0';
                cycles = 0;
            }
            
            sensor = Pump;
            
            break;
        }
        
        case Pump:
        {
            snprintf(dataBuffer, 50, "PS");
            sensor = Delay;
        }
        
        case Delay:
        {
            snprintf(dataBuffer, 50, "PD");
        }
        
        default:
        {
            //IDK how we got here, but reset to a known state
            dataBuffer[0] = '\0';
            sensor = Flow;
            break;
        }
           
    }
    
    RF_SEND(3, dataBuffer, 2);
    cycles++; 
    
    //Start Timer Again
    T5CONbits.ON = 1;
}