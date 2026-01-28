#include <application.h>
#include <encoder.h>

uint8_t rx_buffer[128];
uint8_t DMA_target_location[128];
int32_t encoder_read_result;
float encoder_old_position_cm;
float target_position_cm;
volatile float current_velocity_cm_s;  // updated in ISR, read in main loop
volatile float current_distance_cm;    // updated in ISR, read in main loop
VOFA_REPORT vofa; // transmit via usb


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
    .Kp = 10.0f,
    .Ki = 20.0f,
    .Kd = 0.001f,
    .Integral_max = 0.01f
};

// do controller here
void controller_init(void){
    memset(&vofa, 0, sizeof(vofa));
    vofa.vofaTail[0] = 0x00;
    vofa.vofaTail[1] = 0x00;
    vofa.vofaTail[2] = 0x80;
    vofa.vofaTail[3] = 0x7f;

    // initialize PID value
    target_position_cm = 0.0f;
    debug_motor.Integral = 0.0f;
    debug_motor.last_error = 0.0f;
    debug_motor.output = 0.0f;

    // initialize PWM outputs to 0 - no movement at startup
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
}

// DO NOT USE SPRINT, PRINT IN THIS FUNCTION
// what have done: make some variable global and send
void controller_step(const float dt){
    uint32_t direction;

    encoder_read_value(&htim8, &encoder_read_result, &direction);
    current_distance_cm = encoder_parse_distance_cm(encoder_read_result);
    vofa.val[0] = current_distance_cm;
    current_velocity_cm_s = encoder_calc_speed_cm_s(current_distance_cm, dt);

    // position control - test pid - using step input - input via bluetooth
    float error = target_position_cm - current_distance_cm;
    vofa.val[1] = target_position_cm;

    // deadband: if error is within tolerance, stop motor and reset integral
    const float ERROR_DEADBAND = 0.1f; 
    const float PWM_MIN = 30.0f;        // minimum PWM to actually move motor
    float pwm_output = 0.0f;

    if (error > -ERROR_DEADBAND && error < ERROR_DEADBAND) {
        // close enough - stop motor
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
        debug_motor.Integral = 0.0f;  // reset integral to prevent windup
        debug_motor.output = 0.0f;
    } else {
        pid_cycle(&debug_motor, error, dt);

        // convert pid output to pwm, clamp to valid range
        pwm_output = debug_motor.output;
        if (pwm_output > 99.0f) pwm_output = 99.0f;
        if (pwm_output < -99.0f) pwm_output = -99.0f;

        // direction based on error -> check error so it knows where to move
        if (error > 0.0f) {
            uint32_t pwm = (pwm_output < PWM_MIN) ? (uint32_t)PWM_MIN : (uint32_t)pwm_output;
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwm);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
        } else {
            uint32_t pwm = (-pwm_output < PWM_MIN) ? (uint32_t)PWM_MIN : (uint32_t)(-pwm_output);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pwm);
        }
    }

    vofa.val[2] = pwm_output;
    vofa.val[3] = error;

    if (huart1.gState == HAL_UART_STATE_READY) {
        HAL_UART_Transmit_IT(&huart1, (uint8_t*)&vofa, sizeof(vofa));
    }
}