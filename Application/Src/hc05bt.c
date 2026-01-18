#include <hc05bt.h>
#include "usart.h"

uint8_t rx_data[100] = {0};
uint8_t rx_byte;
volatile uint8_t rx_complete = 0;  // flag: 1 when message ready

/* Binary mode for file transfer */
volatile uint8_t rx_binary_mode = 0;
volatile uint16_t rx_binary_expected = 0;
uint8_t rx_binary_buffer[80] = {0};
volatile uint16_t rx_binary_index = 0;
volatile uint8_t rx_binary_complete = 0;

/* DMA receive buffer */
uint8_t rx_dma_buffer[128] = {0};
volatile uint16_t rx_dma_len = 0;

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
    /* Use DMA with IDLE line detection */
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_dma_buffer, sizeof(rx_dma_buffer));
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
}

/* Restart DMA reception - abort first to ensure clean state */
void HC05_RestartDMA(void){
    /* Abort any ongoing reception first */
    HAL_UART_AbortReceive(&huart1);
    /* Restart DMA */
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rx_dma_buffer, sizeof(rx_dma_buffer));
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
}

int HC05_SendBytes(uint8_t* buffer, uint16_t len){
    /* Use blocking transmit for file transfer responses to ensure delivery */
    /* Timeout: 10ms per byte + 50ms base (safe margin at 115200 baud) */
    uint32_t timeout = 50 + (len * 10);
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, buffer, len, timeout);
    return (status == HAL_OK) ? 1 : 0;
}

/* Non-blocking version for general use */
int HC05_SendBytes_IT(uint8_t* buffer, uint16_t len){
    HAL_UART_StateTypeDef state = HAL_UART_GetState(&huart1);
    if(state == HAL_UART_STATE_READY || state == HAL_UART_STATE_BUSY_RX){
        HAL_UART_Transmit_IT(&huart1, buffer, len);
        return 1;
    }
    return 0;
}
