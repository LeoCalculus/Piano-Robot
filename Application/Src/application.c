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
volatile uint32_t ram_rx_offset = 0; // row count received during accumulation
__ALIGN_BEGIN float rx_data[5500] __ALIGN_END;
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
volatile int stop_playing = 0; // flag to indicate whether to stop playing the song, set by :q command

// motor motion control parameters
int left_motor_arrived = 0; // flag when enter the motor deadband 
int right_motor_arrived = 0; // flag when enter the motor deadband

// PID parameters
PID_t left_motor = {
    .Kp = 0.085f,
    .Ki = 0.015f,
    .Kd = 3.2f, //0.001
    .encoder_cnt = 0, // 0.5
    .current_pos = 0,
    .output_pwm = 500,
    .integral_max = 20.50f
};

PID_t right_motor = {
    .Kp = 0.085f,
    .Ki = 0.015f,
    .Kd = 3.2f, //0.001
    .encoder_cnt = 0, // 0.5
    .current_pos = 0,
    .output_pwm = 500,
    .integral_max = 20.50f
};

void pid_cycle(PID_t* target_system){

    target_system->current_error = target_system->target_pos - target_system->current_pos;

    float derivative = target_system->current_error - target_system->last_error;
    
    if(target_system->current_error < 1.5f && target_system->current_error > -1.5f){
        target_system->integral += target_system->current_error;
    }
    else{
        target_system->integral = 0;
    }
    // bound the integral
    if (target_system->integral > target_system->integral_max){
        target_system->integral = target_system->integral_max;
    } else if (target_system->integral < -target_system->integral_max) {
        target_system->integral = -target_system->integral_max;
    }
    float voltage = target_system->Kp * target_system->current_error + target_system->Ki * target_system->integral + target_system->Kd * derivative;
    target_system->last_error = target_system->current_error;

    int32_t pwm_temp = 500 * (voltage/12.0f + 1);

    if (target_system->current_error > -ERROR_DEADBAND && target_system->current_error < ERROR_DEADBAND) {
        pwm_temp = 500;
        target_system->integral *= 0.95f;  // reset integral to prevent windup
        if (target_system == &left_motor) {
            left_motor_arrived = 1;
        } else if (target_system == &right_motor) {
            right_motor_arrived = 1;
        }
    } else {
        if (target_system == &left_motor) {
            left_motor_arrived = 0;
        } else if (target_system == &right_motor) {
            right_motor_arrived = 0;
        }
    }

    if (pwm_temp > 1000) pwm_temp = 1000;
    if (pwm_temp < 0)   pwm_temp = 0;

    if (pwm_temp < 499 && pwm_temp > 250) pwm_temp = 250;
    if (pwm_temp > 501 && pwm_temp < 750) pwm_temp = 750;
    target_system->output_pwm = pwm_temp;
    
    return;
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

    // // disable idle interrupt half and full interrupt
    // extern DMA_HandleTypeDef handle_GPDMA1_Channel1;
    // __HAL_DMA_DISABLE_IT(&handle_GPDMA1_Channel1, DMA_IT_HT);
    // __HAL_DMA_DISABLE_IT(&handle_GPDMA1_Channel1, DMA_IT_TC); 
}

void controller_step(const float dt) {
#ifdef USECONTROLLER
    // read the encoder 

    left_motor.encoder_cnt += encoder_read_value(&htim3);
    left_motor.current_pos = encoder_parse_distance_mm(left_motor.encoder_cnt);
    vofa.val[0] = left_motor.current_pos;
    vofa.val[1] = left_motor.target_pos;

    right_motor.encoder_cnt += encoder_read_value(&htim4);
    right_motor.current_pos = encoder_parse_distance_mm(right_motor.encoder_cnt);
    vofa.val[5] = right_motor.current_pos;
    vofa.val[6] = right_motor.target_pos;

    pid_cycle(&left_motor);
    pid_cycle(&right_motor);
    vofa.val[2] = left_motor.output_pwm;
    vofa.val[7] = right_motor.output_pwm;

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, right_motor.output_pwm);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, left_motor.output_pwm);

    if (!uart_binary_mode
        && menu_get_state() != MENU_STATE_TRANSMIT
        && menu_get_state() != MENU_STATE_TRANSMIT_RAM
        && huart2.gState == HAL_UART_STATE_READY) {
            HAL_UART_Transmit_DMA(&huart2, (uint8_t*)&vofa, sizeof(vofa));
    }
#endif
}
