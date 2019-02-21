
#ifndef _HALL_EFFECT_H    /* Guard against multiple inclusion */
#define _HALL_EFFECT_H

#include <xc.h>
#include <sys/attribs.h>

float HALL_EFFECT_DELTA;
float HALL_DATA_ARRAY[10]; // Buffer to store averaging data

uint32_t TMR_DELTA = 0x0; // difference between last edge and the current one
uint8_t HALL_EFFECT_INDEX = 0x0;
uint8_t HALL_EFFECT_COUNT = 0x0;

float PUMP_SHUTOFF_TIME = 0.33; // Time between flow stopping and pump shutting off
float PREV_FLOW_RATE;

// Frequency = 6.6 * Q. (Q = L/Min)
// Max frequency = 30 L/Min * 6.6 = 198 Hz

void HALL_EFFECT_INIT (uint8_t tshuttoff);
float HALL_EFFECT_GET_RATE (uint8_t average);
void __ISR_AT_VECTOR(_TIMER_1_VECTOR, IPL6SRS) PUMP_SHUTOFF_ISR(void);
void __ISR_AT_VECTOR(_EXTERNAL_0_VECTOR, IPL7SRS) HALL_EFFECT_ISR(void);


#endif 

