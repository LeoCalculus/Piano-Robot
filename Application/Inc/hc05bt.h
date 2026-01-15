#ifndef __HC05BT_H
#define __HC05BT_H

#include <string.h>
#include <stdint.h>
#include <string.h>

// hc05 using uart1 integrated with command system

extern uint8_t rx_data[100];
extern uint8_t rx_byte;
extern volatile uint8_t rx_complete;  // 1 when message ready

// HC-05 is configured running with 115200 baudrate, 1 stop bit and 0 parity.

// set up functions
int setupBT();

// runtime functions
int HC05_SendInfo(uint8_t* buffer);
void HC05_ReceiveInfo(uint8_t* receiveBuffer);

#endif