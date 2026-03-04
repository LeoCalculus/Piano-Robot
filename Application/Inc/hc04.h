#ifndef __HC04_H
#define __HC04_H

// DOC: ALL MESSAGE NEEDS TO END WITH \n

#include <stdint.h>
#include <usart.h>
#include <string.h>
#include <application.h>
#include <stm32h5xx_hal_dma.h>

// Send char - non blocking DMA mode
void hc04_send_char(uint8_t c);

// Send string - non blocking DMA mode
void hc04_send_string(uint8_t *str);

// Send raw bytes - non blocking DMA mode
void hc04_send_bytes(uint8_t *data, uint16_t len);

// Receive to idle initialization - called only once
void hc04_receive_to_idle_init(UART_HandleTypeDef *huart, uint8_t *buffer, uint16_t buffer_size);

// Receive to idle callback function - this was previously defined as a weak function in stm32h5xx_hal_uart.c, but we are providing our own implementation here
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size);

#endif /* __HC04_H */