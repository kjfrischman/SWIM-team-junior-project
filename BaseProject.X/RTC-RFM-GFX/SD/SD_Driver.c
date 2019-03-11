
#include "SD_Driver.h"

UINT s1, s2, cnt;
DWORD ofs = 0, sect = 0;
FRESULT res;
FATFS fs;				/* Pointer to file system object */
DIR dir;				/* Directory object */


void SD_Init()
{    
    char buffer [50] = "SD Test String\n";
    char readBuffer[50];
    FIL fp;
    UINT bytesWrit;
    UINT bytesRead;
    
    //Turn on SPI2
    //Clear MSSEN Bit in SPI2CON<28>, CKE = 0, SMP = 0
    SPI2BRG = 0x8;
    
    SPI2CONCLR = 0x10000300;
    // CKP = 1
    SPI2CONSET = 0x8060;
    //PPS
    ANSELBbits.ANSB14 = 0;
    //Set LATB
    LATBbits.LATB14 = 1;
    //Clear TRISB
    TRISBbits.TRISB14 = 0;
    LATJbits.LATJ5 = 0;
    TRISJbits.TRISJ5 = 0;
    
    SDI2R = 0b1110; //RD7 In
    RPG8R = 0b0110; //RG8 Out
    
    
    //Init SD Card
    if (disk_initialize(0))
    {
        //SD Card Error
        LED_LAT = 1;
    }
    
    //Mount SD Card
    if (f_mount(0, &fs) == FR_INVALID_DRIVE)
    {
        //Mount Error
        LED_LAT = 1;
    }
    
    //Create test file to ensure SD card isnt write protected
  res = f_open(&fp, "SDTEST.TXT", FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
  
  if (res)
  {
      //Write Protection Error
      LED_LAT = 1;
  }
  else
  {
      //Write to file
      f_write(&fp, &buffer, sizeof(buffer), &bytesWrit);
      //Make sure we can read it
      f_close(&fp);
      f_open(&fp, "SDWriteTest.txt", FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
      f_read(&fp, &readBuffer, bytesWrit, &bytesRead);
      //Make sure it matches
      if (bytesWrit != bytesRead)
      {
          //Read Error
      }
      
      //Close File
      f_close(&fp);
  }
   
}

FIL sd_bitmap_stream(char * path)
{
    FIL  bmp;
    //Try to find matching file
    res = f_open(&bmp, path, FA_READ);
    if (res == FR_OK)
    {
        //Pass pointer to stream
        
    }
    else
    {
        //Display Error Screen?
    }
    
    return bmp;
}


FRESULT sd_directoy_list(char * path)
{
    static FILINFO fno;
    int i = 0;
    
    res = f_opendir(&dir, path);
    if (res == FR_OK)
    {
        for (;;)
        {
        res = f_readdir(&dir, &fno);                /* Read a directory item */
        if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
        if (fno.fattrib & AM_DIR) 
        {                    /* It is a directory */
            i = strlen(path);
            //Print to Card
            tft_setCursor(0, i+10);
            tft_writeString(&path[i]);
            res = sd_directoy_list(path);                    /* Enter the directory */
            if (res != FR_OK) break;
            path[i] = 0;
        } else {    /* It is a file. */
            tft_setCursor(0, i+10);
            tft_writeString(path);
            }
        }
    }
    return res;
}

void sd_get_pixel(FIL * bmp,char * color, int length)
{
    //f_gets(color, length, bmp);
}