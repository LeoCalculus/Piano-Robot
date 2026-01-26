#ifndef __ILI9341_H
#define __ILI9341_H

#include <gpio.h>
#include <stdint.h>
#include <spi.h>

// display size
#define LCD_WIDTH  180
#define LCD_HEIGHT 500

// common colors (RGB565)
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0x001F
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0xF800
#define COLOR_YELLOW  0xFFE0
#define COLOR_CYAN    0x07FF
#define COLOR_MAGENTA 0xF81F

// LCD configuration struct - holds all pin and SPI info
typedef struct {
    SPI_HandleTypeDef* hspi;
    GPIO_TypeDef* cs_port;
    uint16_t cs_pin;
    GPIO_TypeDef* rs_port;
    uint16_t rs_pin;
    GPIO_TypeDef* rst_port;
    uint16_t rst_pin;
} LCD_Config;

// CS pin - indicate whether the device is selected
void LCD_select(LCD_Config* cfg);
void LCD_unselect(LCD_Config* cfg);

// DC pin - indicate command mode or data mode
void LCD_data_mode(LCD_Config* cfg);
void LCD_cmd_mode(LCD_Config* cfg);

// RST pin - used to reset the device
void LCD_do_reset(LCD_Config* cfg);
void LCD_no_reset(LCD_Config* cfg);

// low-level SPI functions
void LCD_send_cmd(LCD_Config* cfg, uint8_t cmd);
void LCD_send_data(LCD_Config* cfg, uint8_t data);
void LCD_send_group_data(LCD_Config* cfg, uint8_t* data, uint16_t len);

// reset functions
void LCD_reset(LCD_Config* cfg);
void LCD_soft_reset(LCD_Config* cfg);

// configuration functions (for custom settings)
void LCD_set_power(LCD_Config* cfg, uint8_t pwr1, uint8_t pwr2, uint8_t vcom1_h, uint8_t vcom1_l, uint8_t vcom2);
void LCD_set_display(LCD_Config* cfg, uint8_t orientation, uint8_t pixel_format, uint8_t frame_div, uint8_t frame_clk);
void LCD_set_function(LCD_Config* cfg, uint8_t bypass, uint8_t scan_mode, uint8_t num_lines);

// sleep control
void LCD_sleep_out(LCD_Config* cfg);
void LCD_sleep_in(LCD_Config* cfg);

// display on/off
void LCD_display_on(LCD_Config* cfg);
void LCD_display_off(LCD_Config* cfg);

// initialize LCD with default settings
void LCD_init(LCD_Config* cfg);

// set drawing window
void LCD_set_window(LCD_Config* cfg, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

// fill LCD with specific color (background)
void LCD_fill_screen(LCD_Config* cfg, uint16_t color);

// draw a pixel with color
void LCD_draw_pixel(LCD_Config* cfg, uint16_t x, uint16_t y, uint16_t color);

// draw single char - can be anywhere on the screen, non-zero based
void LCD_draw_char(LCD_Config* cfg, uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg);

// draw string x is row and y is column, y is multiplied by 8 to convert to pixels - 8 pixels per line
void LCD_draw_string(LCD_Config* cfg, uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bg);

// set the current cursor
void LCD_set_cursor(uint16_t x, uint16_t y);

// print stuff at cursor position
void LCD_print(LCD_Config* cfg, const char* str, uint16_t color, uint16_t bg);

#endif
