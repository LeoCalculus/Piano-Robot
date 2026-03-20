#ifndef __ENCODER_H
#define __ENCODER_H

#include <tim.h>
#include <constants.h>
#include <stdint.h>

#define ENCODER_CPR 64 // 16 CPR * 4 (quadrature mode counts all edges)
#define GEAR_RATIO 6.25f
#define REV_LENGTH 73.0f

// enable the encoder - in initialization stage
void encoder_start(TIM_HandleTypeDef* tim);

// read encoder value
int16_t encoder_read_value(TIM_HandleTypeDef* tim);

// using encoder value and other parameter to find the distance travelled
float encoder_parse_distance_mm(int32_t encoder_count);

// based on distance value find the velocity (need to be careful about direction!)
// dt according to controller
float encoder_calc_speed_mm_s(float new_read_distance, float dt);


#endif