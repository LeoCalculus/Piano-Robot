#include <encoder.h>
#include <application.h>

static uint16_t encoder_last_count = 0;
static int32_t encoder_accumulated = 0;

void encoder_start(TIM_HandleTypeDef* htim){
    HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);
    encoder_last_count = __HAL_TIM_GET_COUNTER(htim);
    encoder_accumulated = 0;
}

int16_t encoder_read_value(TIM_HandleTypeDef* htim){
    uint16_t delta_count = __HAL_TIM_GET_COUNTER(htim);
    __HAL_TIM_SetCounter(htim, 0);
    return (int16_t) delta_count;
}

float encoder_parse_distance_mm(int32_t encoder_count){
    float revolution = (float) encoder_count / (ENCODER_CPR * GEAR_RATIO);
    float distance = revolution * REV_LENGTH;
    return distance; // in mm
}


float encoder_calc_speed_mm_s(float encoder_new_parsed_distance_mm, float dt){
    float change_in_distance = encoder_new_parsed_distance_mm - encoder_old_position_mm;
    float velocity = change_in_distance / dt;
    encoder_old_position_mm = encoder_new_parsed_distance_mm;  // update for next call
    return velocity;
}