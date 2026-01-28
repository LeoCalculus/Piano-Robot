#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <ILI9341.h>
#include <hc04bt.h>
#include <encoder.h>
#include <tim.h>
#include <command.h>

#define VOFA_TAIL {0x00, 0x00, 0x80, 0x7f}

typedef struct __attribute__((packed)) VOFA_REPORT{
    float val[10];  // Only 3 channels for accel X/Y/Z
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

typedef struct {
    const float Kp;
    const float Ki;
    const float Kd;
    const float Integral_max; // avoid too big for integral results
    float Integral; // integral should be bounded by integral max
    float last_error;
    float output;
} PID_t;

void pid_cycle(PID_t* target_pid, float error, const float dt);
void controller_init();
void controller_step(const float dt);




#endif