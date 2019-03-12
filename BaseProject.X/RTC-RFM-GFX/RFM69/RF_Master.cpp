

#include <string.h>
#include "RFM69.h"
#include "RFM69_ATC.h"
#include "RF_Master.h"

RFM69_ATC radio;

#define DATA_STRUCT_SIZE 4

typedef struct{
    // uint8_t level ?
    char res_fill_percent[DATA_STRUCT_SIZE];
    char src_level[DATA_STRUCT_SIZE];
    uint8_t newdata;
}GUI_LEVELS_T;

typedef struct{
    char surface_height[DATA_STRUCT_SIZE];
    uint8_t newdata;
}GUI_SURFACE_T;

typedef struct{
    char flow_rate[DATA_STRUCT_SIZE];
    uint8_t newdata;
    // daily use, other things?
}GUI_FLOW_T;

typedef struct{
    char todays_flow[DATA_STRUCT_SIZE];
    char week_flow[DATA_STRUCT_SIZE];
    char month_flow[DATA_STRUCT_SIZE];
    char year_flow[DATA_STRUCT_SIZE];
    uint8_t newdata;
}GUI_STATS_T;
    
    typedef struct{
        uint8_t newdata;
        char presence[DATA_STRUCT_SIZE];
    }GUI_CP_T;
    


//Extern Global Vars
extern GUI_FLOW_T flow_data;
extern GUI_LEVELS_T level_data;
extern GUI_STATS_T stats_data;
extern GUI_CP_T cp_data;
extern GUI_SURFACE_T surface_data;

extern uint8_t PUMP_STATE;
extern uint8_t TANK_HEIGHT;
extern uint8_t TANK_DIAMETER;
extern uint8_t STAFF_HEIGHT;
extern uint8_t UNITS;
extern uint8_t BUZZER_STATE;
extern uint8_t PUMP_DELAY;

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
/*
 * Recieves Radio data and parses
 * S = second heartbeat
 * TL = Tank level and percent
 * FR = Flow rate 
 * CP = Conductive Presence
 * ES = E-Stop (pump has e stopped)
 * ST = Stats
 */


bool RX_Handler(void)
{
    int count = 0;
    
    if(radio.receiveDone())
    {
       char radiobuff[70];
        int length = radio.DATALEN;
        
       // snprintf(radiobuff, length,"%d",radio.DATA);
            
        //Got Some Data
        if(radio.DATA[0] == 'H')
        {
            LED_LAT = !LED_LAT;
        }
        else if(radio.DATA[0] == 'T' && radio.DATA[1] == 'L')
        {
            /*
            float res_fill_percent;
            float src_level;
            uint8_t newdata;
             */
            //Got Tank Level Data, Load it up
            
            while(count != 7)
            {
                radiobuff[count] = radio.DATA[count];
                count++;
            }

            level_data.newdata = 1;
            level_data.res_fill_percent[0] = radiobuff[2];
            level_data.res_fill_percent[1] = radiobuff[3];//4 bytes
            level_data.src_level[0] = radiobuff[4]; //4 bytes
            level_data.src_level[1] = radiobuff[5];
        }   
        else if(radio.DATA[0] == 'F' && radio.DATA[1] == 'R')
        {
            //Flow Rate
            flow_data.newdata = 1;
            //Loop until null terminated
            while(radio.DATA[count+2] != '\0' && count <= DATA_STRUCT_SIZE)
            {
                flow_data.flow_rate[count] = radio.DATA[count+2];
                count++;
            }
        }
        else if(radio.DATA[0] == 'C' && radio.DATA[1] == 'P')
        {
            //Conductive Presence True False
            cp_data.newdata = 1;
            cp_data.presence[0] = radio.DATA[2];
        }
        
        else if(radio.DATA[0] == 'E' && radio.DATA[1] == 'S')
        {
            //Pump E-Stop, Display Error Message & Sound
        }
        
        
        else if(radio.DATA[0] == 'S' && radio.DATA[1] == 'H')
        {
            //Surface Water Height
            surface_data.newdata = 1;
            //Loop until null terminated
            while(radio.DATA[count+2] != '\0' && count <= DATA_STRUCT_SIZE)
            {
                surface_data.surface_height[count] = radio.DATA[count+2];
            }
        }
        
        else if(radio.DATA[0] == 'P')
        {
            if(radio.DATA[1] == 'S')
            {
                PUMP_STATE = (radio.DATA[2]);
            }
            else if (radio.DATA[1] == 'D')
            {
                PUMP_DELAY = (radio.DATA[2]);
            }
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
