/* Author: 			Peter Stine, Kyle Frischman, <others>
 * Date Created:		12/17/18
 * Last Modification Date:	12/17/18
 * Filename:			main.cpp
 * Dependency Level:		Top-Level
 *
 * Functional Overview:
 *
 * 	This program will utilize several other c++ and c-language libraries similar in function
 * 	to the github repositories listed in the references section of this header block.
 * 	This main.c file will act as the top level dependency for the LCD driver and
 * 	Radio Transciever Module Driver functions.
 *
 * Junior Project Specific Use Policies:
 *
 * 	For maintaining integrity of the function of this code repository, only the person
 * 	who's name is listed at the beginning of the file is permitted to write changes to
 * 	that file. All other persons will be allowed read-only access to a file where 
 * 	their name does not appear in the header.
 *
*/


#include <xc.h>
#include <sys/attribs.h>



#pragma config FPLLIDIV = DIV_2         // System PLL Input Divider (2x Divider) 12MHz/2 = 6MHz
#pragma config FPLLRNG = RANGE_5_10_MHZ // System PLL Input Range (5-10 MHz Input)
#pragma config FPLLICLK = PLL_POSC      // System PLL Input Clock Selection (POSC is input to the System PLL)
#pragma config FPLLMULT = MUL_112       // System PLL Multiplier (PLL Multiply by 112) 6MHz * 112 = 672MHz
#pragma config FPLLODIV = DIV_8         // System PLL Output Clock Divider (8x Divider) 672MHz / 8 = 84MHz
#pragma config UPLLFSEL = FREQ_12MHZ    // USB PLL Input Frequency Selection (USB PLL input is 12 MHz)

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


#include "RTC-RFM-GFX/GFX/LCD_Interface.h"
#include "RTC-RFM-GFX/RFM69/RF_Master.h"
#include "RTC-RFM-GFX/RTC/RTC2.h"

int main(void)
{
    ///////////////////////////
    //Configure Syskey Params//
    ///////////////////////////
    //PB2DIV = 84MHz (No Division)
    //System PB2DIV Setup
    SYSKEY = 0; // Ensure lock
    SYSKEY = 0xAA996655; // Write Key 1
    SYSKEY = 0x556699AA; // Write Key 2
    PB2DIV = _PB2DIV_ON_MASK | 0 & _PB2DIV_PBDIV_MASK; // 0 = div by 
    RTCCONbits.RTCWREN = 1;
    SYSKEY = 0; // Re lock
    
    //Wait states = 1
    //Flash Prefetch On
    PRECON = (1 & _PRECON_PFMWS_MASK) | 
            ((2 << _PRECON_PREFEN_POSITION) & _PRECON_PREFEN_MASK);
    
     //Enable Multi-Vector Mode
    INTCONSET = _INTCON_MVEC_MASK;
    
	///////////////////////////
    //Initialize All Hardware//
    ///////////////////////////
    
    //Initialize Real Time Clock
    clock_init();
    RTC_Config();
    
    
    //Initialize LCD
    LCD_Init();
    
    
    //Initialize Radio
    RF_Init();
    
    while(1)
    {
        //System Running Loop
        //Use for polling tasks if needed
    }
    
	return 0;
}
