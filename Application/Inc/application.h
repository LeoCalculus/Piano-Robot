#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <hc04.h>
#include <ILI9225.h>
#include <sd.h>
#include <file_transfer.h>
#include <menu.h>
#include <command.h>
#include <config.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// EXTERN VARIABLES
// USART1 - HC04
extern uint8_t rx_message_buffer[128];
extern uint8_t rx_message[256]; // accumulation buffer, larger than DMA buffer to handle wraparound
extern uint16_t rx_message_index; // current write position in rx_message
extern int old_buffer_index; // last read position in DMA buffer
extern int rx_complete; // flag: complete message received (ended with \n)
extern int rx_valid;
extern volatile int uart_binary_mode; // 1 = skip \n detection (binary file transfer)
// SPI - LCD - ILI9225
extern LCD_Config lcd_config; 
// FILE TRANSFER
#define VOFA_TAIL {0x00, 0x00, 0x80, 0x7f}
#define MAX_CHORD_NOTES   10 // number of notes to play at one time, fixed value, 5 for left hand and 5 for right hand
#define MAX_CHORD_EVENTS  365 // max chords in a song (matches song_ram rows)
#define PIANO_THRESHOLD   54.0f // define where the boundary of left hand and right hand is
#define LEFT_HAND_SIZE    10.0f // size of left hand
#define RIGHT_HAND_SIZE   10.0f // size of right hand
#define RAM_SONG_MAX_BYTES (365 * 14 * 4) // 20440 bytes max
// VOFA
typedef struct __attribute__((packed)) VOFA_REPORT{
    float val[10]; // used + reserved
    unsigned char vofaTail[4];
}VOFA_REPORT;
// RAM song storage:
extern float song_ram[365][14]; // 365 chords with 14 values for each chord, used for transmit song to RAM
extern volatile int redirect_to_ram; // flag to indicate whether the incoming song data should be written to RAM
extern volatile int ram_rx_started;  // 1 = start sentinel received, accumulating
extern volatile int ram_rx_complete; // 1 = end sentinel received, ready to parse
extern volatile uint32_t ram_rx_offset; // byte offset into song_ram during accumulation
__ALIGN_BEGIN extern float rx_data[5500] __ALIGN_END;

// chord struct - used to store how the song is played
typedef struct ChordEvent {
    float positions[2]; // target position for both left hand and right hand
    bool pressed[MAX_CHORD_NOTES]; // True -> pressed, False -> not pressed
    uint16_t duration_ms; // how long it needs to be pressed
    uint16_t delay_to_next_ms; // to next postions how many time left
} ChordEvent_t;

extern ChordEvent_t chord_events[MAX_CHORD_EVENTS]; // store the whole song, max 100 chords
extern VOFA_REPORT vofa;

// parse flat float buffer (song_ram) into ChordEvent_t array
void parsing_song_buffer_to_struct(float src[][14], ChordEvent_t *dst);

// run homing procedure for the robot
void homing_procedure(void);

#endif /* __APPLICATION_H */