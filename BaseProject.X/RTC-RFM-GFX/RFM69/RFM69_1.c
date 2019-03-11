
#include "../../RFM69_1.h"

//**********************************************************/
void RFM69_Config(void)	// FlagSTR _SecTime
{
// {0x074e, 0x08c0, 0x0900},	//315MHz
// {0x076c, 0x0880, 0x0900},	//434MHz
// {0x07d9, 0x0800, 0x0900},	//868MHz
SPIWrite(0x07e4);
SPIWrite(0x08c0);
SPIWrite(0x0900);		 //915MHz
SPIWrite(0x0200);        //RegDataModul 	FSK Packet
SPIWrite(0x0502);        //RegFdevMsb		241*61Hz = 35KHz	
SPIWrite(0x0641);        //RegFdevLsb
SPIWrite(0x0334);        //RegBitrateMsb	32MHz/0x3410 = 2.4kpbs
SPIWrite(0x0410);        //RegBitrateLsb
SPIWrite(0x130F);        //RegOcp			Disable OCP
SPIWrite(0x1888);        //RegLNA           200R	
SPIWrite(0x1952);        //RegRxBw			RxBW  83KHz
SPIWrite(0x2C00);        //RegPreambleMsb	
SPIWrite(0x2D05);        //RegPreambleLsb	5Byte Preamble
SPIWrite(0x2E90);        //enable Sync.Word	2+1=3bytes		
SPIWrite(0x2617);		 //					RegDioMapping2
SPIWrite(0x29E4); 		 //					RegRssiThresh
SPIWrite(0x0D92);		 //  		 		RegListen1 listen end  	1001 0010
SPIWrite(0x2FAA);        //0xAA				SyncWord = aa2dd4
SPIWrite(0x302D);        //0x2D
SPIWrite(0x31D4);        //0xD4
SPIWrite(0x3700);        //RegPacketConfig1  Disable CRC-NRZ encode
SPIWrite(0x380A);        //RegPayloadLength  10 bytes for length & Fixed length
SPIWrite(0x3C95);        //RegFiFoThresh		95 -> 0	
SPIWrite(0x581B);        //RegTestLna        Normal sensitivity
SPIWrite(0x6F30);        //RegTestDAGC       Improved DAGC
SPIWrite(0x0104);        //Standby         //base parameters
}
//**********************************************************/
uint8_t RFM69_EntryRx(void)		// Set RFM69 to Rx_mode
{
RFM69_Config();          //config RFM69 base parameters
SPIWrite(0x119F);        //RegPaLevel Fifo In for Rx
SPIWrite(0x2544);        //DIO Mapping for Rx  (0100 01200)
SPIWrite(0x5A55);        //Normal and TRx
SPIWrite(0x5C70);        //Normal and TRx		
SPIWrite(0x0110);        //Entry to Rx
TimerTick = 3;
while (TimerTick)                               //wait for entry to Rx mode
  {
  if ((SPIRead(0x27) & 0xC0) == 0xC0)				//RX Ready, Mode Ready
   break;				
  } 
if (!TimerTick)	 
  return(0);                                        //over time for error
else
  return(1);                                        //now in Rx mode 
}
//**********************************************************/
uint8_t RFM69_EntryTx(void)	// Set RFM69 to Tx mode
{
RFM69_Config();           //config RFM69 base parameters
SPIWrite(0x2504);        // DIO mapping for TX
SPIWrite(0x1195);        //RegPaLevel 13dBm
SPIWrite(0x5A55);        //Normal and TRx 
SPIWrite(0x5C70);        //Normal and TRx	
SPIWrite(0x010C);        //Entry to Tx);	
TimerTick = 3;
while (TimerTick)      //wait for entry Tx                                                  
  {	      
  if((SPIRead(0x27)&0xA0)==0xA0)                    // TX ready, Mode ready
   break;				
  } 	 
if (!TimerTick)	 
  RFM69_EntryRx();                                  //over time for error				
else
  Mode = TXmode;
return(1);
}
//**********************************************************/
void RFM69_ClearFIFO(void) // Change to RxMode from StandbyMode, can clear FIFO buffer
{
SPIWrite(0x0104);      //Entry Standby Mode
SPIWrite(0x0110);      //Change to Rx Mode
}
//**********************************************************/
void RFM69_Sleep(void)	// Set RFM69 to sleep mode
{
 SPIWrite(0x0100);                                 
}
//**********************************************************/
void RFM69_Standby(void)	//Set RFM69 to Standby mode
{
 SPIWrite(0x0104);
}
//**********************************************************/
uint8_t RFM69_RxPacket(void)	// Check for receive one packet
{
 uint8_t i; 
 if ((IRQ) == 1)
	{
	for(i=0;i<10;i++)	
		RxData[i] = 0x00;				// Clear RX data buffer
	ReadFifo(0x00, RxData, 10);		// Read 10 bytes
	RFM69_ClearFIFO();					// Clear RFM69 FIFO
	return(1);
	}
 else
  	return(0);
}
//**********************************************************/

void SPICmd8bit(uint8_t WrPara)
{

}
//**********************************************************/
uint8_t SPIRead8bit(void)
{
uint8_t RdPara = 0;
uint8_t bitcnt;
select();
SPIWrite(0);
unselect();
 return(RdPara);
}
//**********************************************************/
uint8_t SPIRead(uint8_t adr)
{
    select();
 uint8_t tmp;	
 SPICmd8bit(adr);                                //Send address first
 tmp = SPIRead8bit();	
 unselect();
 return(tmp);
}
//**********************************************************/
long SPIWrite(long WrPara)								
{  
long Data_In = 0;      
 select();
 
 WrPara |= 0x8000;                                //MSB must be "1" for write 
 
 Data_In = SPIWrite(WrPara);
 
return(Data_In);
}         
//**********************************************************/
void ReadFifo(uint8_t adr, uint8_t *ptr, uint8_t length)
{
 uint8_t i;
 if(length<=1)                               //length must more than one
 	return;
 else
 	{
     select();
 	SPICmd8bit(adr);	
 	for(i=0;i<length;i++)
  		ptr[i] = SPIRead8bit();
	unselect();
	}
}

//**********************************************************/
void TransmitBuffer(uint8_t adr, uint8_t *ptr, uint8_t length)
{ 
 uint8_t i;
 if(length<=1)         //length must more than one
 	return;
 else	
 	{ 	
 	select();	
 	SPICmd8bit(adr|0x80);	
 	for(i=0;i<length;i++)
  		SPICmd8bit(ptr[i]);
	unselect();
	}
}

// select the RFM69 transceiver (save SPI settings, set CS low)
void select() {
    PORTGbits.RG1 = 0;
}

// unselect the RFM69 transceiver (set CS high, restore SPI settings)
void unselect() {
  PORTGbits.RG1 = 1;
}