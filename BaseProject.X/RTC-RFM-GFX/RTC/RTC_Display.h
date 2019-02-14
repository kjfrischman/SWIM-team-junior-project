/* 
 * File:   RTC_Display.h
 * Author: kjfrischman
 *
 * Created on February 14, 2019, 2:28 PM
 */
/*
 * This file is to only be included if you are running an LCD Display
 * LED Heartbeat is not dependent on this file
 */


#ifndef RTC_DISPLAY_H
#define	RTC_DISPLAY_H

#define HOUR_POS    0x87
#define MIN_POS     0x8A
#define SEC_POS     0x8D

#define LCD_Color ILI9340_GREY
#define LCD_TXT ILI9340_WHITE 
#define LCD_Width 240
#define Time_H 12
#define TIME_SPACE 12
#define LCD_SEC 170
#define LCD_MIN 150
#define LCD_HR 130
#define LCD_DAY 70
#define LCD_MTH 50
#define LCD_YR 30

#include "../GFX/tft_master.h"
#include "../GFX/tft_gfx.h"

extern int clock_count;
extern int clock_state;
extern int clock_second;
extern int clock_minute;
extern int clock_hour;
extern int clock_day;
extern int clock_month;
extern long long clock_year;

extern char TIME_INIT[20];



void lcd_time_init (void);

void TimeUpdateLCD(short x, short y, short w, short h, char *buffer);

void clockfield_to_lcdstr (int clockfield, int field_type, char buffer[]);

#endif	/* RTC_DISPLAY_H */

