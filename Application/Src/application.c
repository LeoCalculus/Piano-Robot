#include <application.h>

// uart - buffer
uint8_t rx_message_buffer[128] = {0}; // DMA circular buffer
uint8_t rx_message[256] = {0}; // accumulation buffer for complete messages
uint16_t rx_message_index = 0; // current write position in rx_message
int old_buffer_index = 0;
int rx_complete = 0;
int rx_valid = 0;
volatile int uart_binary_mode = 0; // 1 = skip \n detection (binary file transfer)

// ram
volatile int redirect_to_ram = 0; // flag to indicate whether the incoming song data should be written to RAM
volatile int ram_rx_started = 0;  // 1 = start sentinel received, accumulating
volatile int ram_rx_complete = 0; // 1 = end sentinel received, ready to parse
volatile uint32_t ram_rx_offset = 0; // byte offset into song_ram during accumulation
__ALIGN_BEGIN float rx_data[5500] __ALIGN_END;
__ALIGN_BEGIN float song_ram[365][14] __ALIGN_END; // receive buffer for RAM song transfer (20440 bytes)
VOFA_REPORT vofa; // transmit via usb

// song struct
ChordEvent_t chord_events[MAX_CHORD_EVENTS] = {0}; // store the whole song, max 100 chords

// encoder global variables
volatile int32_t encoder_count = 0;
volatile float encoder_old_position_mm = 0.0f; // for speed calculation
volatile float encoder_speed_mm_s = 0.0f; // current speed in mm/s
int32_t encoder_read_result = 0;
uint32_t encoder_direction = 0;
volatile float current_distance_mm = 0.0f;
volatile float target_position_mm = 0.0f;

// other variables
volatile int32_t counter = 0;

// PID parameters
PID_t left_motor = {
    .Kp = 5.0f,
    .Ki = 0.01f,
    .Kd = 0.13f, //0.001
    .Integral_max = 0.01f // 0.5
};

void pid_cycle(PID_t* target_system, float error, const float dt){
    float derivative = (error - target_system->last_error) / dt;
    
    target_system->Integral += error*dt;
    // bound the integral
    if (target_system->Integral > target_system->Integral_max){
        target_system->Integral = target_system->Integral_max;
    } else if (target_system->Integral < -target_system->Integral_max) {
        target_system->Integral = -target_system->Integral_max;
    }
    target_system->output = target_system->Kp * error + target_system->Ki * target_system->Integral + target_system->Kd * derivative;
    target_system->last_error = error;
}


void parsing_song_buffer_to_struct(float src[][14], ChordEvent_t *dst) {
    for (int i = 0; i < MAX_CHORD_EVENTS; i++) {
        dst[i].positions[0] = src[i][0];
        dst[i].positions[1] = src[i][1];
        for (int j = 0; j < MAX_CHORD_NOTES; j++) {
            dst[i].pressed[j] = (src[i][2 + j] != 0.0f);
        }
        dst[i].duration_ms = (uint16_t)src[i][12];
        dst[i].delay_to_next_ms = (uint16_t)src[i][13];
    }
}

void homing_procedure(void) {
    // TODO: implement homing procedure
}

void controller_init(void) {
    memset(&vofa, 0, sizeof(vofa));
    vofa.vofaTail[0] = 0x00;
    vofa.vofaTail[1] = 0x00;
    vofa.vofaTail[2] = 0x80;
    vofa.vofaTail[3] = 0x7f;
}

void controller_step(const float dt) {
#ifndef USECONTROLLER
    // read the encoder 
    uint32_t direction;

    encoder_read_value(&htim3, &encoder_read_result, &direction);
    current_distance_mm = encoder_parse_distance_mm(encoder_read_result);
    vofa.val[0] = current_distance_mm;

    // pid implementation
    float motor_left_error = target_position_mm - current_distance_mm;
    motor_left_error = 0.0f; // disable the controller
    pid_cycle(&left_motor, motor_left_error, dt);

    float pwm_output = 0.0f;

    if (motor_left_error > -ERROR_DEADBAND && motor_left_error < ERROR_DEADBAND) {
        // close enough - stop motor
        // controller debug using 0 and 1000 
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 500);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 900);
        left_motor.Integral = 0.0f;  // reset integral to prevent windup
        left_motor.output = 0.0f;
    } else {
        pid_cycle(&left_motor, motor_left_error, dt);

        // convert pid output to pwm, clamp to valid range 0-999
        pwm_output = left_motor.output;
        if (pwm_output > 999.0f) pwm_output = 999.0f;
        if (pwm_output < 0.0f) pwm_output = 0.0f;

        // direction based on error -> check error so it knows where to move
        if (motor_left_error > ERROR_DEADBAND) {
            uint32_t pwm = (pwm_output < PWM_MIN_LEFT) ? (uint32_t)PWM_MIN_LEFT : (uint32_t)pwm_output;
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwm);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
        } else if (motor_left_error < -ERROR_DEADBAND) {
            uint32_t pwm = (-pwm_output < PWM_MIN_RIGHT) ? (uint32_t)PWM_MIN_RIGHT : (uint32_t)(-pwm_output);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwm);
        }
    }


    HAL_UART_Transmit_DMA(&huart1, (uint8_t*)vofa.val, sizeof(vofa.val));
#endif
}
