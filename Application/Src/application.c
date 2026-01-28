#include <application.h>
#include <encoder.h>

uint8_t rx_buffer[128];
uint8_t DMA_target_location[128];
int32_t encoder_read_result;
float encoder_old_position_cm;
float target_position_cm;
volatile float current_velocity_cm_s;  // updated in ISR, read in main loop
volatile float current_distance_cm;    // updated in ISR, read in main loop

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

// DO NOT USE SPRINT, PRINT IN THIS FUNCTION
void controller_step(const float dt){
    uint32_t direction;

    encoder_read_value(&htim8, &encoder_read_result, &direction);
    current_distance_cm = encoder_parse_distance_cm(encoder_read_result);
    current_velocity_cm_s = encoder_calc_speed_cm_s(current_distance_cm, dt);

    // position control - test pid - using step input - input via bluetooth
    float error = target_position_cm - current_distance_cm;
    pid_cycle(&debug_motor, error, dt); // new target will be stored in debug_motor->output

    // convert pid output to the pwm signal, units conversion are included in Kp
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, debug_motor.output);
}