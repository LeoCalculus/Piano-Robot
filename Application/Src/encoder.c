#include <encoder.h>
#include <application.h>

static uint16_t encoder_last_count = 0;
static int32_t encoder_accumulated = 0;

void encoder_start(TIM_HandleTypeDef* htim){
    HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);
    encoder_last_count = __HAL_TIM_GET_COUNTER(htim);
    encoder_accumulated = 0;
}

void encoder_read_value(TIM_HandleTypeDef* htim, int32_t* read_result, uint32_t* direction){
    uint16_t current_count = __HAL_TIM_GET_COUNTER(htim);
    // calculate delta as signed 16-bit to handle wraparound correctly
    int16_t delta = (int16_t)(current_count - encoder_last_count);
    encoder_accumulated += delta;
    encoder_last_count = current_count;

    *read_result = encoder_accumulated;
    // TIM_CR1_DIR is not defined in tim.h file, read from register directly
    *direction = (htim->Instance->CR1 >> 4) & 0x1;  // DIR bit is bit 4 in CR1
}

float encoder_parse_distance_cm(int32_t encoder_count){
    float resolution = (float)encoder_count / (ENCODER_CPR * GEAR_RATIO);
    float distance = resolution * (PI * WHEEL_DIAMETER_CM);
    return distance; // in cm
}

extern float encoder_old_position_cm;  // defined in application.c

float encoder_calc_speed_cm_s(float encoder_new_parsed_distance_cm, float dt){
    float change_in_distance = encoder_new_parsed_distance_cm - encoder_old_position_cm;
    float velocity = change_in_distance / dt;
    encoder_old_position_cm = encoder_new_parsed_distance_cm;  // update for next call
    return velocity;
}