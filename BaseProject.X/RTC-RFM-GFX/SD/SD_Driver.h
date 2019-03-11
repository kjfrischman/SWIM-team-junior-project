/* 
 * File:   SD_Driver.h
 * Author: kjfrischman
 *
 * Created on February 3, 2019, 11:48 AM
 */

#ifndef SD_DRIVER_H
#define	SD_DRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SD_CS
#define SD_
    
#include <proc/p32mz2048efg144.h>
#include <string.h>

#include "ff13/diskio.h"
#include "ff13/ff.h"
#include "ff13/ffconf.h"
    
    #define LED_LAT     LATHbits.LATH2
    
    void SD_Init(void);
    //void sd_write(uint8_t);
    FIL sd_bitmap_stream(char * path);
    FRESULT sd_directoy_list(char * path);
    void sd_get_pixel(FIL * bmp,char * color, int length);

#ifdef	__cplusplus
}
#endif

#endif	/* SD_DRIVER_H */

