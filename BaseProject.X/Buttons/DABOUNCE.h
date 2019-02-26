#ifndef DABOUNCE_H
#define DABOUNCE_H

#define NUM_BUTTONS 5
#define UNPRESSED 0
#define DEBOUNCE 1
#define PRESSED 2

typedef struct{
    uint8_t pressed: 1;
    uint8_t state: 2;
    uint8_t reserved: 4;
}BUTTON_STATE_T;



void DEBOUNCE_INIT(void);






#endif