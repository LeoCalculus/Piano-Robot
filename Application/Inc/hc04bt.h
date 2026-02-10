#ifndef __HC04BT_H
#define __HC04BT_H

#include <gpio.h>
#include <stdint.h>
#include <usart.h>

// HC04 Bluetooth configuration, using 115200 baud rate for both setting and normal usage.
void BT_config(UART_HandleTypeDef *huart);

// HC04 send strings, using interrupt mode
void BT_send_info(UART_HandleTypeDef *huart, uint8_t *info, uint16_t info_size);

// HC04 send raw bytes (blocking) - used by file transfer protocol
void HC04_SendBytes(uint8_t *data, uint16_t len);

// HC04 send string (blocking) - used by /listFiles command
void HC04_SendString(const char *str);

#endif