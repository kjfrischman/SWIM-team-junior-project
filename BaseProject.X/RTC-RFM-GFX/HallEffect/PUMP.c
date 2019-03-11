#include <xc.h>
#include <sys/attribs.h>
#include "PUMP.h"

// Sets up the I/O pin for pump usage
void PUMP_INIT(void) {
    TRISAbits.TRISA1 = 0; // RA1 is output
    PUMP_PIN = PUMP_INACTIVE_STATE;
}
// Turns the pump on
void PUMP_ON(void) {
    PUMP_PIN = PUMP_ACTIVE_STATE;
    PUMP_STAT = PUMP_ACTIVE_STATE;
    T1CONbits.ON = 0x1;
}
// Turns the pump off
void PUMP_OFF(void){
    PUMP_PIN = PUMP_INACTIVE_STATE;
    PUMP_STAT = PUMP_INACTIVE_STATE;
}
// Returns to current state of the pump (active or inactive)
uint8_t PUMP_STATUS(void){
    return(PUMP_STAT);
}

void PUMP_TOGGLE(uint8_t state)
{
    if(state)
    {
        PUMP_ON();
    }
    else
    {
        PUMP_OFF();
    }
}