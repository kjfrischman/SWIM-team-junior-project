/* 
 * File:   RF_Master.h
 * Author: kjfrischman
 *
 * Created on February 13, 2019, 2:49 PM
 */

#ifndef RF_MASTER_H
#define	RF_MASTER_H

//***********************************
//**** IMPORTANT RADIO SETTINGS  ****
//***********************************
#define NETWORKID           100  //Do Not Change
#define CONTROL_PANEL_NODE  3    //unique ID of the gateway/receiver
#define SENSOR_UNIT_NODE    2    //ID of the sending Unit
#define NODEID              CONTROL_PANEL_NODE  //change to "SENDER" if this is the sender node
#define FREQUENCY           RF69_915MHZ
#define ENCRYPTKEY          "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define IS_RFM69HW_HCW      //Hardware Revision
#define ENABLE_ATC          //comment this line to disable AUTO TRANSMISSION CONTROL
#define ATC_RSSI            -75  //Reciever Sensitivity (dB)

#define LED_TRIS    TRISHbits.TRISH2 
#define LED_LAT     LATHbits.LATH2

extern "C"
{
    void RF_SEND(uint8_t node, char * message, uint8_t attempts);
};

void RF_Init();
bool RX_Handler(void);
uint8_t DataParse (void);

uint8_t * RX_Get_Raw(void);

#endif	/* RF_MASTER_H */

