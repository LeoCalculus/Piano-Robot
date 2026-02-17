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
volatile uint32_t time_counter = 0;
volatile int is_moving = 0;
volatile int is_blocked = 0;
volatile int redirect_to_ram = 0; // flag to indicate whether the incoming song data should be written to RAM
volatile int ram_rx_started = 0;  // 1 = start sentinel received, accumulating
volatile int ram_rx_complete = 0; // 1 = end sentinel received, ready to parse
volatile uint32_t ram_rx_offset = 0; // byte offset into song_ram during accumulation
__ALIGN_BEGIN float rx_data[5500] __ALIGN_END;
__ALIGN_BEGIN float song_ram[365][14] __ALIGN_END; // receive buffer for RAM song transfer (20440 bytes)

// make a song that has chord nodes:
// test song: 15 chords, left hand only with varying positions and durations
// left hand range: ~5cm to ~49cm (after adding LEFT_HAND_SIZE/2)
Song_t piano_song[MAX_CHORD_EVENTS] = {
    // {{{left_pos, right_pos}, {pressed[10]}, duration_ms, delay_to_next_ms}}
    {{{12.0f, 60.0f}, {true, false, false, false, false, true, false, false, false, false}, 300, 150}},
    {{{25.0f, 0.0f},  {false, true, false, false, false, false, false, false, false, false}, 200, 100}},
    {{{18.0f, 72.0f}, {true, true, false, false, false, false, true, false, false, false}, 350, 120}},
    {{{35.0f, 0.0f}, {false, false, true, false, false, false, false, false, false, false}, 180, 80}},
    {{{8.0f, 65.0f}, {true, false, true, false, false, true, false, true, false, false}, 400, 200}},
    {{{42.0f, 0.0f}, {false, false, false, true, false, false, false, false, false, false}, 250, 100}},
    {{{0.0f, 58.0f}, {false, false, false, false, false, true, true, false, false, false}, 280, 150}},
    {{{30.0f, 70.0f}, {true, false, false, false, true, false, false, true, false, true}, 500, 180}},
    {{{15.0f, 0.0f}, {false, true, true, false, false, false, false, false, false, false}, 220, 100}},
    {{{0.0f, 62.0f}, {false, false, false, false, false, false, true, false, true, false}, 300, 120}},
    {{{38.0f, 75.0f}, {true, false, false, true, false, true, false, false, false, true}, 450, 200}},
    {{{22.0f, 0.0f}, {false, false, true, false, true, false, false, false, false, false}, 180, 80}},
    {{{10.0f, 68.0f}, {true, true, false, false, false, false, true, true, false, false}, 380, 150}},
    {{{45.0f, 0.0f}, {false, false, false, false, true, false, false, false, false, false}, 320, 100}},
    {{{20.0f, 55.0f}, {true, false, true, false, false, true, false, false, true, false}, 600, 200}}
    // remaining entries are zero-initialized
};


int which_hand(ChordEvent_t* this_note){
    // check the array whether it has 
    int left_flag = 0;
    int right_flag = 0;
    for (int i=0; i < 5; i++){
        if (this_note->positions[0]!=0.0f){ // check whether non-zero
            left_flag = 1;
        }
    }

    for (int i=5; i < 10; i++){
        if (this_note->positions[1]!=0.0f){ // non zero means there is target
            right_flag = 1;
        }
    }

    if (left_flag == 1 && right_flag == 0){
        return 0;
    } else if (left_flag == 0 && right_flag == 1){
        return 1;
    } else if (left_flag & right_flag) {
        return 2;
    } else { // no hands
        return 3;
    }
}

// all value should expected larger than 0, left should have range: PIANO_THRESHOLD - LEFT_HAND_SIZE/2
// homing algo
void locate_hand(ChordEvent_t* this_note, int which_hand) {
    // index 3 means
    if (which_hand == 3) {
        return;
    }

    // handle left hand for current version
    if (which_hand == 1 && which_hand == 2){
        target_position_cm = this_note->positions[0];
    }

    if (which_hand == 0 && which_hand == 2) {
    
    }


}

// using the for loop to traversal, call function to start playing the song
void traversal_song(Song_t* entire_song){
    // the entire song chords might be smaller than given length
    for (int i = 0; i < MAX_CHORD_EVENTS; i++){
        // if detect empty notes for both of target position - means end - break loop 
        if (entire_song[i].event.positions[0] == 0.0f && entire_song[i].event.positions[1] == 0.0f) {
            break;
        }
        // otherwise, we deal with the song
        int hand = which_hand(&entire_song[i].event);
        locate_hand(&entire_song[i].event, hand); // target was set inside the function
        // solenoid logic: According to the play map of boolean numbers
        // how to: traversal the boolean array to toggle corresponding GPIO pins
        /*
        if (is_moving == 0) { // means stop
            push_solenoid();
        }
        
        */
        wait_ms(entire_song[i].event.duration_ms);
    }
}

// parse flat float buffer [365][14] into Song_t array
// format per row: [pos_left, pos_right, pressed[0..9], duration_ms, delay_to_next_ms]
void parsing_song_buffer_to_struct(float song_buffer[365][14], Song_t* song_struct){
    memset(song_struct, 0, sizeof(Song_t) * MAX_CHORD_EVENTS);
    int count = (365 < MAX_CHORD_EVENTS) ? 365 : MAX_CHORD_EVENTS;
    for (int i = 0; i < count; i++){
        // stop at all-zero row (end of song)
        int all_zero = 1;
        for (int j = 0; j < 14; j++){
            if (song_buffer[i][j] != 0.0f){ all_zero = 0; break; }
        }
        if (all_zero) break;

        song_struct[i].event.positions[0] = song_buffer[i][0];
        song_struct[i].event.positions[1] = song_buffer[i][1];
        for (int k = 0; k < 10; k++){
            song_struct[i].event.pressed[k] = (song_buffer[i][2 + k] != 0.0f);
        }
        song_struct[i].event.duration_ms = (uint16_t)song_buffer[i][12];
        song_struct[i].event.delay_to_next_ms = (uint16_t)song_buffer[i][13];
    }
}

void homing_procedure() {
    /*
    WARNING, if moving some speed by cannot move might lead to too much current and burn down the circuit
    M1: move with certain speed, e.g. max speed, then use maximum movement distance / max speed for timing to reach the boundary
    M2: add sensor check at 0 and 100 positions if reached sensor -> gpio read to reset distance
    M3: move at low speed and check the distance, if no more distance it can move (stuck) -> stop, homing ok
    */
}

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
    // if (error > 5.0f || error < -5.0f) {
    //     target_system->output = target_system->Kp * error + target_system->Kd * derivative;
    //     target_system->Integral = 0.0f;
    // } else {
    //     target_system->output = target_system->Kp * error + target_system->Ki * target_system->Integral + target_system->Kd * derivative;
    // }
    target_system->output = target_system->Kp * error + target_system->Ki * target_system->Integral + target_system->Kd * derivative;
    target_system->last_error = error;
}

// PID struct for the motor
PID_t debug_motor = {
    .Kp = 5.0f,
    .Ki = 0.01f,
    .Kd = 0.13f, //0.001
    .Integral_max = 0.01f // 0.5
};

// init LCD configuration with SPI2 and GPIOB pins 13, 14, 15
LCD_Config lcd_config = {
    .hspi = &hspi2,
    .cs_port = GPIOB,
    .cs_pin = GPIO_PIN_15,
    .rs_port = GPIOB,
    .rs_pin = GPIO_PIN_13,
    .rst_port = GPIOB,
    .rst_pin = GPIO_PIN_14,
};

void wait_ms(uint32_t ms){
    uint32_t start = time_counter;
    while (time_counter - start < ms);
}

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
// measured duty cycle: 1.254 us per cycle @ 1kHz 
void controller_step(const float dt){
    // measure the duty cycle with LED
    // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);

    uint32_t direction;

    encoder_read_value(&htim8, &encoder_read_result, &direction);
    current_distance_cm = encoder_parse_distance_cm(encoder_read_result);
    vofa.val[0] = current_distance_cm;
    current_velocity_cm_s = encoder_calc_speed_cm_s(current_distance_cm, dt);

    // position control - test pid - using step input - input via bluetooth
    float error = target_position_cm - current_distance_cm;
    // error = 0.0f; // debug 
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
        is_moving = 1; // have done moving
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

    // check for pin state (EE-SX1041: HIGH when blocked, LOW when light passes through)
    is_blocked = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == GPIO_PIN_SET) ? 1 : 0;

    // during these stage the LED will keep on, on oscilloscope will be high.
    // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); 
}

uint32_t get_free_ram(void) {
    // Use address of a local variable as a reliable approximation of SP
    // This avoids inline asm portability issues across optimization levels
    volatile uint32_t stack_marker;
    uint32_t stack_ptr = (uint32_t)&stack_marker;
    uint32_t heap_end = (uint32_t)&_end;
    if (stack_ptr < heap_end) return 0; // safety check
    return stack_ptr - heap_end;
}