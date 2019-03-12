/* 
 * File:   LCD_Interface.c
 * Author: kjfrischman
 *
 * Created on January 16, 2019, 9:47 PM
 */


#include "LCD_Interface.h"
#include "../GFX/tft_gfx.h"
#include "../GFX/tft_master.h"

void LCD_Init()
{
    //Start LCD Hardware
    tft_init_hw();
    tft_begin();
    tft_setRotation(2);
    tft_fillScreen(ILI9340_RED);
    //Set Default Background
    //Fill Screen With Startup
    tft_setTextColor(ILI9340_GREEN);
    tft_setCursor(0,140);
    //Display Loading Screen
    tft_setTextSize(2);
    tft_writeString("Initializing  System\n");
}

void ScreenSaver()
{ 
    
  tft_fillRect(0,12,240,308,ILI9340_BLACK);  
    
  int i, i2, cx, cy;

  cx = 240  / 2 - 1;
  cy = 300 / 2 - 1;


  for(i=min(240, 300); i>20; i-=6) {
    i2 = i / 2;
    tft_fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft_Color565(0, i, 0));
  }
  
  int           x1, y1, x2, y2,
                w = 240,
                h = 300;

  tft_fillRect(0,12,240,308,ILI9340_BLACK);

  x1 = y1 = 13;
  y2    = h - 1;
  for(x2=0; x2<w; x2+=6) tft_drawLine(x1, y1, x2, y2, ILI9340_MAGENTA);
  x2    = w - 1;
  for(y2=13; y2<h; y2+=6) tft_drawLine(x1, y1, x2, y2, ILI9340_MAGENTA);
  
  tft_fillRect(0,12,240,308,ILI9340_BLACK);

  x1    = w - 1;
  y1    = 13;
  y2    = h - 1;
  for(x2=0; x2<w; x2+=6) tft_drawLine(x1, y1, x2, y2, ILI9340_CYAN);
  x2    = 0;
  for(y2=13; y2<h; y2+=6) tft_drawLine(x1, y1, x2, y2, ILI9340_CYAN);

  tft_fillRect(0,12,240,308,ILI9340_BLACK);

  x1    = 0;
  y1    = h - 1;
  y2    = 13;
  for(x2=0; x2<w; x2+=6) tft_drawLine(x1, y1, x2, y2, ILI9340_MAGENTA);
  x2    = w - 1;
  for(y2=13; y2<h; y2+=6) tft_drawLine(x1, y1, x2, y2, ILI9340_MAGENTA);

  tft_fillRect(0,12,240,308,ILI9340_BLACK);

  x1    = w - 1;
  y1    = h - 1;
  y2    = 13;
  for(x2=0; x2<w; x2+=6) tft_drawLine(x1, y1, x2, y2, ILI9340_YELLOW);
  x2    = 0;
  for(y2=13; y2<h; y2+=6) tft_drawLine(x1, y1, x2, y2, ILI9340_YELLOW);
}

void lcd_time_init(void)
{
    char buffer[20];
    
    //MUST be called AFTER LCD_Init
    //Set 'Time:    :  :  '
    tft_fillRect(0,0,240,18,LCD_Color );
    
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
    tft_setTextSize(1);
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
    buffer[2] = '\0';

}
