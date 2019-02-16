//Kyle Frischman
//Junior Project 


#include <xc.h>
#include <sys/attribs.h>
#include "RTC2.h"
#include "../RFM69/RF_Master.h"


#define LCD_Color ILI9340_GREY
#define LCD_TXT ILI9340_WHITE 
#define LCD_PRESENT 0
#define LCD_Width 240
#define Time_H 12
#define TIME_SPACE 12
#define LCD_SEC 170
#define LCD_MIN 150
#define LCD_HR 130
#define LCD_DAY 70
#define LCD_MTH 50
#define LCD_YR 30

#if defined(LCD_PRESENT)
#include "RTC_Display.h"
#endif

char buffer[20];
extern char TIME_INIT[20];

void app_clock(void)
{
    switch (clock_state)
    {
        case 0:
                //New Year
                if (clock_second >= 59 && clock_minute >= 59 && clock_hour >= 24 && clock_month >= 12 && clock_day >= 31)
                {
                    clock_second = 0;
                    clock_minute = 0;
                    clock_hour = 0;
                    clock_day = 1;
                    clock_month = 1;
                    clock_year++;
                    clock_state = 6;
                }
                //New Month
                else if (clock_second >= 59 && clock_minute >= 59 && clock_hour >= 24 && clock_day >= 28)
                {
                    switch(clock_month)
                    {
                        case 0:
                            //Error State
                            //Throw An Error
                        break; 
                        
                        case 1:
                            //January 31 Days
                            if (clock_day == 31)
                            {
                                clock_month_inc();
                            }
                            else
                            {
                                clock_day_inc();
                            }
                        break;    
                        
                        case 2:
                            //Check Leap Year
                            if (clock_year % 4)
                            {
                                if (clock_day == 29)
                                {
                                    clock_month_inc();
                                }
                                else
                                {
                                    clock_day_inc();
                                }
                            }
                            else
                            {
                                //28 Days
                                clock_month_inc();
                            }
                        break; 
                        
                        case 3:
                            //March 31 Days
                            if (clock_day == 31)
                            {
                                clock_month_inc();
                            }
                            else
                            {
                                clock_day_inc();
                            }
                        break; 
                        
                        case 4:
                            //April 30 Days
                            if (clock_day == 30)
                            {
                                clock_month_inc();
                            }
                            else
                            {
                                clock_day_inc();
                            }
                        break; 
                        
                        case 5:
                            //May 31 Days
                            if (clock_day == 31)
                            {
                                clock_month_inc();
                            }
                            else
                            {
                                clock_day_inc();
                            }
                        break; 
                        
                        case 6:
                            //June 30
                            if (clock_day == 30)
                            {
                                clock_month_inc();
                            }
                            else
                            {
                                clock_day_inc();
                            }
                        break; 
                        
                        case 7:
                            //July 31
                            if (clock_day == 31)
                            {
                                clock_month_inc();
                            }
                            else
                            {
                                clock_day_inc();
                            }
                        break;
                        
                        case 8:
                            //August 31
                            if (clock_day == 31)
                            {
                                clock_month_inc();
                            }
                            else
                            {
                                clock_day_inc();
                            }
                        break; 
                        
                        case 9:
                            //September 30
                            if (clock_day == 30)
                            {
                                clock_month_inc();
                            }
                            else
                            {
                                clock_day_inc();
                            }
                        break; 
                        
                        case 10:
                            //October 31
                            if (clock_day == 31)
                            {
                                clock_month_inc();
                            }
                            else
                            {
                                clock_day_inc();
                            }
                        break; 
                        
                        case 11:
                            //November 30
                            if (clock_day == 30)
                            {
                                clock_month_inc();
                            }
                            else
                            {
                                clock_day_inc();
                            }
                        break; 
                        
                        case 12:
                            //December ? Error State?
                        break; 
                    }
             
                }
                //New Day
                else if (clock_second >= 59 && clock_minute >= 59 && clock_hour >= 24)
                 {
                    clock_day_inc();
                 }
                //New Hour
                else if (clock_second >= 59 && clock_minute >= 59)
                {
                    clock_second = 0;
                    clock_minute = 0;
                    clock_hour++;
                    clock_state = 3;
                }
                //New Minute
                else if (clock_second >= 59)
                {
                    clock_second = 0;
                    clock_minute++;
                    clock_state = 2;
                }
                //New Second
                else
                {
                    clock_second++;
                    clock_state = 1;
                }
             break;
        
         case 1:
            //Return to State 0
            clock_state = 0;
            //Set Seconds
            #if defined(LCD_PRESENT)
            clockfield_to_lcdstr(clock_second, 1, buffer);
            #endif
            //Blink LED
            RF_SEND(2, buffer, 2);
            //Write Current Second Value
            TimeUpdate(LCD_SEC, 0, TIME_SPACE, Time_H);
    
             break;
             
         case 2:
             //Update Minute Display
             //Set Minutes
            #if defined(LCD_PRESENT)
            clockfield_to_lcdstr(clock_minute, 1, buffer);
            #endif
            //Write Current Minute Value
            TimeUpdate(LCD_MIN, 0, TIME_SPACE, Time_H);
             
             //Go To State 1
             clock_state = 1;
             break;
             
             
         case 3:
             //Update Hour Display
             //Set Hours
            #if defined(LCD_PRESENT) 
            clockfield_to_lcdstr(clock_hour, 0, buffer);
            #endif
            //Move Cursor To Hour Position (0x86)
            TimeUpdate(LCD_HR, 0, TIME_SPACE, Time_H);
            
            //Go To State 2
            clock_state = 2;
            break;
             
        case 4:
             //Update Day
            #if defined(LCD_PRESENT)
            clockfield_to_lcdstr(clock_day, 0, buffer);
            #endif
            //Write Current Day
            TimeUpdate(LCD_DAY, 0, TIME_SPACE, Time_H);
            
            //Go To State 3
            clock_state = 3;
            break;
             
             
        case 5:
             //Update Month
            #if defined(LCD_PRESENT)
            clockfield_to_lcdstr(clock_month, 0,buffer);
            #endif
            //Set Month
            TimeUpdate(LCD_MTH, 0, TIME_SPACE, Time_H);
            
            //Go To State 4
            clock_state = 4;
            break;
             
             
        case 6:
            //Update Year
            #if defined(LCD_PRESENT)
            clockfield_to_lcdstr(clock_year, 0, buffer);
            #endif
            //Set Year
            TimeUpdate(LCD_YR, 0, TIME_SPACE, Time_H);
            
            //Go To State 5
            clock_state = 5;
            break;     
     }
     
}

void clock_day_inc (void)
{
    clock_second = 0;
    clock_minute = 0;
    clock_hour = 1;
    clock_day++;
    clock_state = 4;
}

void clock_month_inc (void)
{
    clock_second = 0;
    clock_minute = 0;
    clock_hour = 1;
    clock_day = 1;
    clock_month++;
    clock_state = 5;
}

void clock_init(void)
{
    //Set Year
    clock_year = 19;
    //Set Month
    clock_month = 1;        
    //Set Day
    clock_day = 11;
    //Set Hour
    clock_hour = 14;
    //Set Minute
    clock_minute = 1;
    //Set Second
    clock_second = 1;
    
#if defined(LCD_PRESENT)
    lcd_time_init();
#endif
}

void RTC_Config(void)
{
    //Configure Priority 
    IPC41bits.RTCCIP = 4;
    IEC5bits.RTCCIE = 1;
       
    //Real Time Alarm Config
    RTCALRMbits.CHIME = 1;
    RTCALRMbits.AMASK = 0;
    RTCALRMbits.ALRMEN = 1;
    
    //Real Time Clock Config
    RTCCONbits.RTCOE = 1;
    RTCCONbits.ON = 1;
}

//RTC Vector
void __ISR_AT_VECTOR(_RTCC_VECTOR, IPL4SOFT) RTC_HANDLER(void)
{
    //Call Real Time Clock
    app_clock();
    //Clear Interrupt
    IFS5bits.RTCCIF = 0;
}

void TimeUpdate(short x, short y, short w, short h)
{
#if defined(LCD_PRESENT)
    TimeUpdateLCD(x,y,w,h,buffer);    
#endif
}