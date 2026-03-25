#include <hc04.h>

void hc04_send_char(uint8_t c){
    HAL_UART_Transmit_DMA(&huart2, &c, 1);
}

void hc04_send_string(uint8_t *str){
    HAL_UART_Transmit_DMA(&huart2, str, strlen((char*)str));
}

void hc04_send_bytes(uint8_t *data, uint16_t len){
    HAL_UART_Transmit_DMA(&huart2, data, len);
}

void hc04_receive_to_idle_init(UART_HandleTypeDef *huart, uint8_t *buffer, uint16_t buffer_size){
    HAL_UARTEx_ReceiveToIdle_DMA(huart, buffer, buffer_size);
    // HT and TC stay enabled — the callback accumulates until \n is found
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size){
    if(huart == &huart2){
        if (size == old_buffer_index) {
            return;
        }

        // find total length and traversal the message, stop until \n is found

        // find total message length
        uint16_t len;
        uint16_t start = old_buffer_index;
        if (size > start) {
            len = size - start;
        } else {
            // wraparound
            len = sizeof(rx_message_buffer) - start + size;
        }

        for (uint16_t i = 0; i < len; i++) {
            // indexing in DMA stored buffer with wraparound
            uint16_t buf_pos = (start + i) % sizeof(rx_message_buffer);
            uint8_t c = rx_message_buffer[buf_pos];

            if (!uart_binary_mode && c == '\n') {
                // complete message found (text mode only, skipped in binary mode)
                rx_message[rx_message_index] = '\0';
                rx_valid = rx_message_index; // store TOTAL accumulated length
                // overwrite the place by setting index back to 0
                rx_message_index = 0;
                rx_complete = 1;
                // update the old buffer index for next use, +1 due to \0
                old_buffer_index = (buf_pos + 1) % sizeof(rx_message_buffer);
                return;
            }

            // if cannot find \n it will just append the message to buffer, which bypass TC and HC partial write problem
            // it will stop usually where \n happens like idle interrupt
            if (rx_message_index < sizeof(rx_message) - 1) {
                rx_message[rx_message_index++] = c;
            }
        }
        old_buffer_index = size;
    }
}