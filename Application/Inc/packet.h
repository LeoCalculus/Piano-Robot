#ifndef __PACKET_H
#define __PACKET_H

#include <stdint.h>

/* XOR Packet Protocol
 * Format: [SYNC1=0xAA] [SYNC2=0x55] [TYPE] [LEN] [PAYLOAD...] [XOR]
 * XOR = TYPE ^ LEN ^ PAYLOAD[0] ^ ... ^ PAYLOAD[LEN-1]
 */

#define PKT_SYNC1 0xAA
#define PKT_SYNC2 0x55
#define PKT_MAX_PAYLOAD 48

/* MCU -> PC packet types */
#define PKT_TYPE_UI_STATE        0x10  /* menu_state, menu_index, extra1, extra2 */
#define PKT_TYPE_HEARTBEAT       0x11  /* empty - BT alive check */
#define PKT_TYPE_FILE_ENTRY      0x12  /* filename[13] + size[4] = 17 bytes */
#define PKT_TYPE_FILE_LIST_END   0x13  /* count[1] */
#define PKT_TYPE_PLAY_STATUS     0x14  /* state[1], current[2], total[2] */
#define PKT_TYPE_TRANSFER_STATUS 0x15  /* status[1]: 0=begin, 1=complete, 2=error */

/* Send a generic XOR packet over BT (huart2).
 * Uses blocking HAL_UART_Transmit so it doesn't conflict with DMA VOFA.
 * Returns 0 on success, -1 on error. */
int pkt_send(uint8_t type, const uint8_t *payload, uint8_t len);

/* Convenience senders */
void pkt_send_ui_state(uint8_t state, uint8_t index, uint8_t extra1, uint8_t extra2);
void pkt_send_heartbeat(void);
void pkt_send_file_entry(const char *name, uint32_t size);
void pkt_send_file_list_end(uint8_t count);
void pkt_send_play_status(uint8_t state, uint16_t current, uint16_t total);
void pkt_send_transfer_status(uint8_t status);

#endif /* __PACKET_H */
