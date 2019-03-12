#include <xc.h>
#include <sys/attribs.h>
#include "Debounce.h"

BUTTON_STATE_T BUTTON_STATES[NUM_BUTTONS];
uint8_t STATE_INDEX = 0;


void __ISR_AT_VECTOR(19, IPL5SRS) 
DEBOUNCE_ISR(void){
    
    T4CONbits.ON = 0x0; // turn timer 4 off
    IFS0bits.T4IF = 0x0; // clear interrupt flag
    TMR4 = 0x0; // Reset count value
    
    STATE_INDEX = 0x0;  //start at state 0
    uint8_t BUTTON_PRESSED = 0;   //button not pressed
    
    
    while (STATE_INDEX < NUM_BUTTONS){ //if a button is pressed
        
        switch(STATE_INDEX){
            case 0: BUTTON_PRESSED = PORTAbits.RA4;  //check the first button
            break;
            case 1: BUTTON_PRESSED = PORTAbits.RA5;  //check the second button
            break;
            case 2: BUTTON_PRESSED = PORTAbits.RA6;  //third
            break;
            case 3: BUTTON_PRESSED = PORTAbits.RA7;  //fourth
            break;
            case 4: BUTTON_PRESSED = PORTAbits.RA9;  //fifth
        }
        
        switch(BUTTON_STATES[STATE_INDEX].state){
            case UNPRESSED:
                switch(STATE_INDEX){
                    case 0: UP_BUTTON = 0x0; break;
                    case 1: DOWN_BUTTON = 0x0; break;
                    case 2: LEFT_BUTTON = 0x0; break;
                    case 3: RIGHT_BUTTON = 0x0; break;
                    case 4: ENTER_BUTTON = 0x0; break;
                }
                if (BUTTON_PRESSED) BUTTON_STATES[STATE_INDEX].state = DEBOUNCE;  //if it really is pressed check
                break;
            case DEBOUNCE:
                if (BUTTON_PRESSED){  //if it is pressed go to the pressed state
                    BUTTON_STATES[STATE_INDEX].state = PRESSED;
                }
                else {
                    BUTTON_STATES[STATE_INDEX].state = UNPRESSED; //if not pressed return to the unpressed state                   
                }
                break;
            case PRESSED:
                switch(STATE_INDEX){
                    case 0: UP_BUTTON = 0x1; break;
                    case 1: DOWN_BUTTON = 0x1; break;
                    case 2: LEFT_BUTTON = 0x1; break;
                    case 3: RIGHT_BUTTON = 0x1; break;
                    case 4: ENTER_BUTTON = 0x1; break;
                }
                BUTTON_STATES[STATE_INDEX].pressed = 0x1;  //sets the button to be pressed
                if (BUTTON_PRESSED == 0x0) BUTTON_STATES[STATE_INDEX].state = DEBOUNCE;  //if the button is released check it
                break;
                
        }
        
        STATE_INDEX++;
    }  
    T4CONbits.ON = 0x1;
}

void DEBOUNCE_INIT(void) {
    
    INTCONbits.MVEC = 0x1;
    
    //*** Oscillator Configuration ***
    SYSKEY = 0;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    PB2DIVCLR = 0x7F; // Divide by 1 (84 Mhz))
    PB3DIVSET = 0x7F; // Divide by 128 (656,250 Hz))
    SYSKEY = 0;
    
    //*** Input Setup
    // Set input
    TRISAbits.TRISA4 = 1; //RA4
    TRISAbits.TRISA5 = 1; //RA5
    TRISAbits.TRISA6 = 1; //RA6
    TRISAbits.TRISA7 = 1; //RA7
    TRISAbits.TRISA9 = 1; //RA9
    // Enable pulldowns
    CNPDAbits.CNPDA4 = 1; //RA4
    CNPDAbits.CNPDA5 = 1; //RA5
    CNPDAbits.CNPDA6 = 1; //RA6
    CNPDAbits.CNPDA7 = 1; //RA7
    CNPDAbits.CNPDA9 = 1; //RA9
    //Ansel Bits
    ANSELAbits.ANSA9 = 0x0;
    ANSELAbits.ANSA5 = 0x0;
    
    
    //*** Timer 4 setup **
    T4CONbits.TCKPS = 0x3; // 656,250 / 256 = 2,563 Hz
    PR4 = 0xDD; // 13 / 2,653 = 5 ms
    T4CONbits.ON = 0x1;   //turn the timer on
    IPC4bits.T4IP = 0x5;  //Interrupt
    
    //*** Interrupt Setup ***
    IEC0bits.T4IE = 0x1; // Enable timer 4 interrupt

}