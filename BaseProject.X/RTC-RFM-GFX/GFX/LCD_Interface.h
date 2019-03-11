/* 
 * File:   LCD_Interface.h
 * Author: kjfrischman
 *
 * Created on January 16, 2019, 9:47 PM
 */

#ifndef LCD_INTERFACE_H
#define	LCD_INTERFACE_H

#ifdef	__cplusplus
extern "C" {
#endif

    void LCD_Init();
    void ScreenSaver();

    
    /////////////////////////////
    //Real Time Clock Functions//
    /////////////////////////////
    
#define HOUR_POS    0x87
#define MIN_POS     0x8A
#define SEC_POS     0x8D

#define LCD_Color 0x3D4C
#define LCD_TXT 0xFFFF
#define LCD_Width 240
#define Time_H 12
#define TIME_SPACE 12
#define LCD_SEC 170
#define LCD_MIN 150
#define LCD_HR 130
#define LCD_DAY 70
#define LCD_MTH 50
#define LCD_YR 30

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
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* LCD_INTERFACE_H */

