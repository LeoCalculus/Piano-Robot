#ifndef __ILI9341_H
#define __ILI9341_H

#include "gpio.h"
#include <stdint.h>
#include <spi.h>

#define SELECT_LCD() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET)
#define UNSELECT_LCD() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET)
#define CMD_MODE() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET)
#define DATA_MODE() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET)
#define LCD_RST_LOW() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET)
#define LCD_RST_HIGH() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET)

// display size (default portrait)
#define LCD_WIDTH  240
#define LCD_HEIGHT 500  // try 400 if 320 doesn't fill screen, some displays are taller

// GRAM offset - some displays need this (try 0, 40, 80 if display is misaligned)
#define LCD_X_OFFSET 0
#define LCD_Y_OFFSET 0

// common colors (RGB565)
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0
#define COLOR_CYAN    0x07FF
#define COLOR_MAGENTA 0xF81F

void LcdInit(void);
void LcdWriteCmd(uint8_t cmd);
void LcdWriteData(uint8_t data);
void LcdSetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void LcdFillScreen(uint16_t color);
void LcdDrawPixel(uint16_t x, uint16_t y, uint16_t color);
void LcdDrawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg);
void LcdDrawString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg);
void LcdSetCursor(uint16_t x, uint16_t y);
void LcdPrint(const char *str, uint16_t color, uint16_t bg);  // auto advance cursor

#endif