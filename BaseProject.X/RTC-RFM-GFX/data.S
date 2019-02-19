
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

// End of:
// Global Data variables used in conjunction with the app_clock file.


// Global Data variables used in conjunction with the CPS.c file.
// 
// Global variables
.global water_detected
    
// Variable declarations
.data
water_detected:		.word 0
    
    
// End of: 
// Global Data variables used in conjunction with the CPS.c file.
