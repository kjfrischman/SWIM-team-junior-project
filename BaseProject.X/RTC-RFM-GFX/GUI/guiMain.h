/* 
 * File:   guiMain.h
 * Author: kjfrischman, Colin Lundquist
 *
 * Created on February 3, 2019, 11:45 AM
 */

#ifndef GUIMAIN_H
#define	GUIMAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

#define NUM_PAGES 4
#define NUM_SETTINGS 10
#define SETTINGS_OFFSET 45
    
#define LEVELS 0
#define LEVELS_PAGE "LVL.BMP"

#define FLOW 1
#define FLOW_PAGE "FR.BMP"

#define STATS 2
#define STATS_PAGE "STS.BMP"

#define SETTINGS 3
#define SETTINGS_PAGE "S1.BMP"   
    
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
    
    typedef struct{
        char todays_flow[DATA_STRUCT_SIZE];
        char week_flow[DATA_STRUCT_SIZE];
        char month_flow[DATA_STRUCT_SIZE];
        char year_flow[DATA_STRUCT_SIZE];
        uint8_t newdata;
    }GUI_STATS_T;
    
    typedef struct{
        uint8_t newdata;
        char presence[DATA_STRUCT_SIZE];
    }GUI_CP_T;
    
    
    typedef struct{
    float daily_total;
    float weekly_total;
    float monthly_total;
    float yearly_total;
    
} Summary_Data_T;
    
    //https://stackoverflow.com/questions/14279242/read-bitmap-file-into-structure

typedef struct { // Total: 54 bytes
  uint32_t  type;             // Magic identifier: 0x4d42
  uint32_t  size;             // File size in bytes
  uint32_t  reserved1;        // Not used
  uint32_t  reserved2;        // Not used
  uint32_t  offset;           // Offset to image data in bytes from beginning of file (54 bytes)
  uint32_t  dib_header_size;  // DIB Header size in bytes (40 bytes)
  int32_t   width_px;         // Width of the image
  int32_t   height_px;        // Height of image
  uint32_t  num_planes;       // Number of color planes
  uint32_t  bits_per_pixel;   // Bits per pixel
  uint32_t  compression;      // Compression type
  uint32_t  image_size_bytes; // Image size in bytes
  int32_t   x_resolution_ppm; // Pixels per meter
  int32_t   y_resolution_ppm; // Pixels per meter
  uint32_t  num_colors;       // Number of colors  
  uint32_t  important_colors; // Important colors 
} BMPHeaderTMP;

typedef struct {             // Total: 54 bytes
  uint16_t  type;             // Magic identifier: 0x4d42
  uint32_t  size;             // File size in bytes
  uint16_t  reserved1;        // Not used
  uint16_t  reserved2;        // Not used
  uint32_t  offset;           // Offset to image data in bytes from beginning of file (54 bytes)
  uint32_t  dib_header_size;  // DIB Header size in bytes (40 bytes)
  int32_t   width_px;         // Width of the image
  int32_t   height_px;        // Height of image
  uint16_t  num_planes;       // Number of color planes
  uint16_t  bits_per_pixel;   // Bits per pixel
  uint32_t  compression;      // Compression type
  uint32_t  image_size_bytes; // Image size in bytes
  int32_t   x_resolution_ppm; // Pixels per meter
  int32_t   y_resolution_ppm; // Pixels per meter
  uint32_t  num_colors;       // Number of colors  
  uint32_t  important_colors; // Important colors 
} BMPHeader;
    
    
    uint8_t gui_page = 0;
    
    int8_t  selection_input = 0; // Variable passed to indicate direction of
                                 // data entry direction. 1 = up, 0 = none,
                                 // -1 = down.
    uint8_t item_selected = 0;
    uint8_t cursor = 0;
   
    int gui_load_bmp(char * location);
    
    void gui_init(void);
    void gui_main_loop(void);
    
    void draw_levels(GUI_LEVELS_T * data);
    void draw_flow(GUI_FLOW_T * data);
    void draw_stats(GUI_STATS_T * data);
    void draw_settings(uint8_t curs, uint8_t * item_sel, int val, uint8_t  * old_cursor);
    void struct_fix(BMPHeaderTMP * bmpFHT, BMPHeader * bmpFH);
    void draw_settings_init(void);



#ifdef	__cplusplus
}
#endif

#endif	/* GUIMAIN_H */

