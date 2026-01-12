#ifndef __OLED1315_H
#define __OLED1315_H

#include <stdint.h>
#include "i2c.h"

#define SSD1315_ADDR 0x78
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_PAGES (OLED_HEIGHT / 8)

void OLED_Init(void);
void OLED_Clear(void);
void OLED_SetCursor(uint8_t x, uint8_t page);
void OLED_WriteCommand(uint8_t cmd);
void OLED_WriteData(uint8_t data);
void OLED_WriteChar(char c);
void OLED_WriteString(const char *str);
void OLED_SetContrast(uint8_t contrast);

// 5x7 font (stored in flash)
extern const uint8_t OLED_Font5x7[][5];

#endif
