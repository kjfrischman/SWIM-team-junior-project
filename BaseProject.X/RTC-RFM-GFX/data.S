
// Global Data variables used in conjunction with the app_clock file.
// 
// Global variables
.global clock_second
.global clock_minute
.global clock_hour
.global clock_day
.global clock_month
.global clock_year
.global TIME_INIT
.global clock_state
.global clock_count
.global LCD_BUFFER
.global button1
.global button2
.global button3
.global button4

    
// Variable declarations
.data
TIME_INIT:		.asciiz "Date:  /  /      Time:  :  :  "
clock_count:		.word 0
clock_state:		.word 0
clock_second:		.word 0
clock_minute:		.word 0
clock_hour:		.word 0
clock_day:		.word 0
clock_month:		.word 0
clock_year:		.word 0

LCD_BUFFER:		.space 20
    
button1:		.word 0
button2:		.word 0
button3:		.word 0
button4:		.word 0

//
// Global Data variables used in conjunction with the app_clock file.

    
    
// Global Variables For Sensor Levels Transmitted By Radio
//extern int GlobalFlowRate;
//extern int GlobalTankLevel;
//extern int GlobalSurfaceLevel;
       
.global GlobalFlowRate
.global GlobalTankLevel
.global GlobalSurfaceLevel

.data
    GlobalFlowRate:	    .word 0
    GlobalTankLevel:	    .word 0
    GlobalSurfaceLevel:	    .word 0
    
    
.global UP_BUTTON
.global DOWN_BUTTON
.global ENTER_BUTTON
.global LEFT_BUTTON
.global RIGHT_BUTTON
    
.data
    UP_BUTTON:		.word 0
    DOWN_BUTTON:	.word 0
    ENTER_BUTTON:	.word 0
    LEFT_BUTTON:	.word 0
    RIGHT_BUTTON:	.word 0
    
    
    //////////////////////////////////////////////////
    //Settings Global Vars
    /////////////////////////////////////////////////
    
.global PUMP_STATE
.global TANK_HEIGHT
.global TANK_DIAMETER
.global STAFF_HEIGHT
.global MIN
.global HOUR
.global SEC
.global DAY
.global MONTH
.global YEAR
.global UNITS
.global BUZZER_STATE
.global PUMP_DELAY
    
    
    .data
PUMP_STATE:	    .word 0
TANK_HEIGHT:	    .word 10
TANK_DIAMETER:	    .word 10
STAFF_HEIGHT:	    .word 10
UNITS:		    .word 0
BUZZER_STATE:	    .word 0
PUMP_DELAY:	    .word 10