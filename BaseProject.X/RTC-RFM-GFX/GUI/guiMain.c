/* 
 * File:   guiMain.c
 * Author: kjfrischman, Colin Lundquist
 *
 * Created on February 3, 2019, 11:46 AM
 */

#include <xc.h>
#include <sys/attribs.h>
#include <string.h>
#include <stdlib.h>
#include "guiMain.h"
#include "../GFX/tft_gfx.h"
#include "../GFX/tft_master.h"
#include "../SD/SD_Driver.h"


GUI_FLOW_T flow_data;
GUI_LEVELS_T level_data;
GUI_STATS_T stats_data;
GUI_CP_T cp_data;
GUI_SURFACE_T surface_data;

extern Summary_Data_T summary_data;

extern int UP_BUTTON;
extern int DOWN_BUTTON;
extern int ENTER_BUTTON;
extern int LEFT_BUTTON;
extern int RIGHT_BUTTON;


//Extern Settings Variables
extern uint8_t PUMP_STATE;
extern uint8_t TANK_HEIGHT;
extern uint8_t TANK_DIAMETER;
extern uint8_t STAFF_HEIGHT;
extern uint8_t UNITS;
extern uint8_t BUZZER_STATE;
extern uint8_t PUMP_DELAY;

int gui_state;



int gui_load_bmp(char * location)
{
    //Load BMP File
    FIL fileptr;
    BMPHeaderTMP bmpFHT;
    BMPHeader bmpFH;
    char *bitmapImage;
    int imageIdx = 0;
    char tempRGB;
    int read;
    
    
    
    FRESULT res = f_open(&fileptr, location, FA_READ);
    
    if (res == FR_OK)
    {
        f_read(&fileptr, &bmpFHT, sizeof(BMPHeaderTMP), &read);
        //Fix Struct
        struct_fix(&bmpFHT, &bmpFH);
        //Move to bitmap data
        //bmpFH.size = (240 * 300 * sizeof(char) * 3);
        
        //f_lseek(&fileptr, bmpFH.offset);
        f_lseek(&fileptr, 0x44);
        
        bitmapImage = (char *) malloc (bmpFH.size);
        
        if(!bitmapImage)
        {
         f_close(&fileptr);
         
        }
        
        asm("di");
        
        f_read(&fileptr, (void *)bitmapImage, (bmpFH.size), &read);
        
        asm("ei");
        
        if (bitmapImage == NULL)
        {
            f_close(&fileptr);
            
        }
        
        int x = 0;
        int y = 320;
        for (imageIdx = 2; imageIdx < bmpFH.size; imageIdx+=2)
        {
           
              
            uint16_t pixel = (bitmapImage[imageIdx+1]<<8) | (bitmapImage[imageIdx]);
            x++;
            
            //Store Pixels in Array and write 1 line at a time
            
            if(x == 240)
            {
                
                x = 0;
                y--;
            }
            
            if(y >= 15) tft_drawPixel(x,y,pixel);
            
            
        }
        
        //Write lines in loop
        
        
        free(bitmapImage);
        f_close(&fileptr);
    }
    else
    {
        //Bad Open
    }
    

}

void struct_fix(BMPHeaderTMP * bmpFHT, BMPHeader * bmpFH)
{
    //Read it all into 2 temps
    uint8_t size1;
    uint8_t size2;
    uint8_t size3;
    uint8_t size4;
    
    size1 = (bmpFHT->size) & 0x0000FF00;
    size2 = (bmpFHT->size) & 0x000000FF;
    size3 = (bmpFHT->type) >> 24;
    size4 = (bmpFHT->type)  >> 16;
    
    //Merge together
    bmpFH->size = 0;
    bmpFH->size += size1 << 24;
    bmpFH->size += size2 << 16;
    bmpFH->size += size3 << 8;
    bmpFH->size += size4;
    
    
}

void gui_init(void)
{   
    //FIL bmp =  sd_bitmap_stream(locations[current]);
    //Load first image (bootup image/video?)
    gui_load_bmp(LEVELS_PAGE);
    level_data.newdata = 1;
    
    IFS0bits.T3IF = 0x0; // Clear existing flag
    IEC0bits.T3IE = 0x1; // Timer 3 Interrupt Enabled
    IPC3bits.T3IP = 0x4; // Priority 4
    
    T3CONbits.TCKPS = 0x6; // Prescaler = 64. 656,250 / 64 = 10,254 Hz
    PR3 = 2050; // 100 ms = 1,025 / 10,254
    
    T3CONbits.ON = 0x1; // Turn the polling timer on
    
}
/* Button Layout:
 *
 * 
 *          ^ 
 *        < x > 
 *          v
 *        
 * Key:
 *
 *      ^ UP
 *      < LEFT
 *      > RIGHT
 *      v DOWN
 *      x ENTER
 * 
 * Button Functions:
 * 
 *      UP and DOWN move the selection cursor (used for settings)
 *      LEFT and RIGHT change the main page
 *      ENTER chooses menu options, and returns from them
 * 
 */

uint8_t newpage = 0;

void __ISR_AT_VECTOR(_TIMER_3_VECTOR, IPL4SRS) gui_polling_isr(void) 
{
    
    static uint8_t old_cursor = -1;
    static uint8_t old_select = 0;
    //*** Handle Inputs ***  
    if (gui_page == SETTINGS){ // only track cursor on relevant pages
        if (item_selected == 0x0){
            if(UP_BUTTON && cursor < NUM_SETTINGS)cursor++;
            else if (UP_BUTTON && cursor == NUM_SETTINGS)cursor = 0;
            else if(DOWN_BUTTON && cursor > 0) cursor--;
            else if(DOWN_BUTTON && cursor == 0) cursor = NUM_SETTINGS;
        }
        else { // Item selected, gather data entry direction
            if (UP_BUTTON) selection_input = 1;
            else if (DOWN_BUTTON) selection_input = -1;
            else selection_input = 0;
        }
        
        if (ENTER_BUTTON) item_selected = ~item_selected;
    }
    else {
        cursor = 0x0; // reset cursor on irrelevant pages
        item_selected = 0x0; // unselect item on pages without items
    }
    
    if (RIGHT_BUTTON){
        if (gui_page == 0) gui_page = NUM_PAGES-1;
        else gui_page--;
        
       newpage = 1;
    }
    if (LEFT_BUTTON){
        if (gui_page == (NUM_PAGES - 1)) gui_page = 0x0;
        else gui_page++;
        
        newpage = 1;
    }
    
    if(newpage)
    {
        switch(gui_page)
        {
            case LEVELS: {
            gui_load_bmp(LEVELS_PAGE);
            level_data.newdata = 1;
            break;
        }
            case FLOW: {
            gui_load_bmp(FLOW_PAGE);
            flow_data.newdata = 1;
            break;
            
            case STATS:{
            gui_load_bmp(STATS_PAGE);
            stats_data.newdata = 1;
                break;
            }
            
            case SETTINGS:{
                draw_settings_init();
            }
        }
        }
        newpage = 0;
    }
   
    //*** Handle / Draw Pages ***
    switch (gui_page){
    
        case LEVELS: {
            draw_levels(&level_data);
            break;
        }
        case FLOW: {
            draw_flow(&flow_data);
            break;
        }
        case STATS:
        {
            draw_stats(&stats_data);
            break;
        }
        case SETTINGS: {
            
            if(cursor != old_cursor)
            {
                old_cursor = cursor;
                switch(cursor){
                    case 0:
                        //Hightlight Settings First (x,y,w,h, clr)
                        tft_drawRect(0,60,240,20,0xFC00);
                        //UnSelect Settings Last
                        tft_drawRect(0, 260 , 240, 20, 0x0000);
                        //unselect Next
                        tft_drawRect(0, 80 , 240, 20, 0x0000);
                        break;
                    case 1:
                        //Hightlight Settings First (x,y,w,h, clr)
                        tft_drawRect(0,80,240,20,0xFC00);
                        //UnSelect Settings Last
                        tft_drawRect(0, 60 , 240, 20, 0x0000);
                        //unselect Next
                        tft_drawRect(0, 100 , 240, 20, 0x0000);
                        break;
                    case 2:
                        //Hightlight Settings First (x,y,w,h, clr)
                        tft_drawRect(0,100,240,20,0xFC00);
                        //UnSelect Settings Last
                        tft_drawRect(0, 80 , 240, 20, 0x0000);
                        //unselect Next
                        tft_drawRect(0, 120 , 240, 20, 0x0000);
                        break;
                    case 3:
                        //Hightlight Settings First (x,y,w,h, clr)
                        tft_drawRect(0,120,240,20,0xFC00);
                        //UnSelect Settings Last
                        tft_drawRect(0, 100 , 240, 20, 0x0000);
                        //unselect Next
                        tft_drawRect(0, 140 , 240, 20, 0x0000);
                        break;

                    case 4:
                        //Hightlight Settings First (x,y,w,h, clr)
                        tft_drawRect(0,140,240,20,0xFC00);
                        //UnSelect Settings Last
                        tft_drawRect(0, 120 , 240, 20, 0x0000);
                        //unselect Next
                        tft_drawRect(0, 160 , 240, 20, 0x0000);
                        break;

                    case 5:
                        //Hightlight Settings First (x,y,w,h, clr)
                        tft_drawRect(0,160,240,20,0xFC00);
                        //UnSelect Settings Last
                        tft_drawRect(0, 140 , 240, 20, 0x0000);
                        //unselect Next
                        tft_drawRect(0, 180 , 240, 20, 0x0000);
                        break;

                    case 6:
                        //Hightlight Settings First (x,y,w,h, clr)
                        tft_drawRect(0,180,240,20,0xFC00);
                        //UnSelect Settings Last
                        tft_drawRect(0, 160 , 240, 20, 0x0000);
                        //unselect Next
                        tft_drawRect(0, 200 , 240, 20, 0x0000);
                        break;

                    case 7:
                        //Hightlight Settings First (x,y,w,h, clr)
                        tft_drawRect(0,200,240,20,0xFC00);
                        //UnSelect Settings Last
                        tft_drawRect(0, 180 , 240, 20, 0x0000);
                        //unselect Next
                        tft_drawRect(0, 220 , 240, 20, 0x0000);
                        break;

                    case 8:
                        //Hightlight Settings First (x,y,w,h, clr)
                        tft_drawRect(0,220,240,20,0xFC00);
                        //UnSelect Settings Last
                        tft_drawRect(0, 200 , 240, 20, 0x0000);
                        //unselect Next
                        tft_drawRect(0, 240 , 240, 20, 0x0000);
                        break;

                    case 9:
                        //Hightlight Settings First (x,y,w,h, clr)
                        tft_drawRect(0,240,240,20,0xFC00);
                        //UnSelect Settings Last
                        tft_drawRect(0, 220 , 240, 20, 0x0000);
                        //unselect Next
                        tft_drawRect(0, 260 , 240, 20, 0x0000);
                        break;

                    case 10:
                        //Hightlight Settings First (x,y,w,h, clr)
                        tft_drawRect(0,260,240,20,0xFC00);
                        //UnSelect Settings Last
                        tft_drawRect(0, 240 , 240, 20, 0x0000);
                        //unselect Next
                        tft_drawRect(0, 60 , 240, 20, 0x0000);
                        break;

                }

                
            
            }
            
            draw_settings(cursor, &item_selected, selection_input, &old_cursor);
            
            break;
            
        }
    
    }
    
    IFS0bits.T3IF = 0x0; // Clear existing flag
    
}

void draw_levels(GUI_LEVELS_T * data)
{
    
    if (data->newdata)
    {
        data->newdata = 0;
        //Draw Rect
        tft_fillRect(0, 275, 240, 45, 0xFFFF);
        tft_setTextColor(0x0000);
        //Move Cursor
        tft_setCursor(10, 275);
        //Set Font Size
        tft_setTextSize(2);
        //Write Variable
        
        tft_writeString(data->src_level);
        //Tab
        tft_writeString("           ");
        //Write Units
        tft_writeString("Gallons");  
        //Enter
        tft_setCursor(10, 295);
        tft_writeString(data->res_fill_percent);
        //Tab
        tft_writeString("%          ");
        //Write Units
        tft_writeString("Full");  
        
        
        //Draw Cool Tank Levels
        tft_fillRect(48,80,96,146, 0x059F);
        int percent = atoi(data->res_fill_percent);
        percent = 100 - percent ;
        int height = (146* (percent) / 100);
        //if(height == 0) height = 146;
        tft_fillRect(48,80,96, height,0xFFFF);
    }
}
void draw_flow(GUI_FLOW_T * data)
{
    if (data->newdata)
    {
        data->newdata = 0;
        //Draw Rect
        tft_fillRect(0, 275, 240, 45, 0xFFFF);
        tft_setTextColor(0x0000);
        //Move Cursor
        tft_setCursor(10, 275);
        //Set Font Size
        tft_setTextSize(2);
        //Write Variable
        
        tft_writeString(data->flow_rate);
        //Tab
        tft_writeString("           ");
        //Write Units
        tft_writeString("GPH");  
    }
    
}
void draw_stats(GUI_STATS_T * data)
{
    //Purposly Ignore new data to cycle instead
    uint8_t delay = 0;
    uint8_t cycle = 0;
    if (delay = 50)
    {
        delay = 0;
        
        //Draw bottom 2 items in a loop
        switch(cycle)
        {
            case 0:
                //Cyclce first 2 data items
                //Draw Rect
                tft_fillRect(0, 275, 240, 45, 0xFFFF);
                tft_setTextColor(0x0000);
                //Move Cursor
                tft_setCursor(10, 275);
                //Set Font Size
                tft_setTextSize(2);
                //Write Variable

                tft_writeString(data->todays_flow);
                //Tab
                tft_writeString("   ");
                //Write Units
                tft_writeString("Gallons Used Today");  
                //Enter
                tft_setCursor(10, 295);
                tft_writeString(data->week_flow);
                //Tab
                tft_writeString("   ");
                //Write Units
                tft_writeString("Gallons This Week");  
                cycle = 1;
                break;
            
            case 1:
                //Draw Rect
                tft_fillRect(0, 275, 240, 45, 0xFFFF);
                tft_setTextColor(0x0000);
                //Move Cursor
                tft_setCursor(10, 275);
                //Set Font Size
                tft_setTextSize(2);
                //Write Variable

                tft_writeString(data->month_flow);
                //Tab
                tft_writeString("   ");
                //Write Units
                tft_writeString("Gallons This Month");  
                //Enter
                tft_setCursor(10, 295);
                tft_writeString(data->year_flow);
                //Tab
                tft_writeString("   ");
                //Write Units
                tft_writeString("Gallons This Year");  
                cycle = 0;
                break;
            
            
        }
    }
    else delay++;
}
void draw_settings(uint8_t curs, uint8_t * item_sel, int val, uint8_t  * old_cursor)
{
    
     char buffer [20];
    
    if(*item_sel)
    {

        switch (curs)
        {
            case 0:
                //Toggle Pump
                if(PUMP_STATE) PUMP_STATE = 0;
                else PUMP_STATE = 1;
                newpage = 1;
                *item_sel = 0;
                snprintf(buffer, 20, "PT%d", PUMP_STATE);
                RF_SEND(buffer);
                break;

            case 1:
                if(val == 1 && PUMP_DELAY < 0xFFFF) 
                {
                    PUMP_DELAY ++;
                    newpage = 1;
                }
                else if (val == -1 && PUMP_DELAY >= 0) 
                {
                    PUMP_DELAY --;
                    newpage = 1;
                }
                snprintf(buffer, 20, "PD%d", PUMP_DELAY);
                RF_SEND(buffer);
                
                break;


            case 2:
                if(val == 1 && TANK_HEIGHT < 0xFFFF) 
                {
                    TANK_HEIGHT ++;
                    newpage = 1;
                }
                else if (val == -1 && TANK_HEIGHT >= 0) 
                {
                    TANK_HEIGHT --;
                    newpage = 1;
                }
                
                snprintf(buffer, 20, "TH%d", TANK_HEIGHT);
                RF_SEND(buffer);
                
                break;
                
            case 3:
                if(val == 1 && TANK_DIAMETER < 0xFFFF) 
                {
                    TANK_DIAMETER ++;
                    newpage = 1;
                }
                else if (val == -1 && TANK_DIAMETER >= 0) 
                {
                    TANK_DIAMETER --;
                    newpage = 1;
                }
                
                snprintf(buffer, 20, "TD%d", TANK_DIAMETER);
                RF_SEND(buffer);
                
                break;
                
            case 4:
                if(val == 1 && STAFF_HEIGHT < 0xFFFF) 
                {
                    STAFF_HEIGHT ++;
                    newpage = 1;
                }
                else if (val == -1 && STAFF_HEIGHT >= 0) 
                {
                    STAFF_HEIGHT --;
                    newpage = 1;
                }
                
                snprintf(buffer, 20, "SH%d", STAFF_HEIGHT);
                RF_SEND(buffer);
                
                break;
                
                
            case 5:
                //Toggle Pump
                if(UNITS) UNITS = 0;
                else UNITS = 1;
                newpage = 1;
                *item_sel = 0;
                break;
                
            case 6:
                //Toggle Pump
                if(BUZZER_STATE) BUZZER_STATE = 0;
                else BUZZER_STATE = 1;
                newpage = 1;
                *item_sel = 0;
                break;
        }
        
        *old_cursor = -1;
    }
    
}

void draw_settings_init(void)
{
    asm("di");
    
    char dataBuffer[50];
    
    char * settingsArray[] = {"Toggle Pump   ", "Pump Delay   " , "Tank Height  ", "Tank Diameter", 
                              "Staff Height " , "Toggle Units  " , "Buzzer Toggle " ,"Change Date", "Change Time",
                              "Eject SD", "Mount SD"};
    
    uint8_t * dataArray[] = {&PUMP_STATE, &PUMP_DELAY, &TANK_HEIGHT, &TANK_DIAMETER, &STAFF_HEIGHT, 
                              &UNITS, &BUZZER_STATE};
    
    //Draw Black Background
    tft_fillRect(0, 15, 240, 305, 0x0000);
    //Draw Settings Text
    tft_setTextColor(0xFFFF);
    //Move Cursor
    tft_setCursor(10, 30);
    //Set Font Size
    tft_setTextSize(3);
    tft_writeString("Settings");
    tft_setTextSize(1.5);        
    //Loop Writing Stuff
    int count = 0;
    for (; count < NUM_SETTINGS+1; count++)
    {
        tft_setCursor(10, (20*(count+1)) + SETTINGS_OFFSET);
        if (count < 7) snprintf(dataBuffer, 50,"%s             %d", settingsArray[count], *(dataArray[count]));
        else snprintf(dataBuffer, 50, "%s", settingsArray[count]);
        tft_writeString(dataBuffer);
    }
    
    asm("ei");
}

