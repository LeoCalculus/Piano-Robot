// driver source: https://github.com/Nkawu/TFT_22_ILI9225/blob/master/src/TFT_22_ILI9225.cpp
// font lib from web

#include <gpio.h>
#include <spi.h>
#include <ILI9225.h>
#include <string.h>
#include <stdint.h>

LCD_Config lcd_config = {
    .hspi = &hspi3,
    .cs_port = LCD_CS_GPIO_Port,
    .cs_pin = LCD_CS_Pin,
    .rs_port = LCD_RST_GPIO_Port, // here RS and RST are reversed (misconfigured naming in schematic)
    .rs_pin = LCD_RST_Pin,
    .rst_port = LCD_RS_GPIO_Port,
    .rst_pin = LCD_RS_Pin,
};

// LCD buffer render in RAM (row-major, matches LCD scan order)
uint16_t plot_buffer[LCD_HEIGHT][LCD_WIDTH];

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

// CS control
void LCD_select(void) {
    HAL_GPIO_WritePin(lcd_config.cs_port, lcd_config.cs_pin, GPIO_PIN_RESET);
}

void LCD_unselect(void) {
    HAL_GPIO_WritePin(lcd_config.cs_port, lcd_config.cs_pin, GPIO_PIN_SET);
}

// RS pin: LOW = register/command index, HIGH = register data
static void LCD_rs_low(void) {
    HAL_GPIO_WritePin(lcd_config.rs_port, lcd_config.rs_pin, GPIO_PIN_RESET);
}

static void LCD_rs_high(void) {
    HAL_GPIO_WritePin(lcd_config.rs_port, lcd_config.rs_pin, GPIO_PIN_SET);
}

// ILI9225 protocol: write 16-bit register index, then 16-bit data
// RS=LOW for index, RS=HIGH for data
void LCD_write_reg(uint16_t reg, uint16_t data) {
    LCD_select();

    // send register index (RS low)
    LCD_rs_low();
    uint8_t reg_bytes[2] = {reg >> 8, reg & 0xFF};
    HAL_SPI_Transmit(lcd_config.hspi, reg_bytes, 2, HAL_MAX_DELAY);

    // send register data (RS high)
    LCD_rs_high();
    uint8_t data_bytes[2] = {data >> 8, data & 0xFF};
    HAL_SPI_Transmit(lcd_config.hspi, data_bytes, 2, HAL_MAX_DELAY);

    LCD_unselect();
}

// write 16-bit data (pixel data after GRAM register is selected)
void LCD_write_data16(uint16_t data) {
    uint8_t bytes[2] = {data >> 8, data & 0xFF};
    HAL_SPI_Transmit(lcd_config.hspi, bytes, 2, HAL_MAX_DELAY);
}

// select the GRAM register for writing pixel data
void LCD_start_gram_write(void) {
    LCD_select();

    // send GRAM register index (RS low)
    LCD_rs_low();
    uint8_t reg_bytes[2] = {0x00, ILI9225_GRAM_DATA_REG};
    HAL_SPI_Transmit(lcd_config.hspi, reg_bytes, 2, HAL_MAX_DELAY);

    // switch to data mode (RS high) - caller will send pixel data
    LCD_rs_high();
}

// hardware reset sequence
void LCD_reset(void) {
    HAL_GPIO_WritePin(lcd_config.rst_port, lcd_config.rst_pin, GPIO_PIN_SET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(lcd_config.rst_port, lcd_config.rst_pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(lcd_config.rst_port, lcd_config.rst_pin, GPIO_PIN_SET);
    HAL_Delay(150);
}

void LCD_init(void) {
    LCD_unselect();
    LCD_reset();

    // ---------- Power-on sequence (from ILI9225 datasheet) ----------

    // Power control 2: set step-up circuit
    LCD_write_reg(ILI9225_POWER_CTRL2, 0x0018);
    // Power control 3: set operating frequency
    LCD_write_reg(ILI9225_POWER_CTRL3, 0x6121);
    // Power control 4: VREG1OUT voltage
    LCD_write_reg(ILI9225_POWER_CTRL4, 0x006F);
    // Power control 5: VCOM amplitude
    LCD_write_reg(ILI9225_POWER_CTRL5, 0x495F);
    // Power control 1: start power supply
    LCD_write_reg(ILI9225_POWER_CTRL1, 0x0800);
    HAL_Delay(10);

    // Power control 2: step-up circuit 2
    LCD_write_reg(ILI9225_POWER_CTRL2, 0x103B);
    HAL_Delay(50);

    // ---------- Display control ----------

    // Driver output control: SS=0, GS=1, NL=28 (220 lines) - landscape 0°
    LCD_write_reg(ILI9225_DRIVER_OUTPUT_CTRL, 0x011C);
    // LCD AC driving control: line inversion
    LCD_write_reg(ILI9225_LCD_AC_DRIVING_CTRL, 0x0100);
    // Entry mode: BGR, AM=1 (vertical first), ID1=0 ID0=1
    LCD_write_reg(ILI9225_ENTRY_MODE, 0x1018);

    // Blank period control
    LCD_write_reg(ILI9225_BLANK_PERIOD_CTRL1, 0x0808);
    // Frame cycle control
    LCD_write_reg(ILI9225_FRAME_CYCLE_CTRL, 0x1100);
    // Interface control
    LCD_write_reg(ILI9225_INTERFACE_CTRL, 0x0000);
    // Oscillator control: enable oscillator
    LCD_write_reg(ILI9225_OSC_CTRL, 0x0D01);
    // VCI recycling
    LCD_write_reg(ILI9225_VCI_RECYCLING, 0x0020);

    // ---------- RAM address ----------
    LCD_write_reg(ILI9225_RAM_ADDR_SET1, 0x0000);
    LCD_write_reg(ILI9225_RAM_ADDR_SET2, 0x0000);

    // ---------- Gate scan control ----------
    LCD_write_reg(ILI9225_GATE_SCAN_CTRL, 0x0000);
    LCD_write_reg(ILI9225_VERTICAL_SCROLL_CTRL1, 0x00DB);
    LCD_write_reg(ILI9225_VERTICAL_SCROLL_CTRL2, 0x0000);
    LCD_write_reg(ILI9225_VERTICAL_SCROLL_CTRL3, 0x0000);
    LCD_write_reg(ILI9225_PARTIAL_DRIVING_POS1, 0x00DB);
    LCD_write_reg(ILI9225_PARTIAL_DRIVING_POS2, 0x0000);

    // ---------- Window address (full screen, landscape) ----------
    LCD_write_reg(ILI9225_HORIZONTAL_WINDOW_ADDR1, 0x00AF); // 175 (physical horizontal)
    LCD_write_reg(ILI9225_HORIZONTAL_WINDOW_ADDR2, 0x0000);
    LCD_write_reg(ILI9225_VERTICAL_WINDOW_ADDR1, 0x00DB); // 219 (physical vertical)
    LCD_write_reg(ILI9225_VERTICAL_WINDOW_ADDR2, 0x0000);

    // ---------- Gamma control ----------
    LCD_write_reg(ILI9225_GAMMA_CTRL1, 0x0000);
    LCD_write_reg(ILI9225_GAMMA_CTRL2, 0x0808);
    LCD_write_reg(ILI9225_GAMMA_CTRL3, 0x080A);
    LCD_write_reg(ILI9225_GAMMA_CTRL4, 0x000A);
    LCD_write_reg(ILI9225_GAMMA_CTRL5, 0x0A08);
    LCD_write_reg(ILI9225_GAMMA_CTRL6, 0x0808);
    LCD_write_reg(ILI9225_GAMMA_CTRL7, 0x0000);
    LCD_write_reg(ILI9225_GAMMA_CTRL8, 0x0A00);
    LCD_write_reg(ILI9225_GAMMA_CTRL9, 0x0710);
    LCD_write_reg(ILI9225_GAMMA_CTRL10, 0x0710);

    // ---------- Display ON ----------
    LCD_write_reg(ILI9225_DISP_CTRL1, 0x0012);
    HAL_Delay(50);
    LCD_write_reg(ILI9225_DISP_CTRL1, 0x1017);

    // clear screen
    LCD_fill_screen(COLOR_WHITE);
}

void LCD_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // landscape 180°: logical x -> physical vertical (inverted), logical y -> physical horizontal
    LCD_write_reg(ILI9225_HORIZONTAL_WINDOW_ADDR1, y1);
    LCD_write_reg(ILI9225_HORIZONTAL_WINDOW_ADDR2, y0);
    LCD_write_reg(ILI9225_VERTICAL_WINDOW_ADDR1, 219 - x0);
    LCD_write_reg(ILI9225_VERTICAL_WINDOW_ADDR2, 219 - x1);

    // set RAM address to start of window (H=y0 increments, V=219-x0 decrements)
    LCD_write_reg(ILI9225_RAM_ADDR_SET1, y0);
    LCD_write_reg(ILI9225_RAM_ADDR_SET2, 219 - x0);
}

void LCD_fill_screen(uint16_t color) {
    LCD_set_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

    LCD_start_gram_write();
    for (uint32_t i = 0; i < (uint32_t)LCD_WIDTH * LCD_HEIGHT; i++) {
        LCD_write_data16(color);
    }
    LCD_unselect();
}

void LCD_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT) return;

    // landscape 180°: x -> vertical (inverted), y -> horizontal
    LCD_write_reg(ILI9225_RAM_ADDR_SET1, y);
    LCD_write_reg(ILI9225_RAM_ADDR_SET2, 219 - x);

    LCD_start_gram_write();
    LCD_write_data16(color);
    LCD_unselect();
}

void LCD_draw_char(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg) {
    if (c < 32 || c > 126) c = '?';

    const uint8_t* glyph = font5x7[c - 32];

    // draw character pixel by pixel (6x7: 5 cols + 1 spacing)
    for (uint8_t row = 0; row < 7; row++) {
        for (uint8_t col = 0; col < 6; col++) {
            uint16_t px_color;
            if (col < 5 && (glyph[col] & (1 << row))) {
                px_color = color;
            } else {
                px_color = bg;
            }
            LCD_draw_pixel(x + col, y + row, px_color);
        }
    }
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
            cursor_y = 0;
        }
        LCD_draw_char(cursor_x, cursor_y, *str, color, bg);
        cursor_x += 6;
        str++;
    }
}

void LCD_draw_picture(const uint16_t* pixel_data){
    LCD_set_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

    LCD_start_gram_write();
    // HAL_SPI_Transmit size is uint16_t (max 65535), so send in chunks
    uint32_t total_bytes = (uint32_t)LCD_WIDTH * LCD_HEIGHT * 2;
    const uint8_t* ptr = (const uint8_t*)pixel_data;
    while (total_bytes > 0) {
        uint16_t chunk = (total_bytes > 65534) ? 65534 : (uint16_t)total_bytes;
        HAL_SPI_Transmit(lcd_config.hspi, (uint8_t*)ptr, chunk, HAL_MAX_DELAY);
        ptr += chunk;
        total_bytes -= chunk;
    }
    LCD_unselect();
}

void LCD_draw_region(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, const uint16_t* data) {
    LCD_set_window(x0, y0, x1, y1);

    LCD_start_gram_write();
    uint32_t total_bytes = (uint32_t)(x1 - x0 + 1) * (y1 - y0 + 1) * 2;
    const uint8_t* ptr = (const uint8_t*)data;
    while (total_bytes > 0) {
        uint16_t chunk = (total_bytes > 65534) ? 65534 : (uint16_t)total_bytes;
        HAL_SPI_Transmit(lcd_config.hspi, (uint8_t*)ptr, chunk, HAL_MAX_DELAY);
        ptr += chunk;
        total_bytes -= chunk;
    }
    LCD_unselect();
}

void LCD_draw_image(const tImage* img) {
    LCD_set_window(0, 0, img->width - 1, img->height - 1);

    LCD_start_gram_write();
    uint32_t total_pixels = (uint32_t)img->width * img->height;
    const uint8_t* src = img->data;
    for (uint32_t i = 0; i < total_pixels; i++) {
        // convert RGB888 to RGB565
        uint8_t r = src[i * 3];
        uint8_t g = src[i * 3 + 1];
        uint8_t b = src[i * 3 + 2];
        uint16_t color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
        LCD_write_data16(color);
    }
    LCD_unselect();
}

void LCD_clear_screen(uint16_t color){
    LCD_select();
    uint16_t swapped = LCD_SWAP16(color);
    for (uint16_t y = 0; y < LCD_HEIGHT; y++) {
        for (uint16_t x = 0; x < LCD_WIDTH; x++) {
            plot_buffer[y][x] = swapped;
        }
    }
    // set up window and start GRAM write, then send entire buffer
    LCD_set_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    LCD_start_gram_write();

    uint32_t total_bytes = (uint32_t)LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t);
    const uint8_t* ptr = (const uint8_t*)plot_buffer;
    while (total_bytes > 0) {
        uint16_t chunk = (total_bytes > 65534) ? 65534 : (uint16_t)total_bytes;
        HAL_SPI_Transmit(lcd_config.hspi, (uint8_t*)ptr, chunk, HAL_MAX_DELAY);
        ptr += chunk;
        total_bytes -= chunk;
    }
    LCD_unselect();
}
