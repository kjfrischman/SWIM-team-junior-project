
#include <sys/attribs.h>
#include <xc.h>

#include "Data_Proc.h"
#include "../SD/SD_Driver.h"

typedef struct{
    float daily_total;
    float weekly_total;
    float monthly_total;
    float yearly_total;
    
} Summary_Data_T;

#define DATA_STRUCT_SIZE 4

typedef struct{
    // uint8_t level ?
    char res_fill_percent[DATA_STRUCT_SIZE];
    char src_level[DATA_STRUCT_SIZE];
    uint8_t newdata;
}GUI_LEVELS_T;
    
typedef struct{
    char surface_height[DATA_STRUCT_SIZE];
    uint8_t newdata;
}GUI_SURFACE_T;

typedef struct{
    char flow_rate[DATA_STRUCT_SIZE];
    uint8_t newdata;
    // daily use, other things?
}GUI_FLOW_T;

Summary_Data_T summary_data;
extern GUI_FLOW_T flow_data;
extern GUI_LEVELS_T level_data;
extern GUI_SURFACE_T surface_data;

void Data_Proc_Init(void)
{
    //Setup Timer 5
    T5CONbits.TCKPS = 3;
    //1025
    PR5 = 5000;
    
    IEC0bits.T5IE = 1;
    IFS0bits.T5IF = 0;
    IPC6bits.T5IP = 5;
    
    //2500ms Delay
    T5CONbits.ON = 1;
}

void Data_Proc_Write(void)
{
    //Write All Data To SD Card
    char buffer[500];
    FIL dataFile;
    
    uint8_t curDay = (RTCDATEbits.DAY10 << 4 | RTCDATEbits.DAY01);
    uint8_t curYear = (RTCDATEbits.MONTH10 << 4 | RTCDATEbits.MONTH01);
    uint8_t curMonth = (RTCDATEbits.YEAR10 << 4 | RTCDATEbits.YEAR01);
    

    //Open File
    snprintf(buffer, 500, "Date/%d/%d/%d.csv", curYear, curMonth, curDay);
    f_open(&dataFile, buffer, FA_OPEN_APPEND | FA_WRITE | FA_READ);
    
    //Store everything into a buffer
    snprintf(buffer, 500, "Date:%d-%d-%d, Yearly Total:%f, Monthly Total:%f,"
            "Weekly Total:%f, Today:%f, Tank Height:%f, Source Height:%f", 
            curYear, curMonth, curDay, summary_data.yearly_total, 
            summary_data.monthly_total, summary_data.weekly_total,
            summary_data.daily_total, level_data.src_level, surface_data.surface_height);
    
    //Write to File
    UINT written;
    f_write(&dataFile, buffer, strlen(buffer), &written);
     
    //Close File 
     f_close(&dataFile);

}

void Data_Proc_Stats(void)
{
    //Update all the statistics
    float flow_total = atof(flow_data.flow_rate) / (720); //Divide for 5 secs
    summary_data.daily_total += flow_total;
    summary_data.monthly_total += flow_total;
    summary_data.yearly_total += flow_total;
}

void __ISR_AT_VECTOR (_TIMER_5_VECTOR, IPL5SRS)Data_Proc_Handler(void)
{
    //Turn off Timer
    T5CONbits.ON = 0;
    IFS0bits.T5IF = 0;
    //Clear Val
    TMR5 = 0;
    static uint8_t turn = 0; 
    //Alternate between writing to SD Card and Stat update
    if(turn)
    {
        Data_Proc_Write();
    }
    else
    {
        Data_Proc_Stats();
    }
    
    turn = !turn;
    
    //Start Timer Again
    T5CONbits.ON = 1;
}