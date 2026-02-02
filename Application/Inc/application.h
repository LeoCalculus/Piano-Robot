#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <ILI9341.h>
#include <hc04bt.h>
#include <encoder.h>
#include <tim.h>
#include <command.h>

#define VOFA_TAIL {0x00, 0x00, 0x80, 0x7f}
#define MAX_CHORD_NOTES   10 // number of notes to play at one time, fixed value, 5 for left hand and 5 for right hand
#define MAX_CHORD_EVENTS  100 // this define how many chords it will play, not fixed value
#define PIANO_THRESHOLD   54.0f // define where the boundary of left hand and right hand is
#define LEFT_HAND_SIZE    10.0f // size of left hand
#define RIGHT_HAND_SIZE   10.0f // size of right hand


typedef struct __attribute__((packed)) VOFA_REPORT{
    float val[10]; // used + reserved
    unsigned char vofaTail[4];
}VOFA_REPORT;

extern LCD_Config lcd_config; // should be defined in main.c at the beginning of the program
extern uint8_t rx_buffer[128]; // receive buffer from BT
extern uint8_t DMA_target_location[128];
extern int32_t encoder_read_result;
extern float encoder_old_position_cm;
extern float target_position_cm;
extern volatile float current_velocity_cm_s;  // updated in ISR, read in main loop
extern volatile float current_distance_cm;    // updated in ISR, read in main loop
extern VOFA_REPORT vofa; // transmit via usb
extern volatile uint32_t time_counter;
extern volatile int is_moving; // 0 not moving and 1 is moving
extern volatile int is_blocked;

// pid struct - used for controller
typedef struct {
    const float Kp;
    const float Ki;
    const float Kd;
    const float Integral_max; // avoid too big for integral results
    float Integral; // integral should be bounded by integral max
    float last_error;
    float output;
} PID_t;

// chord struct - used to store the song, note, this part of memory might need to be modified, so DO NOT USE CONST 
typedef struct ChordEvent {
    uint8_t num_notes; // tells how many notes will be used
    float positions[MAX_CHORD_NOTES]; // an array contain position value used for solenoid control, POSITION SHOULD BE NONE ZEROS
    uint16_t duration_ms; // how long it needs to be pressed
    uint16_t delay_to_next_ms; // to next postions how many time left, used for determine how fast I need to move to next position
} ChordEvent_t;

typedef struct Song {
    ChordEvent_t event;
} Song_t;

extern Song_t piano_song[MAX_CHORD_EVENTS];

// find out left hand move or right hand move: 0 for left hand and 1 for right hand and 2 for both hand, 3 for no hands 
int which_hand(ChordEvent_t* this_note);

// locate hand: return the hand position -> set as target position, we need to return both hand position
void locate_hand(ChordEvent_t* this_note, float hand_pos[2], int which_hand);

// traversal the song
void traversal_song(Song_t* entire_song);

// handle pid control per controller cycle
void pid_cycle(PID_t* target_pid, float error, const float dt);

// initialize the controller
void controller_init();

// controller run at 1000Hz
void controller_step(const float dt);

// non-block wait function (using interrupt)
void wait_ms(uint32_t ms);



#endif