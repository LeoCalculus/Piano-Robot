#include <application.h>

uint8_t rx_message_buffer[128] = {0}; // DMA circular buffer
uint8_t rx_message[256] = {0}; // accumulation buffer for complete messages
uint16_t rx_message_index = 0; // current write position in rx_message
int old_buffer_index = 0;
int rx_complete = 0;
int rx_valid = 0;
volatile int uart_binary_mode = 0; // 1 = skip \n detection (binary file transfer)

// ram
volatile int redirect_to_ram = 0; // flag to indicate whether the incoming song data should be written to RAM
volatile int ram_rx_started = 0;  // 1 = start sentinel received, accumulating
volatile int ram_rx_complete = 0; // 1 = end sentinel received, ready to parse
volatile uint32_t ram_rx_offset = 0; // byte offset into song_ram during accumulation
__ALIGN_BEGIN float rx_data[5500] __ALIGN_END;
__ALIGN_BEGIN float song_ram[365][14] __ALIGN_END; // receive buffer for RAM song transfer (20440 bytes)
VOFA_REPORT vofa; // transmit via usb

ChordEvent_t chord_events[MAX_CHORD_EVENTS] = {0}; // store the whole song, max 100 chords

void parsing_song_buffer_to_struct(float src[][14], ChordEvent_t *dst) {
    for (int i = 0; i < MAX_CHORD_EVENTS; i++) {
        dst[i].positions[0] = src[i][0];
        dst[i].positions[1] = src[i][1];
        for (int j = 0; j < MAX_CHORD_NOTES; j++) {
            dst[i].pressed[j] = (src[i][2 + j] != 0.0f);
        }
        dst[i].duration_ms = (uint16_t)src[i][12];
        dst[i].delay_to_next_ms = (uint16_t)src[i][13];
    }
}

void homing_procedure(void) {
    // TODO: implement homing procedure
}

