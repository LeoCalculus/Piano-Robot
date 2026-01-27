#ifndef __ENCODER_H
#define __ENCODER_H

#include <tim.h>
#include <constants.h>

#define ENCODER_CPR 700 // should be gear box shaft
#define ENCODER_MULTIPLIER 1
#define WHEEL_DIAMETER_MM 30.0f

// enable the encoder - in initialization stage
void encoder_start(TIM_HandleTypeDef* tim);

// read encoder value
void encoder_read_value(TIM_HandleTypeDef* tim, int32_t* read_result, uint32_t* direction);

// using encoder value and other parameter to find the distance travelled
float encoder_parse_distance_mm(int32_t encoder_count);

// based on distance value find the velocity (need to be careful about direction!)
// dt according to controller
float encoder_calc_speed_mm_s(float new_read_distance, float dt);


#endif