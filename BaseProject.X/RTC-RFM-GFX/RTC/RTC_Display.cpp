

#include "RTC_Display.h"


void lcd_time_init(void)
{
    char buffer[20];
    
    //MUST be called AFTER LCD_Init
    //Set 'Time:    :  :  '
    tft_fillRect(0,0,240,12,LCD_Color );
    
    tft_setCursor(0, 0);
    tft_setTextColor(LCD_TXT );  
    tft_setTextSize(1);
    tft_writeString(TIME_INIT);
    
    //Set Month
    tft_setCursor(LCD_MTH, 0);
    clockfield_to_lcdstr(clock_month, 1, buffer);
    tft_writeString(buffer);
    
    //Set Day
    tft_setCursor(LCD_DAY, 0);
    clockfield_to_lcdstr(clock_day, 1, buffer);
    tft_writeString(buffer);
    
    //Set Year
    tft_setCursor(LCD_YR, 0);
    clockfield_to_lcdstr(clock_year, 1, buffer);
    tft_writeString(buffer);
    
    //Set Hours
    clockfield_to_lcdstr(clock_hour, 0, buffer);
    //Move Cursor To Hour Position (0x86)
    tft_setCursor(LCD_HR, 0);
    tft_writeString(buffer);
    
    //Set Minutes
    clockfield_to_lcdstr(clock_minute, 1, buffer);
    //Move Cursor To Minute Position (0x10)
    tft_setCursor(LCD_MIN, 0);
    tft_writeString(buffer);
    
    //Set Seconds
    clockfield_to_lcdstr(clock_second, 1, buffer);
    //Move Cursor To Second Position (0x13)
    tft_setCursor(LCD_SEC , 0);
    tft_writeString(buffer);
}


//Update Time on LCD Panel
void TimeUpdateLCD(short x, short y, short w, short h, char *buffer)
{
    tft_fillRect(x,y,w,h,LCD_Color ); 
    tft_setTextColor(LCD_TXT); 
    tft_setCursor(x, 0);
    tft_writeString(buffer);
}

void clockfield_to_lcdstr (int clockfield, int field_type, char buffer[])
{
    //Convert Values
    int temp = 0;
    
    temp = clockfield % 10;
    clockfield = clockfield / 10;
    if (clockfield == 0 && field_type == 0)
    {
        clockfield = 0x20;
    }
    else
    {
        clockfield += 0x30;
    }
    
    temp += 0x30;
    
    //Place In Buffer
    buffer[0] = clockfield;
    buffer[1] = temp;
    buffer[2] = '\n';

}