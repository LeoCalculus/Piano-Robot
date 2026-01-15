#include <hc05bt.h>
#include "usart.h"

uint8_t rx_data[100] = {0};
uint8_t rx_byte;
volatile uint8_t rx_complete = 0;  // flag: 1 when message ready

// set up functions, work with baudrate 38400
int setupBT(){
    // reset baudrate
    uint8_t cmd[] = "AT+UART=115200,0,0\r\n";
    uint8_t buffer[16] = {0};
    HAL_UART_Transmit(&huart1, cmd, strlen((char*)cmd), 100);
    // receive response
    HAL_StatusTypeDef status = HAL_UART_Receive(&huart1, buffer, 4, 1000);
    // use strncmp to compare strings and find "OK"
    if (status != HAL_OK || strncmp((char*)buffer, "OK", 2) != 0){
        return 0;
    } 

    // set PIN (do this BEFORE changing baud rate!)
    uint8_t cmd2[] = "AT+PSWD=6666\r\n";  // fixed typo: PSWD not PSDW
    uint8_t buffer2[16] = {0};
    HAL_UART_Transmit(&huart1, cmd2, strlen((char*)cmd2), 100);
    // receive response
    HAL_StatusTypeDef status2 = HAL_UART_Receive(&huart1, buffer2, 4, 1000);
    // use strncmp to compare strings and find "OK"
    if (status2 != HAL_OK || strncmp((char*)buffer2, "OK", 2) != 0){  // fixed: buffer2
        return 0;
    }    
    return 1; // all set done!
}

// runtime function will use interrupt
// use interrupt to send
// expect input a string
int HC05_SendInfo(uint8_t* buffer){
    HAL_UART_StateTypeDef state = HAL_UART_GetState(&huart1);
    // allow transmit when idle OR when only receiving (not when already transmitting)
    if(state == HAL_UART_STATE_READY || state == HAL_UART_STATE_BUSY_RX){
        HAL_UART_Transmit_IT(&huart1, buffer, strlen((char*)buffer));
        return 1;
    }

    return 0; // busy transmitting
}

void HC05_ReceiveInfo(uint8_t* receiveBuffer){
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1); // trigger interrupt when one byte is fetched!

}
