
#ifndef _BUZZER_H    /* Guard against multiple inclusion */
#define _BUZZER_H


#define BUZZER_PIN LATBbits.LATB0
#define BUZZER_ACTIVE_STATE 1
#define BUZZER_INACTIVE_STATE 0

typedef struct {
    uint8_t BUZZER_ON;
    float BUZZER_DURATION;
    float BUZZER_TIME_REMAINING;
} BUZZER_DATA_T;

BUZZER_DATA_T BUZZER_DATA;

// Initializes piezo. Run once at system startup
void PIEZO_INIT(void);
// assigns buzzer to 'state' for 'duration' in seconds. maximum value // 0.99.
void PIEZO_BUZZER(uint8_t state, float duration);
// returns the status of the buzzer in BUZZER_DATA_T format.
BUZZER_DATA_T BUZZER_STATUS (void);
void __ISR_AT_VECTOR(_TIMER_2_VECTOR, IPL6SOFT) PIEZO_BUZZER_ISR(void);
#endif