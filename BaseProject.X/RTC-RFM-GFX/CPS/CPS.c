// This file contains the drivers for the Conductive Presence sensor.
// 2/18/2019
// Peter Stine
// CPS.c
// Junior Project

#include <xc.h>
#include <sys/attribs.h>
#include "CPS.h"

void CPS_config(void){
	
    // Setup 16-bit capture mode Timer 3
    // IC1CON
    // ICTMR
  //  IC1CONbit.ICTMRbits;

    // Poll ICx Input pin. If the Pin is 3.3v, then trigger interrupt

    // Once interrupt triggers, save current time of RTC and exit interrupt.

    // Continue to trigger interrupt, until pin is no longer high.  

}

// CPS Vector
//void __ISR_AT_VECTOR(_RTCC_VECTOR, IPL4SOFT) CPS_HANDLER(void)
//{
// Set global variable high
// water_detected = 1;
//Clear Interrupt
//IFS5bits.RTCCIF = 0;
//}
