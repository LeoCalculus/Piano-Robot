#include <application.h>
#include <encoder.h>
#include <stdio.h>

uint8_t rx_buffer[128];
uint8_t DMA_target_location[128];
int32_t encoder_read_result;
float encoder_old_position_mm;
float target_position_mm;

void pid_cycle(PID_t* target_system, float error, const float dt){
    float derivative = (error - target_system->last_error) / dt;
    
    target_system->Integral += error*dt;
    // bound the integral
    if (target_system->Integral > target_system->Integral_max){
        target_system->Integral = target_system->Integral_max;
    } else if (target_system->Integral < -target_system->Integral_max) {
        target_system->Integral = -target_system->Integral_max;
    }
    // controller output here
    target_system->output = target_system->Kp * error + target_system->Ki * target_system->Integral + target_system->Kd * derivative;
    target_system->last_error = error;
}

// PID struct for the motor
PID_t debug_motor = {
    .Kp = 3.0f,
    .Ki = 1.0f,
    .Kd = 0.0f,
    .Integral_max = 0.01f
};

// do controller here
void controller_init(void){

}

void controller_step(const float dt){
    uint32_t direction;
    char lcd_buf[32]; // buffer to put string and data

    encoder_read_value(&htim8, &encoder_read_result, &direction);
    float current_distance_mm = encoder_parse_distance_mm(encoder_read_result);
    float velocity_mm_s = encoder_calc_speed_mm_s(current_distance_mm, dt);
    float error = 0.0f;

    sprintf(lcd_buf, "Dist: %.2f mm           ", current_distance_mm);
    LCD_draw_string(&lcd_config, 0, 11, lcd_buf, COLOR_BLACK, COLOR_WHITE);

    sprintf(lcd_buf, "Vel: %.2f mm/s          ", velocity_mm_s);
    LCD_draw_string(&lcd_config, 0, 12, lcd_buf, COLOR_BLACK, COLOR_WHITE);

    // position control - test pid - using step input - input via bluetooth
    error = target_position_mm - current_distance_mm;
    pid_cycle(&debug_motor, error, dt); // new target will be stored in debug_motor->output

    // convert pid output to the pwm signal, units conversion are included in Kp
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, debug_motor.output);
    
}