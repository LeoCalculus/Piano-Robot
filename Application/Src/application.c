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

// make a song that has chord nodes:
// test song: 15 chords, left hand only with varying positions and durations
// left hand range: ~5cm to ~49cm (after adding LEFT_HAND_SIZE/2)
Song_t piano_song[MAX_CHORD_EVENTS] = {
    // {{num_notes, {positions...}, duration_ms, delay_to_next}}
    {{1, {10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 200, 100}},  // target: 15cm
    {{1, {20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 200, 100}},  // target: 25cm
    {{1, {30.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 200, 100}},  // target: 35cm
    {{1, {40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 200, 100}},  // target: 45cm
    {{1, {44.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 300, 100}},  // target: 49cm
    {{1, {35.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 150, 100}},  // target: 40cm
    {{1, {25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 150, 100}},  // target: 30cm
    {{1, {15.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 150, 100}},  // target: 20cm
    {{1, { 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 300, 100}},  // target: 10cm
    {{2, {10.0f, 20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 250, 100}}, // target: 15cm
    {{2, {25.0f, 35.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 250, 100}}, // target: 30cm
    {{3, {15.0f, 25.0f, 35.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 200, 100}},// target: 20cm
    {{1, {40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 400, 100}},  // target: 45cm
    {{1, {20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 400, 100}},  // target: 25cm
    {{1, {10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 500, 100}}   // target: 15cm
    // remaining entries are zero-initialized
};


int which_hand(ChordEvent_t* this_note){
    // check the array whether it has 
    int left_flag = 0;
    int right_flag = 0;
    for (int i=0; i < 5; i++){
        if (this_note->positions[i]!=0){
            left_flag = 1;
        }
    }

    for (int i=5; i < 10; i++){
        if (this_note->positions[i]!=0){
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
void locate_hand(ChordEvent_t* this_note, float hand_pos[2], int which_hand) {
    hand_pos[0] = 0.0f;
    hand_pos[1] = 0.0f;

    // index 3 means
    if (which_hand == 3) {
        return;
    }

    // left hand: index 0 to 4
    if (which_hand == 0 || which_hand == 2) {
        float min_value = 1e9f;
        for (int i = 0; i < 5; i++) {
            if (this_note->positions[i] != 0.0f && this_note->positions[i] < min_value) {
                min_value = this_note->positions[i];
            }
        }
        float left_pos = min_value;
        // clamp to avoid collision at threshold
        hand_pos[0] = (left_pos + LEFT_HAND_SIZE / 2.0f > PIANO_THRESHOLD) ? PIANO_THRESHOLD - LEFT_HAND_SIZE / 2.0f : left_pos + LEFT_HAND_SIZE / 2.0f;
    }

    // right hand: index 5 to 9
    if (which_hand == 1 || which_hand == 2) {
        float max_value = -1e9f;
        for (int i = 5; i < 10; i++) {
            if (this_note->positions[i] != 0.0f && this_note->positions[i] > max_value) {
                max_value = this_note->positions[i];
            }
        }
        float right_pos = max_value;
        // clamp to avoid collision at threshold
        hand_pos[1] = (right_pos - RIGHT_HAND_SIZE / 2.0f < PIANO_THRESHOLD) ? PIANO_THRESHOLD + RIGHT_HAND_SIZE / 2.0f : right_pos - RIGHT_HAND_SIZE / 2.0f;
    }
}

// using the for loop to traversal, call function to start playing the song
void traversal_song(Song_t* entire_song){
    // the entire song chords might be smaller than given length
    for (int i = 0; i < MAX_CHORD_EVENTS; i++){
        // if detect empty notes - means end - break loop 
        if (entire_song[i].event.num_notes == 0) {
            break;
        }
        // otherwise, we deal with the song
        float hand_pos[2];
        int hand = which_hand(&entire_song[i].event);
        locate_hand(&entire_song[i].event, hand_pos, hand);
        // set target position from parsed data:
        target_position_cm = hand_pos[0];
        // solenoid logic:
        /*
        if (is_moving == 0) { // means stop
            push_solenoid();
        }
        
        */
        wait_ms(entire_song[i].event.duration_ms);
    }
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
    .Kp = 8.0f,
    .Ki = 1.0f,
    .Kd = 0.0f, //0.001
    .Integral_max = 200000000.0f // 0.5
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
}