#include <packet.h>
#include <usart.h>
#include <string.h>
#include "stm32h5xx_hal.h"
#include "stm32h5xx_hal_uart.h"

static uint8_t pkt_tx_buf[PKT_MAX_PAYLOAD + 5]; /* sync1 + sync2 + type + len + payload + xor */

static uint8_t xor_checksum(const uint8_t *data, uint16_t len)
{
    uint8_t x = 0;
    for (uint16_t i = 0; i < len; i++)
        x ^= data[i];
    return x;
}

int pkt_send(uint8_t type, const uint8_t *payload, uint8_t len)
{
    uint32_t t0;
    if (len > PKT_MAX_PAYLOAD) return -1;

    /* Wait for any ongoing DMA transmit to finish before polling TX */
    t0 = HAL_GetTick();
    while (huart2.gState != HAL_UART_STATE_READY) {
        if (HAL_GetTick() - t0 > 50) return -1; /* timeout safety */
    }

    pkt_tx_buf[0] = PKT_SYNC1;
    pkt_tx_buf[1] = PKT_SYNC2;
    pkt_tx_buf[2] = type;
    pkt_tx_buf[3] = len;
    if (len > 0 && payload != NULL)
        memcpy(&pkt_tx_buf[4], payload, len);
    pkt_tx_buf[4 + len] = xor_checksum(&pkt_tx_buf[2], 2 + len);

    HAL_UART_Transmit(&huart2, pkt_tx_buf, 5 + len, 20);
    return 0;
}

void pkt_send_ui_state(uint8_t state, uint8_t index, uint8_t extra1, uint8_t extra2)
{
    uint8_t payload[4] = {state, index, extra1, extra2};
    pkt_send(PKT_TYPE_UI_STATE, payload, 4);
}

void pkt_send_heartbeat(void)
{
    pkt_send(PKT_TYPE_HEARTBEAT, NULL, 0);
}

void pkt_send_file_entry(const char *name, uint32_t size)
{
    uint8_t payload[17];
    memset(payload, 0, sizeof(payload));
    strncpy((char *)payload, name, 12);
    memcpy(&payload[13], &size, 4);
    pkt_send(PKT_TYPE_FILE_ENTRY, payload, 17);
}

void pkt_send_file_list_end(uint8_t count)
{
    pkt_send(PKT_TYPE_FILE_LIST_END, &count, 1);
}

void pkt_send_play_status(uint8_t state, uint16_t current, uint16_t total)
{
    uint8_t payload[5];
    payload[0] = state;
    memcpy(&payload[1], &current, 2);
    memcpy(&payload[3], &total, 2);
    pkt_send(PKT_TYPE_PLAY_STATUS, payload, 5);
}

void pkt_send_transfer_status(uint8_t status)
{
    pkt_send(PKT_TYPE_TRANSFER_STATUS, &status, 1);
}
