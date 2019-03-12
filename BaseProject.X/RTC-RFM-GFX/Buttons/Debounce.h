#ifndef DABOUNCE_H
#define DEBOUNCE_H

#define NUM_BUTTONS 5
#define UNPRESSED 0
#define DEBOUNCE 1
#define PRESSED 2

typedef struct{
    uint8_t pressed: 1;
    uint8_t state: 2;
    uint8_t reserved: 4;
}BUTTON_STATE_T;

int UP_BUTTON;
int DOWN_BUTTON;
int ENTER_BUTTON;
int LEFT_BUTTON;
int RIGHT_BUTTON;

void DEBOUNCE_INIT(void);






#endif