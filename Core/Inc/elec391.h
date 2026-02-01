#ifndef __ELEC391_h
#define __ELEC391_h

#include "main.h"
#include <stdint.h>

#ifdef GLOBAL_DEFINITION
    #define __EXTERN 
#else
    #define __EXTERN extern
#endif

#ifndef FRAME_RATE
 #define FRAME_RATE 50  // in ms
#endif

#ifndef MEASURE_RATE
 #define MEASURE_RATE 10  // in ms
#endif

#ifndef BT_RX_LEN
 #define BT_RX_LEN 16  
#endif

#ifndef COM_RX_LEN
 #define COM_RX_LEN 64  
#endif

#ifndef NUM_LYRICS_LINE
    #define NUM_LYRICS_LINE 6  
#endif

__EXTERN float vofa_buf[7]; 

__EXTERN uint8_t BT_DMA_rx_buff[BT_RX_LEN]; 
__EXTERN uint8_t BT_cmd_type;

__EXTERN uint8_t COM_DMA_rx_buff[BT_RX_LEN]; 
__EXTERN uint8_t COM_cmd_type;

__EXTERN uint8_t msec_count_frame;
__EXTERN uint8_t half_msec_count_measure;
__EXTERN uint8_t msec_count_limit;

__EXTERN uint8_t main_update;
__EXTERN uint8_t limit_count_en;

__EXTERN int32_t speed_10000;
__EXTERN int32_t rad_s_10000;
__EXTERN int32_t counter_acc;

__EXTERN int32_t location_1000;
__EXTERN int32_t err_prev_1000;
__EXTERN int32_t err_acc_1000;
__EXTERN int32_t err_1000;
__EXTERN int32_t target_1000;

__EXTERN float Kp;
__EXTERN float Ki;
__EXTERN float Kd;

#endif
