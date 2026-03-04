#include <gpio.h>
#include <spi.h>
#include <ILI9225.h>
#include <string.h>

LCD_Config lcd_config = {
    .hspi = &hspi2,
    .cs_port = GPIOD,
    .cs_pin = GPIO_PIN_11,
    .rs_port = GPIOD,
    .rs_pin = GPIO_PIN_13,
    .rst_port = GPIOD,
    .rst_pin = GPIO_PIN_12,
};

// 5x7 font for ASCII 32-126
static const uint8_t font5x7[][5] = {
    {0x00,0x00,0x00,0x00,0x00}, // space
    {0x00,0x00,0x5F,0x00,0x00}, // !
    {0x00,0x07,0x00,0x07,0x00}, // "
    {0x14,0x7F,0x14,0x7F,0x14}, // #
    {0x24,0x2A,0x7F,0x2A,0x12}, // $
    {0x23,0x13,0x08,0x64,0x62}, // %
    {0x36,0x49,0x55,0x22,0x50}, // &
    {0x00,0x05,0x03,0x00,0x00}, // '
    {0x00,0x1C,0x22,0x41,0x00}, // (
    {0x00,0x41,0x22,0x1C,0x00}, // )
    {0x08,0x2A,0x1C,0x2A,0x08}, // *
    {0x08,0x08,0x3E,0x08,0x08}, // +
    {0x00,0x50,0x30,0x00,0x00}, // ,
    {0x08,0x08,0x08,0x08,0x08}, // -
    {0x00,0x60,0x60,0x00,0x00}, // .
    {0x20,0x10,0x08,0x04,0x02}, // /
    {0x3E,0x51,0x49,0x45,0x3E}, // 0
    {0x00,0x42,0x7F,0x40,0x00}, // 1
    {0x42,0x61,0x51,0x49,0x46}, // 2
    {0x21,0x41,0x45,0x4B,0x31}, // 3
    {0x18,0x14,0x12,0x7F,0x10}, // 4
    {0x27,0x45,0x45,0x45,0x39}, // 5
    {0x3C,0x4A,0x49,0x49,0x30}, // 6
    {0x01,0x71,0x09,0x05,0x03}, // 7
    {0x36,0x49,0x49,0x49,0x36}, // 8
    {0x06,0x49,0x49,0x29,0x1E}, // 9
    {0x00,0x36,0x36,0x00,0x00}, // :
    {0x00,0x56,0x36,0x00,0x00}, // ;
    {0x00,0x08,0x14,0x22,0x41}, // <
    {0x14,0x14,0x14,0x14,0x14}, // =
    {0x41,0x22,0x14,0x08,0x00}, // >
    {0x02,0x01,0x51,0x09,0x06}, // ?
    {0x32,0x49,0x79,0x41,0x3E}, // @
    {0x7E,0x11,0x11,0x11,0x7E}, // A
    {0x7F,0x49,0x49,0x49,0x36}, // B
    {0x3E,0x41,0x41,0x41,0x22}, // C
    {0x7F,0x41,0x41,0x22,0x1C}, // D
    {0x7F,0x49,0x49,0x49,0x41}, // E
    {0x7F,0x09,0x09,0x01,0x01}, // F
    {0x3E,0x41,0x41,0x51,0x32}, // G
    {0x7F,0x08,0x08,0x08,0x7F}, // H
    {0x00,0x41,0x7F,0x41,0x00}, // I
    {0x20,0x40,0x41,0x3F,0x01}, // J
    {0x7F,0x08,0x14,0x22,0x41}, // K
    {0x7F,0x40,0x40,0x40,0x40}, // L
    {0x7F,0x02,0x04,0x02,0x7F}, // M
    {0x7F,0x04,0x08,0x10,0x7F}, // N
    {0x3E,0x41,0x41,0x41,0x3E}, // O
    {0x7F,0x09,0x09,0x09,0x06}, // P
    {0x3E,0x41,0x51,0x21,0x5E}, // Q
    {0x7F,0x09,0x19,0x29,0x46}, // R
    {0x46,0x49,0x49,0x49,0x31}, // S
    {0x01,0x01,0x7F,0x01,0x01}, // T
    {0x3F,0x40,0x40,0x40,0x3F}, // U
    {0x1F,0x20,0x40,0x20,0x1F}, // V
    {0x7F,0x20,0x18,0x20,0x7F}, // W
    {0x63,0x14,0x08,0x14,0x63}, // X
    {0x03,0x04,0x78,0x04,0x03}, // Y
    {0x61,0x51,0x49,0x45,0x43}, // Z
    {0x00,0x00,0x7F,0x41,0x41}, // [
    {0x02,0x04,0x08,0x10,0x20}, // backslash
    {0x41,0x41,0x7F,0x00,0x00}, // ]
    {0x04,0x02,0x01,0x02,0x04}, // ^
    {0x40,0x40,0x40,0x40,0x40}, // _
    {0x00,0x01,0x02,0x04,0x00}, // `
    {0x20,0x54,0x54,0x54,0x78}, // a
    {0x7F,0x48,0x44,0x44,0x38}, // b
    {0x38,0x44,0x44,0x44,0x20}, // c
    {0x38,0x44,0x44,0x48,0x7F}, // d
    {0x38,0x54,0x54,0x54,0x18}, // e
    {0x08,0x7E,0x09,0x01,0x02}, // f
    {0x08,0x14,0x54,0x54,0x3C}, // g
    {0x7F,0x08,0x04,0x04,0x78}, // h
    {0x00,0x44,0x7D,0x40,0x00}, // i
    {0x20,0x40,0x44,0x3D,0x00}, // j
    {0x00,0x7F,0x10,0x28,0x44}, // k
    {0x00,0x41,0x7F,0x40,0x00}, // l
    {0x7C,0x04,0x18,0x04,0x78}, // m
    {0x7C,0x08,0x04,0x04,0x78}, // n
    {0x38,0x44,0x44,0x44,0x38}, // o
    {0x7C,0x14,0x14,0x14,0x08}, // p
    {0x08,0x14,0x14,0x18,0x7C}, // q
    {0x7C,0x08,0x04,0x04,0x08}, // r
    {0x48,0x54,0x54,0x54,0x20}, // s
    {0x04,0x3F,0x44,0x40,0x20}, // t
    {0x3C,0x40,0x40,0x20,0x7C}, // u
    {0x1C,0x20,0x40,0x20,0x1C}, // v
    {0x3C,0x40,0x30,0x40,0x3C}, // w
    {0x44,0x28,0x10,0x28,0x44}, // x
    {0x0C,0x50,0x50,0x50,0x3C}, // y
    {0x44,0x64,0x54,0x4C,0x44}, // z
    {0x00,0x08,0x36,0x41,0x00}, // {
    {0x00,0x00,0x7F,0x00,0x00}, // |
    {0x00,0x41,0x36,0x08,0x00}, // }
    {0x08,0x08,0x2A,0x1C,0x08}, // ~
};

// cursor position
static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;

// CS pin - indicate whether the device is selected
void LCD_select() {
    HAL_GPIO_WritePin(lcd_config.cs_port, lcd_config.cs_pin, GPIO_PIN_RESET);
}

void LCD_unselect() {
    HAL_GPIO_WritePin(lcd_config.cs_port, lcd_config.cs_pin, GPIO_PIN_SET);
}

// DC pin - indicate command mode or data mode
void LCD_data_mode() {
    HAL_GPIO_WritePin(lcd_config.rs_port, lcd_config.rs_pin, GPIO_PIN_SET);
}

void LCD_cmd_mode() {
    HAL_GPIO_WritePin(lcd_config.rs_port, lcd_config.rs_pin, GPIO_PIN_RESET);
}

// RST pin - used to reset the device
void LCD_do_reset() {
    HAL_GPIO_WritePin(lcd_config.rst_port, lcd_config.rst_pin, GPIO_PIN_RESET);
}

void LCD_no_reset() {
    HAL_GPIO_WritePin(lcd_config.rst_port, lcd_config.rst_pin, GPIO_PIN_SET);
}

// send command byte - known the device is selected
void LCD_send_cmd(uint8_t cmd) {
    LCD_cmd_mode();
    HAL_SPI_Transmit(lcd_config.hspi, &cmd, 1, 10);
}

// send data byte - known the device is selected
void LCD_send_data(uint8_t data) {
    LCD_data_mode();
    HAL_SPI_Transmit(lcd_config.hspi, &data, 1, 10);
}

// send multiple data bytes - known the device is selected
void LCD_send_group_data(uint8_t* data, uint16_t len) {
    LCD_data_mode();
    HAL_SPI_Transmit(lcd_config.hspi, data, len, 100);
}

// hardware reset sequence
void LCD_reset() {
    LCD_no_reset();
    HAL_Delay(5);
    LCD_do_reset();
    HAL_Delay(20);
    LCD_no_reset();
    HAL_Delay(150);
}

// software reset command
void LCD_soft_reset() {
    LCD_select();
    LCD_send_cmd(0x01);
    LCD_unselect();
    HAL_Delay(150);
}

// power control settings
void LCD_set_power(uint8_t pwr1, uint8_t pwr2, uint8_t vcom1_h, uint8_t vcom1_l, uint8_t vcom2) {
    LCD_select();
    // power control 1
    LCD_send_cmd(0xC0);
    LCD_send_data(pwr1);
    // power control 2
    LCD_send_cmd(0xC1);
    LCD_send_data(pwr2);
    // VCOM control 1
    LCD_send_cmd(0xC5);
    LCD_send_data(vcom1_h);
    LCD_send_data(vcom1_l);
    // VCOM control 2
    LCD_send_cmd(0xC7);
    LCD_send_data(vcom2);
    LCD_unselect();
}

// display configuration
void LCD_set_display(uint8_t orientation, uint8_t pixel_format, uint8_t frame_div, uint8_t frame_clk) {
    LCD_select();
    // memory access control (orientation)
    LCD_send_cmd(0x36);
    LCD_send_data(orientation);
    // pixel format
    LCD_send_cmd(0x3A);
    LCD_send_data(pixel_format);
    // frame rate control
    LCD_send_cmd(0xB1);
    LCD_send_data(frame_div);
    LCD_send_data(frame_clk);
    LCD_unselect();
}

// display function control
void LCD_set_function(uint8_t bypass, uint8_t scan_mode, uint8_t num_lines) {
    LCD_select();
    LCD_send_cmd(0xB6);
    LCD_send_data(bypass);
    LCD_send_data(scan_mode);
    LCD_send_data(num_lines);
    LCD_unselect();
}

// sleep control
void LCD_sleep_out() {
    LCD_select();
    LCD_send_cmd(0x11);
    LCD_unselect();
    HAL_Delay(120);
}

void LCD_sleep_in() {
    LCD_select();
    LCD_send_cmd(0x10);
    LCD_unselect();
    HAL_Delay(5);
}

// display on/off
void LCD_display_on() {
    LCD_select();
    LCD_send_cmd(0x29);
    LCD_unselect();
}

void LCD_display_off() {
    LCD_select();
    LCD_send_cmd(0x28);
    LCD_unselect();
}

void LCD_init() {
    // hardware reset
    LCD_reset();

    // software reset
    LCD_soft_reset();

    // display off during init
    LCD_display_off();

    // power settings (defaults)
    LCD_set_power(0x23, 0x10, 0x3E, 0x28, 0x86);

    // display settings: orientation (0xA8 for landscape, 0x68 for portrait), pixel format (0x55 for 16-bit color), frame rate control (0x00 for 70Hz, 0x18 for 60Hz)
    LCD_set_display(0xA8, 0x55, 0x00, 0x18);

    // display function control
    LCD_set_function(0x08, 0x82, 0x27);

    // exit sleep
    LCD_sleep_out();

    // turn on display
    LCD_display_on();
    HAL_Delay(50);

    // clear screen
    LCD_fill_screen(COLOR_WHITE);
}

// this LCD render things needs four parameters to set the window size from (x0, y0) to (x1, y1)
// LCD flow chart: CASET(2A) and PASET(2B) -> RAMWR(2C)
void LCD_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    LCD_select();

    // column address set
    LCD_send_cmd(0x2A);
    // x0 for SC, SC is 16 bit but divided into 2 parameters
    // x1 for EC, EC is 16 bit but divided into 2 parameters
    uint8_t col_data[4] = {x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF};
    LCD_send_group_data(col_data, 4);

    // row address set
    LCD_send_cmd(0x2B);
    uint8_t row_data[4] = {y0 >> 8, y0 & 0xFF, y1 >> 8, y1 & 0xFF};
    LCD_send_group_data(row_data, 4);

    // memory write command - tells ready to receive pixel data (color)
    LCD_send_cmd(0x2C);
    // CS stays LOW, caller sends pixel data then calls LCD_unselect
}

void LCD_fill_screen(uint16_t color) {
    LCD_set_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    // CS is already LOW from LCD_set_window
    // RAMWR here, set pixel data (color)
    uint8_t data[2] = {color >> 8, color & 0xFF};

    LCD_data_mode();
    // send pixel by pixel
    for (uint32_t i = 0; i < (uint32_t)LCD_WIDTH * LCD_HEIGHT; i++) {
        HAL_SPI_Transmit(lcd_config.hspi, data, 2, HAL_MAX_DELAY);
    }
    LCD_unselect();
}

void LCD_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT) return;

    LCD_set_window(x, y, x, y);
    // CS is LOW from LCD_set_window, start sending pixel data (color)
    uint8_t data[2] = {color >> 8, color & 0xFF};
    LCD_send_group_data(data, 2);
    LCD_unselect();
}

void LCD_draw_char(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg) {
    if (c < 32 || c > 126) c = '?'; // handle invalid characters

    const uint8_t* glyph = font5x7[c - 32]; // match user input character to the glyph

    // draw character using a window for better performance
    LCD_set_window(x, y, x + 5, y + 6);
    LCD_data_mode();

    uint8_t color_hi = color >> 8;
    uint8_t color_lo = color & 0xFF;
    uint8_t bg_hi = bg >> 8;
    uint8_t bg_lo = bg & 0xFF;

    // font is 5 cols x 7 rows, plus 1 col spacing = 6x7 total
    for (uint8_t row = 0; row < 7; row++) {
        for (uint8_t col = 0; col < 6; col++) {
            uint8_t pixel[2];
            if (col < 5 && (glyph[col] & (1 << row))) { // foreground color
                pixel[0] = color_hi;
                pixel[1] = color_lo;
            } else { // background color
                pixel[0] = bg_hi;
                pixel[1] = bg_lo;
            }
            HAL_SPI_Transmit(lcd_config.hspi, pixel, 2, 10);
        }
    }
    LCD_unselect();
}

void LCD_draw_string(uint16_t x, uint16_t y, char* str, uint16_t color, uint16_t bg) {
    y *= 8; // convert to pixels - 8 pixels per line
    while (*str) {
        if (x + 6 > LCD_WIDTH) {
            x = 0;
            y += 8;
        }
        if (y + 7 > LCD_HEIGHT) return;

        LCD_draw_char(x, y, *str, color, bg);
        x += 6;  // 5 pixels + 1 spacing
        str++;
    }
}

void LCD_set_cursor(uint16_t x, uint16_t y) {
    cursor_x = x;
    cursor_y = y;
}

void LCD_print(const char* str, uint16_t color, uint16_t bg) {
    while (*str) {
        if (cursor_x + 6 > LCD_WIDTH) {
            cursor_x = 0;
            cursor_y += 8;
        }
        if (cursor_y + 7 > LCD_HEIGHT) {
            cursor_y = 0;  // wrap to top
        }
        LCD_draw_char(cursor_x, cursor_y, *str, color, bg);
        cursor_x += 6;
        str++;
    }
}
