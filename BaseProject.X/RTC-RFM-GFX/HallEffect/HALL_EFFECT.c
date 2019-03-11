#include "HALL_EFFECT.h"


void HALL_EFFECT_INIT (uint8_t tshutoff) {
    
    HALL_EFFECT_FLOW_RATE = 0.0;
    HALL_EFFECT_DELTA = 0.0;
    
    CNCONDbits.ON = 0x1;
    CNCONDbits.w = 0x1;
    CNENDbits.CNIED0 = 0x1;
    CNPDDbits.CNPDD0 = 0x1; // Enable pulldown on RD0
    
    // Set PB3CLK
    SYSKEY = 0;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    PB3DIVSET = 0x107F; // Divide by 128 (656,250 Hz))
    SYSKEY = 0;
    
    INTCONbits.MVEC = 0x1;
        
    T1CON = 0; // Clear control register
    T1CONSET = 0x30; // Set prescale = 128:1, 656,250 / 128 = 5,126 Hz
    PR1 = ( 51 * tshutoff );


    // IPC1 T1CON IEC3 IEC0
    IEC0bits.INT0IE = 0x1;
    IFS0bits.INT0IF = 0x0;
    IPC0bits.INT0IP = 0x7;
    
    
    //IEC0SET = 0x10; // Enable timer 1 interrupt
    IFS0CLR = 0x10; // Clear existing flag
    IPC1bits.T1IP = 0x6;
    
    
    INTCONSET = 0x1; // Set external interrupt 0 to rising edge
    TRISDSET = 0x1; // RPD0 input
    
}


void __ISR_AT_VECTOR(_TIMER_1_VECTOR, IPL6SRS) PUMP_SHUTOFF_ISR(void){
    

    if (PUMP_STATUS()){
        PUMP_OFF();
        
        T1CONbits.ON = 0x0; // Turn T1 off
    }
    
    TMR1 = 0x0; // Reset count value
}

// Interrupts on the rising edge of the flow rate hall effect sensor.
// calculates the flow rate at each interrupt and stores ten of 
// the most recent flow rate values in float array HALL_DATA_ARRAY[10].
// External interrupt source 0 is on RPD0
void __ISR_AT_VECTOR(_EXTERNAL_0_VECTOR, IPL7SRS) HALL_EFFECT_ISR(void){
    static count = 0;
    T1CONCLR = 0x8000; // Turn timer 1 off
    TMR_DELTA = TMR1; // Store the timer delta
    TMR1 = 0; // Reset TMR1 count value
    
    // Find the hall effect period, then invert to get freq, then divide by 6.6
    // to get the flow rate in L/Min.
    HALL_DATA_ARRAY[HALL_EFFECT_INDEX] = 
    (float)( (1.0 / ((float)TMR_DELTA / 656250)) / 6.6 ); 
    
    // Find the delta by subtracting the previous rate from the current one
    HALL_EFFECT_DELTA = HALL_DATA_ARRAY[HALL_EFFECT_INDEX] - PREV_FLOW_RATE;
    PREV_FLOW_RATE = HALL_DATA_ARRAY[HALL_EFFECT_INDEX]; // Save current rate to use next time as previous
    
   
    if (HALL_EFFECT_INDEX < 0x9) HALL_EFFECT_INDEX++;
    else HALL_EFFECT_INDEX = 0x0;
    
    count++;
    
    IFS0bits.INT0IF = 0x0;
    T1CONSET = 0x8000; // Turn timer 1 on
}

// Returns the flow rate detected by the hall effect sensor.
// Passing '0' to the function it to return the most
// recent rate recorded, while passing a non-zero
// value will yield the average of the most recent 10 readings.
float HALL_EFFECT_GET_RATE (uint8_t average){
    static count = 0;
    float retVal = 0.0;
    uint8_t i = 0x0;
    
    if (average) {
        
        while(i < 0xA){
            retVal += HALL_DATA_ARRAY[i];
            i++;
        }
        
        retVal /= 10.0;
    }
    else {
        retVal = HALL_DATA_ARRAY[HALL_EFFECT_INDEX];
    }
    count++;
    return(retVal);    
}