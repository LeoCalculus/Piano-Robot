#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <ILI9341.h>
#include <hc04bt.h>

extern LCD_Config lcd_config; // should be defined in main.c at the beginning of the program
extern uint8_t rx_buffer[128]; // receive buffer from BT
extern uint8_t DMA_target_location[128];


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