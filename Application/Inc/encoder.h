#ifndef __ENCODER_H
#define __ENCODER_H

#include <tim.h>
#include <constants.h>
#include <stdint.h>

#define ENCODER_CPR 64 // 16 CPR * 4 (quadrature mode counts all edges)
#define GEAR_RATIO 43.8f
#define WHEEL_DIAMETER_CM 5.0f

// enable the encoder - in initialization stage
void encoder_start(TIM_HandleTypeDef* tim);

// read encoder value
void encoder_read_value(TIM_HandleTypeDef* tim, int32_t* read_result, uint32_t* direction);

// using encoder value and other parameter to find the distance travelled
float encoder_parse_distance_cm(int32_t encoder_count);

// based on distance value find the velocity (need to be careful about direction!)
// dt according to controller
float encoder_calc_speed_cm_s(float new_read_distance, float dt);


#endif