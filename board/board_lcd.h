/*
    Board_core.h
    Board core Driver header file

    eMKa elektronika
    Maciej Kowalski 
*/


#ifndef BOARD_LCD_H
#define BOARD_LCD_H

#include <stdbool.h>
#include <stdint.h>

#define BOARD_LCD_COLUMNn   128
#define BOARD_LCD_PAGEn     8
#define BOARD_LCD_BUFFER_SIZE   BOARD_LCD_COLUMNn*BOARD_LCD_PAGEn

#define BOARD_LCD_CMDn      15

#define BOARD_LCD_CMD_POWER_ON          0x2F
#define BOARD_LCD_CMD_POWER_OFF         0x28

#define BOARD_LCD_CMD_ALL_PIX_OFF       0xA4
#define BOARD_LCD_CMD_BIAS_1D7          0xA3
#define BOARD_LCD_CMD_BIAS_1D9          0xA2

#define BOARD_LCD_CMD_DISPLAY_NORMAL        0xA6
#define BOARD_LCD_CMD_DISPLAY_INVERSE       0xA7

#define BOARD_LCD_CMD_SEG_DIR_NORM      0xA0
#define BOARD_LCD_CMD_SEG_DIR_REVERSE   0xA1

#define BOARD_LCD_CMD_COM_NORMAL        0xC0
#define BOARD_LCD_CMD_COM_REVERSE       0xC8

#define BOARD_LCD_CMD_DISP_ON           0xAF


extern uint8_t *board_lcd_data_buffer;

extern bool board_lcd_transmission_in_progress;

typedef enum lcd_disp_on_t
{
    disp_off,
    disp_on
}lcd_disp_on_t;

typedef enum lcd_seg_dir_t
{
    seg_dir_normal,
    seg_dir_reverse
}lcd_seg_dir_t;

typedef enum lcd_disp_inverse_t
{
    disp_no_inverse,
    disp_inverse
}lcd_disp_inverse_t;

typedef enum lcd_all_pixel_on_t
{
    normal_display,
    all_pixel_on
}lcd_all_pixel_on_t;

typedef enum lcd_bias_t
{
    bias_1d9,
    bias_1d7
}lcd_bias_t;

typedef enum lcd_com_dir_t
{
    com_dir_normal,
    com_dir_reverse
}lcd_com_dir_t;

void board_lcd_init(void);
void board_lcd_hardware_reset(void);

void board_lcd_set_display_data_address(uint8_t *data_address);

void board_lcd_send_display_data(void);
void board_lcd_send_config_data(void);

void board_lcd_routine(void);

void board_lcd_activate(void);
void board_lcd_deactivate(void);
bool board_lcd_check_if_active(void);

void board_lcd_backlight_enable(uint32_t lcd_on_time_ms);
void board_lcd_backlight_disable(void);

void boardLcdRefresh(void);


#endif
