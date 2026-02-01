#include "user_uart_isr.h"
#include "main.h"
#include "hc04.h"
#include "usart.h"
#include <stdint.h>
#include "elec391.h"

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        if (Size < BT_RX_LEN) {
            BT_DMA_rx_buff[Size] = '\0';
        }

        if (BT_DMA_rx_buff[0] == ',') {
            BT_cmd_type = 1; 
        }
        else if (BT_DMA_rx_buff[0] == ';') {

            if(BT_DMA_rx_buff[1] == 'A') BT_cmd_type = 2; 
            else if(BT_DMA_rx_buff[1] == 'B') BT_cmd_type = 3;
            else if(BT_DMA_rx_buff[1] == 'C') BT_cmd_type = 4;
        }
        else {
            BT_cmd_type = 127; 
        }
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, BT_DMA_rx_buff, BT_RX_LEN);
    }

    else if (huart->Instance == USART2)
    {
        if (Size < COM_RX_LEN) {
            COM_DMA_rx_buff[Size] = '\0';
        }

        if (COM_DMA_rx_buff[0] == ',') {
            COM_cmd_type = 1; 
        }
        else if (COM_DMA_rx_buff[0] == ';') {

            if(COM_DMA_rx_buff[1] == 'A') COM_cmd_type = 2; 
            else if(COM_DMA_rx_buff[1] == 'B') COM_cmd_type = 3;
            else if(COM_DMA_rx_buff[1] == 'C') COM_cmd_type = 4;
            else if(COM_DMA_rx_buff[1] == 'D') COM_cmd_type = 5;
        }
        else {
            COM_cmd_type = 127; 
        }
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, COM_DMA_rx_buff, COM_RX_LEN);
    }
}
