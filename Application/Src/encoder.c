#include <encoder.h>
#include <application.h>

void encoder_start(TIM_HandleTypeDef* htim){
    HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);
}

void encoder_read_value(TIM_HandleTypeDef* htim, int32_t* read_result, uint32_t* direction){
    *read_result = __HAL_TIM_GET_COUNTER(htim);
    // TIM_CR1_DIR is not defined in tim.h file, read from register directly
    *direction = (htim->Instance->CR1 >> 4) & 0x1;  // DIR bit is bit 4 in CR1
}

float encoder_parse_distance_mm(int32_t encoder_count){
    float resolution = (float)encoder_count / (ENCODER_CPR * ENCODER_MULTIPLIER);
    float distance = resolution * (PI * WHEEL_DIAMETER_MM);
    return distance;
}

extern float encoder_old_position_mm;  // defined in application.c

float encoder_calc_speed_mm_s(float encoder_new_parsed_distance_mm, float dt){
    float change_in_distance = encoder_new_parsed_distance_mm - encoder_old_position_mm;
    float velocity = change_in_distance / dt;
    encoder_old_position_mm = encoder_new_parsed_distance_mm;  // update for next call
    return velocity;
}