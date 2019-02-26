#include <xc.h>
#include <sys/attribs.h>
#include "DeBOUNCE.h"

// PIC32MZ2048ECG144 Configuration Bit Settings
// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // System PLL Input Divider (2x Divider) 12MHz/2 = 6MHz
#pragma config FPLLRNG = RANGE_5_10_MHZ // System PLL Input Range (5-10 MHz Input)
#pragma config FPLLICLK = PLL_POSC      // System PLL Input Clock Selection (POSC is input to the System PLL)
#pragma config FPLLMULT = MUL_112       // System PLL Multiplier (PLL Multiply by 112) 6MHz * 112 = 672MHz
#pragma config FPLLODIV = DIV_8         // System PLL Output Clock Divider (8x Divider) 672MHz / 8 = 84MHz
#pragma config UPLLFSEL = FREQ_12MHZ    // USB PLL Input Frequency Selection (USB PLL input is 12 MHz)
//#pragma config UPLLEN = OFF             // USB PLL Enable (USB PLL is disabled)

// DEVCFG1
#pragma config FNOSC = SPLL             // Oscillator Selection Bits (Primary Osc (HS,EC))
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disable SOSC)
#pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disabled, FSCM Disabled)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled)
#pragma config FDMTEN = OFF             // Deadman Timer Enable (Deadman Timer is disabled)

// DEVCFG0
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
#pragma config ICESEL = ICS_PGx2        // ICE/ICD Comm Channel Select (Communicate on PGEC2/PGED2)

BUTTON_STATE_T BUTTON_STATES[NUM_BUTTONS];
BUTTON_GLOBAL_STATES_T GLOBAL_BUTTON_STATES;
uint8_t STATE_INDEX = 0;

int main(void){
   
    DEBOUNCE_INIT();

//    IFS0bits.T4IF = 0x1;
    while('a') {}
    
    return 0;
}

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
                BUTTON_STATES[STATE_INDEX].pressed = 0x0;  //State is unpressed
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
    PR4 = 0xD; // 13 / 2,653 = 5 ms
    T4CONbits.ON = 0x1;   //turn the timer on
    IPC4bits.T4IP = 0x5;  //Interrupt
    
    //*** Interrupt Setup ***
    IEC0bits.T4IE = 0x1; // Enable timer 4 interrupt
    PRISSbits.PRI5SS = 0x4;  //set the shadow register set on
    
    asm("ei"); //enable interrupts
    
}