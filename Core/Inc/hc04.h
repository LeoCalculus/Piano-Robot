#ifndef __HC04_H
#define __HC04_H

#include "main.h"
#include <stdint.h>

void BT_SendString_DMA(const char *str);
void BT_Printf_DMA(const char *format, ...);
void BT_VOFA_JustFloat_Send(float d1, float d2, float d3);
void BT_SetBaudRate_115200(UART_HandleTypeDef *huart);

#endif
