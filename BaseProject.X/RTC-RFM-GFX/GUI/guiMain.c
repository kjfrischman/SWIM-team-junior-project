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
//#include "../SD/SD_Driver.h"
#include "../GFX/tft_gfx.h"
#include "../GFX/tft_master.h"
#include "../SD/SD_Driver.h"


GUI_FLOW_T flow_data;
GUI_LEVELS_T level_data;
GUI_STATS_T stats_data;

extern int UP_BUTTON;
extern int DOWN_BUTTON;
extern int ENTER_BUTTON;
extern int LEFT_BUTTON;
extern int RIGHT_BUTTON;

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
        
        f_read(&fileptr, (void *)bitmapImage, (bmpFH.size), &read);
        
        if (bitmapImage == NULL)
        {
            f_close(&fileptr);
            
        }
        
        int x = 0;
        int y = 320;
        for (imageIdx = 2; imageIdx < bmpFH.size; imageIdx+=2)
        {
            uint16_t pixel;
              
            pixel = (bitmapImage[imageIdx+1]<<8) | (bitmapImage[imageIdx]);
            
            if(x == 240)
            {
                x = 0;
                y--;
            }
            if(y >= 15) tft_drawPixel(x,y, pixel);
            
            x++;
            
        }
        
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

void __ISR_AT_VECTOR(_TIMER_3_VECTOR, IPL4SRS) gui_polling_isr(void) 
{
    uint8_t newpage = 0;
    
    //*** Handle Inputs ***  
    if (gui_page == SETTINGS){ // only track cursor on relevant pages
        if (item_selected == 0x0){
            cursor += UP_BUTTON;
            cursor -= DOWN_BUTTON;
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
    
    if (LEFT_BUTTON){
        if (gui_page == 0) gui_state = NUM_PAGES;
        else gui_page--;
        
       newpage = 1;
    }
    if (RIGHT_BUTTON){
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
            gui_load_bmp(SETTINGS_PAGE);
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
            
            switch(cursor){
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    
                    break;
                    
            }
            
            draw_settings(cursor, item_selected);
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
    }
}
void draw_flow(GUI_FLOW_T * data)
{
    
    
}
void draw_stats(GUI_STATS_T * data)
{
    
}
void draw_settings(uint8_t curs, uint8_t item_sel)
{
    
}

