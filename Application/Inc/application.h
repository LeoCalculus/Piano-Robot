#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <hc04.h>
#include <ILI9225.h>
#include <sd.h>
#include <file_transfer.h>
#include <menu.h>
#include <command.h>
#include <config.h>
#include <constants.h>
#include <encoder.h>
#include <protection.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stm32h5xx_hal_tim.h>
#include <usart.h>
#include <debug_song.h>

// ================================== DEFINES =========================================
#define ERROR_DEADBAND 1.2f
#define INT_ON 50.0f
#define I_ON_ZONE 2.0f
#define PWM_MIN_LEFT 300
#define PWM_MIN_RIGHT 700

// EXTERN VARIABLES
// =============================== USART2 - HC04 =======================================
extern uint8_t rx_message_buffer[128];
extern uint8_t rx_message[256]; // accumulation buffer, larger than DMA buffer to handle wraparound
extern uint16_t rx_message_index; // current write position in rx_message
extern int old_buffer_index; // last read position in DMA buffer
extern int rx_complete; // flag: complete message received (ended with \n)
extern int rx_valid;
extern volatile int uart_binary_mode; // 1 = skip \n detection (binary file transfer)

// ============================= SPI - LCD - ILI9225 ===================================
extern LCD_Config lcd_config; 

// =============================== FILE TRANSFER =======================================
#define VOFA_TAIL {0x00, 0x00, 0x80, 0x7f}
#define MAX_CHORD_NOTES   10 // number of notes to play at one time, fixed value, 5 for left hand and 5 for right hand
#define MAX_CHORD_EVENTS  365 // max chords in a song
#define PIANO_THRESHOLD   54.0f // define where the boundary of left hand and right hand is
#define LEFT_HAND_SIZE    10.0f // size of left hand
#define RIGHT_HAND_SIZE   10.0f // size of right hand

// ================================= VOFA ==============================================
typedef struct __attribute__((packed)) VOFA_REPORT{
    float val[10]; // used + reserved
    unsigned char vofaTail[4];
}VOFA_REPORT;

// ============================= RAM song storage ======================================
extern volatile int redirect_to_ram; // flag to indicate whether the incoming song data should be written to RAM
extern volatile int ram_rx_started;  // 1 = start sentinel received, accumulating
extern volatile int ram_rx_complete; // 1 = end sentinel received, ready to parse
extern volatile uint32_t ram_rx_offset; // row count received during accumulation
__ALIGN_BEGIN extern float rx_data[5500] __ALIGN_END;

// ============================= ENCODER - TIMER - AB phase ============================
extern volatile int32_t encoder_count;
extern volatile float encoder_old_position_mm; // for speed calculation
extern volatile float encoder_speed_mm_s; // current speed in mm/s
extern int32_t encoder_read_result;
extern uint32_t encoder_direction;
extern volatile float current_distance_mm;
extern volatile int32_t counter; // general purpose counter for testing

// ========================== chord struct - used to store how the song is played ===========================
typedef struct ChordEvent {
    float positions[2]; // target position for both left hand and right hand
    bool pressed[MAX_CHORD_NOTES]; // True -> pressed, False -> not pressed
    uint16_t duration_ms; // how long it needs to be pressed
} ChordEvent_t;

// ========================================== Global variables =============================================
extern ChordEvent_t chord_events[MAX_CHORD_EVENTS]; // store the whole song, max 100 chords
extern VOFA_REPORT vofa;
extern volatile float target_position_mm; // for PID control, set by the song, read by the controller step function
extern int left_motor_arrived; // flag when enter the motor deadband 
extern int right_motor_arrived; // flag when enter the motor deadband

// ============================================= PID struct ===============================================
typedef struct {
    float Kp;
    float Ki;
    float Kd;
    const float integral_max; // avoid too big for integral results
    int32_t encoder_cnt;
    float target_pos;
    float current_pos;
    float integral; // integral should be bounded by integral max
    float current_error;
    float last_error;
    uint16_t output_pwm;
} PID_t;

// ======================================== Hand definitions ============================================
// left hand sols
#define GPIO_SOL_1_PORT GPIOB
#define GPIO_SOL_1_PIN GPIO_PIN_7

#define GPIO_SOL_2_PORT GPIOB
#define GPIO_SOL_2_PIN GPIO_PIN_8

#define GPIO_SOL_3_PORT GPIOB
#define GPIO_SOL_3_PIN GPIO_PIN_9

#define GPIO_SOL_4_PORT GPIOE
#define GPIO_SOL_4_PIN GPIO_PIN_0

#define GPIO_SOL_5_PORT GPIOE
#define GPIO_SOL_5_PIN GPIO_PIN_2

#define GPIO_SOL_6_PORT GPIOE
#define GPIO_SOL_6_PIN GPIO_PIN_3

#define GPIO_SOL_7_PORT GPIOE
#define GPIO_SOL_7_PIN GPIO_PIN_4

#define GPIO_SOL_8_PORT GPIOE
#define GPIO_SOL_8_PIN GPIO_PIN_5

// right hand sols
#define GPIO_SOL_9_PORT GPIOE
#define GPIO_SOL_9_PIN GPIO_PIN_6

#define GPIO_SOL_10_PORT GPIOC
#define GPIO_SOL_10_PIN GPIO_PIN_13

#define GPIO_SOL_11_PORT GPIOC
#define GPIO_SOL_11_PIN GPIO_PIN_14

#define GPIO_SOL_12_PORT GPIOC
#define GPIO_SOL_12_PIN GPIO_PIN_15

typedef struct Solenoid_t{
    GPIO_TypeDef* port;
    uint16_t pin;
} Solenoid_t;

static const Solenoid_t solenoids[MAX_CHORD_NOTES] = {
    {GPIO_SOL_1_PORT, GPIO_SOL_1_PIN}, // Index 0: left hand 1
    {GPIO_SOL_2_PORT, GPIO_SOL_2_PIN}, // Index 1
    {GPIO_SOL_3_PORT, GPIO_SOL_3_PIN}, // Index 2
    {GPIO_SOL_4_PORT, GPIO_SOL_4_PIN}, // Index 3
    {GPIO_SOL_5_PORT, GPIO_SOL_5_PIN}, // Index 4
    {GPIO_SOL_6_PORT, GPIO_SOL_6_PIN}, // Index 5
    {GPIO_SOL_7_PORT, GPIO_SOL_7_PIN}, // Index 6: left hand 7 
    {GPIO_SOL_9_PORT, GPIO_SOL_9_PIN}, // Index 7: right hand 1, skipped SOL 8
    {GPIO_SOL_10_PORT, GPIO_SOL_10_PIN},// Index 8
    {GPIO_SOL_11_PORT, GPIO_SOL_11_PIN} // Index 9: right hand 3
};


// REMAINING FUNCTIONS:
// target system is for the motor's struct, error = target - actual, dt = 1000Hz timer interrupt period
void pid_cycle(PID_t* target_system);

// run homing procedure for the robot
void homing_procedure(void);

// controller start up
void controller_init(void);

// controller step - in 1000Hz timer interrupt
void controller_step(const float dt);

#endif /* __APPLICATION_H */