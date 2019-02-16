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
#define NETWORKID     100
#define RECEIVER      2    //unique ID of the gateway/receiver
#define SENDER        1
#define NODEID        RECEIVER  //change to "SENDER" if this is the sender node
#define FREQUENCY     RF69_915MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define IS_RFM69HW_HCW //uncomment only for RFM69HW/HCW! Leave out if you have RFM69W/CW!
#define ENABLE_ATC      //comment out this line to disable AUTO TRANSMISSION CONTROL
#define ATC_RSSI -75

#define LED_TRIS    TRISHbits.TRISH2    // macro for direction register bit of the LED pin
#define LED_LAT     LATHbits.LATH2

void RF_Init();
void RX_Handler(void);
void RF_SEND(uint8_t node, char * message, uint8_t attempts);


#endif	/* RF_MASTER_H */

