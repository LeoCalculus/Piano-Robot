#ifndef __USER_UART_ISR_H
#define __USER_UART_ISR_H

#include "main.h"

// UART call back function 
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif 
