#ifndef __HC05BT_H
#define __HC05BT_H

#include <string.h>
#include <stdint.h>
#include <string.h>

// hc05 using uart1 integrated with command system

extern uint8_t rx_data[100];
extern uint8_t rx_byte;
extern volatile uint8_t rx_complete;  // 1 when message ready

/* Binary mode for file transfer */
extern volatile uint8_t rx_binary_mode;
extern volatile uint16_t rx_binary_expected;
extern uint8_t rx_binary_buffer[80];
extern volatile uint16_t rx_binary_index;
extern volatile uint8_t rx_binary_complete;

/* DMA receive buffer */
extern uint8_t rx_dma_buffer[128];
extern volatile uint16_t rx_dma_len;

// HC-05 is configured running with 115200 baudrate, 1 stop bit and 0 parity.

// set up functions
int setupBT();

// runtime functions
int HC05_SendInfo(uint8_t* buffer);
void HC05_ReceiveInfo(uint8_t* receiveBuffer);
void HC05_RestartDMA(void);
int HC05_SendBytes(uint8_t* buffer, uint16_t len);      // Blocking (for file transfer)
int HC05_SendBytes_IT(uint8_t* buffer, uint16_t len);   // Non-blocking (for general use)

#endif