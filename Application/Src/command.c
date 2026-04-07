#include <command.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32h5xx_hal.h"
#include <application.h>
#include <diskio.h>
#include <sd.h>
#include <hc04.h>
#include <menu.h>
#include <user_timer.h>
#include <tim.h>

static uint8_t cmd_listen[16];

extern PID_t left_motor;
extern PID_t right_motor;


int parse_command(uint8_t *cmd)
{
    /* ---- Colon commands (:p, :w, :s, :r, :d, :a) ---- */
    if (cmd[0] == ':'){
        switch (cmd[1]){
        case 'p':{ /* :p <value> — set position in mm */
            char *arg = (char *)cmd + 3;
            while (*arg == ' ')
                arg++;
            if (*arg == '\0')
                return 3; /* incomplete */

            // separate left and right
            if (cmd[2] == 'l'){
                left_motor.target_pos = strtof(arg, NULL);
            }
            else if (cmd[2] == 'r'){
                right_motor.target_pos = strtof(arg, NULL);
            }
            else {
                return 1;
            }
            return 0;
        }
        case 'w': /* :w — menu move up */
            menu_move_up = 1;
            return 0;
        case 's': /* :s — menu move down */
            menu_move_down = 1;
            return 0;
        case 'r': /* :r — reset encoder position */
            return 0;
        case 'd': /* :d — enter / select current menu item */
            menu_enter = 1;
            return 0;
        case 'a': /* :a — back to main menu */
            menu_back = 1;
            return 0;
        case 't': // set text in LCD
            char *arg = (char *)cmd + 2;
            while (*arg == ' ')
                arg++;
            if (*arg == '\0')
                return 3; /* incomplete */
            set_text((uint8_t *)arg);
            return 0;
        case 'o': // reset position
            return 0;
        case 'l': // list files on SD card via XOR packets
            list_files_over_bt();
            return 0;

        case 'z':
            traversal();
            return 0;

        case 'x':
            debug_traversal();
            return 0;

        case 'q': // stop playing current song:
            stop_playing = 1;
            return 0;

        // right now redirect all playing to Ji's song
        case 'v': // Ji's song
            load_debug_song2();
            if (menu_get_state() == MENU_STATE_DEBUG) {
                traversal_debug_mode();
            } else {
                menu_set_state(MENU_STATE_PLAYING);
                pkt_send_ui_state((uint8_t)MENU_STATE_PLAYING, 0, 0, 0);
                traversal();
                menu_set_state(MENU_STATE_MAIN);
            }
            return 0;

        case 'b': // swan lake
            load_debug_song3();
            if (menu_get_state() == MENU_STATE_DEBUG) {
                traversal_debug_mode();
            } else {
                menu_set_state(MENU_STATE_PLAYING);
                pkt_send_ui_state((uint8_t)MENU_STATE_PLAYING, 0, 0, 0);
                traversal();
                menu_set_state(MENU_STATE_MAIN);
            }
            return 0;

        case 'n': // Jamie song
            load_debug_song();
            if (menu_get_state() == MENU_STATE_DEBUG) {
                traversal_debug_mode();
            } else {
                menu_set_state(MENU_STATE_PLAYING);
                pkt_send_ui_state((uint8_t)MENU_STATE_PLAYING, 0, 0, 0);
                traversal();
                menu_set_state(MENU_STATE_MAIN);
            }
            return 0;


        default:
            return 1; /* unknown colon command */
        }
    }

    // else if (cmd[0] == ';'){
    //     switch (cmd[1]){
    //     case 'p':{ /* ;p <value> — set Kp */
    //         char *arg = (char *)cmd + 3;
    //         while (*arg == ' ')
    //             arg++;
    //         if (*arg == '\0')
    //             return 3; /* incomplete */

    //         if (cmd[2] == 'l'){
    //             left_motor.Kp = strtof(arg, NULL);
    //         }
    //         else if (cmd[2] == 'r'){
    //             left_motor.Kp = strtof(arg, NULL);
    //         }
    //         else {
    //             return 1;
    //         }
    //         return 0;
    //     }
    //     case 'i':{ /* ;i <value> — set Ki */
    //         char *arg = (char *)cmd + 3;
    //         while (*arg == ' ')
    //             arg++;
    //         if (*arg == '\0')
    //             return 3; /* incomplete */

    //         if (cmd[2] == 'l'){
    //             left_motor.Ki = strtof(arg, NULL);
    //         }
    //         else if (cmd[2] == 'r'){
    //             left_motor.Ki = strtof(arg, NULL);
    //         }
    //         else {
    //             return 1;
    //         }
    //         return 0;
    //     }
    //     case 'd':{ /* ;d <value> — set Kd */
    //         char *arg = (char *)cmd + 3;
    //         while (*arg == ' ')
    //             arg++;
    //         if (*arg == '\0')
    //             return 3; /* incomplete */

    //         if (cmd[2] == 'l'){
    //             left_motor.Kd = strtof(arg, NULL);
    //         }
    //         else if (cmd[2] == 'r'){
    //             left_motor.Kd = strtof(arg, NULL);
    //         }
    //         else {
    //             return 1;
    //         }
    //         return 0;
    //     }
    //     }
    // }
    return 1;
}

void execute_command(uint8_t *command)
{
    int result = parse_command(command);
    if (result == 0)
    {
        LCD_draw_string(0, 14, "OK.                      ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 15, "                         ", COLOR_BLACK, COLOR_WHITE);
    }
    else if (result == 1)
    {
        LCD_draw_string(0, 14, "Cannot parse!            ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 15, (char*)command, COLOR_BLACK, COLOR_WHITE);
    }
    else if (result == 2)
    {
        LCD_draw_string(0, 14, "Empty command!           ", COLOR_BLACK, COLOR_WHITE);
    }
    else if (result == 3)
    {
        LCD_draw_string(0, 14, "Incomplete command!      ", COLOR_BLACK, COLOR_WHITE);
        LCD_draw_string(0, 15, (char*)command, COLOR_BLACK, COLOR_WHITE);
    }
}

void set_text(uint8_t *displayedText)
{
    LCD_draw_string(0, 13, (char *)displayedText, COLOR_BLACK, COLOR_WHITE);
}

void reset_pos(TIM_HandleTypeDef *htim)
{
    __HAL_TIM_SET_COUNTER(htim, 0);
}

/*
 * Send the TXT file list over Bluetooth using XOR packets.
 */
void list_files_over_bt(void)
{
    sd_list_txt_files();
    for (int i = 0; i < fileCount; i++) {
        /* Wait until UART TX is idle before sending next entry */
        while (huart2.gState != HAL_UART_STATE_READY) { /* spin */ }
        pkt_send_file_entry(fileList[i].name, fileList[i].size);
    }
    while (huart2.gState != HAL_UART_STATE_READY) { /* spin */ }
    pkt_send_file_list_end((uint8_t)fileCount);
}

void toggle_solenoid(uint16_t traversal_index){
    pwm_mode = PWM_STOP;
    for (int i = 0; i < 10; i++) {
        HAL_GPIO_WritePin(solenoids[i].port, solenoids[i].pin, chord_events[traversal_index].pressed[i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
    // wait for the duration of the chord:
    wait_ms(chord_events[traversal_index].duration_ms);
    // reset solenoids based on which hand changes position next
    if (chord_events[traversal_index].long_pressed[0]){ // left hand long press, keep it pressed
        for (int i = 0; i < 7; i++) {
            HAL_GPIO_WritePin(solenoids[i].port, solenoids[i].pin, chord_events[traversal_index].pressed[i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
    } else { // reset all left hand solenoids, since short click
        for (int i = 0; i < 7; i++) {
            HAL_GPIO_WritePin(solenoids[i].port, solenoids[i].pin, GPIO_PIN_RESET);
        }

    }

    if (chord_events[traversal_index].long_pressed[1]){ // right hand long press
        for (int i = 7; i < 10; i++) {
            HAL_GPIO_WritePin(solenoids[i].port, solenoids[i].pin, chord_events[traversal_index].pressed[i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
    } else { // short click for right
        for (int i = 7; i < 10; i++) {
            HAL_GPIO_WritePin(solenoids[i].port, solenoids[i].pin, GPIO_PIN_RESET);
        }
    }
    pwm_mode = PWM_PID;
}

/* Count total chords in the loaded song (looks for end sentinel 1000,1000) */
static uint16_t count_total_chords(void)
{
    for (uint16_t i = 0; i < MAX_CHORD_EVENTS; i++) {
        if (chord_events[i].positions[0] == 1000.0f &&
            chord_events[i].positions[1] == 1000.0f)
            return i;
    }
    return MAX_CHORD_EVENTS;
}

void traversal(void){

    pwm_mode = PWM_PID;
    uint16_t traversal_index = 0;
    if (chord_events[traversal_index].positions[0] == 0.0f && chord_events[traversal_index].positions[1] == 0.0f){
        LCD_draw_string(0, 9, "No Song loaded!     ", COLOR_BLACK, COLOR_WHITE);
        pkt_send_play_status(2, 0, 0); /* 2 = error/no song */
        return;
    }

    uint16_t total_chords = count_total_chords();

    /* Clear LCD for playing UI */
    for (int r = 1; r <= 9; r++)
        LCD_draw_string(0, r, "                        ", COLOR_BLACK, COLOR_WHITE);
    LCD_draw_string(0, 1, "[Playing Song]          ", COLOR_BLACK, COLOR_WHITE);
    LCD_draw_string(0, 8, ":q to stop              ", COLOR_BLACK, COLOR_WHITE);

    pkt_send_play_status(1, 0, total_chords); /* 1 = playing */

    while (1){
        if (chord_events[traversal_index].positions[0] == 1000.0f && chord_events[traversal_index].positions[1] == 1000.0f){
            LCD_draw_string(0, 9, "Song ends     ", COLOR_BLACK, COLOR_WHITE);
            pkt_send_play_status(0, traversal_index, total_chords); /* 0 = done */
            pwm_mode = PWM_STOP;
            break;
        }

        // listen to incoming commands only, check for :q only
        if (rx_complete) {
            uint16_t cmd_len = rx_valid;
            memcpy(cmd_listen, rx_message, cmd_len + 1);
            rx_complete = 0;
            cmd_listen[cmd_len] = '\0';
            if (cmd_len > 0 && cmd_listen[cmd_len - 1] == '\n') {
                cmd_listen[cmd_len - 1] = '\0';
                cmd_len--;
            }
            menu_process_message(cmd_listen, cmd_len);
            execute_command(cmd_listen);
        }

        if (stop_playing) {
            LCD_draw_string(0, 9, "Stopped by user     ", COLOR_BLACK, COLOR_WHITE);
            pkt_send_play_status(2, traversal_index, total_chords); /* 2 = stopped */
            
            // first disable all solenoids:
            for (int i = 0; i < 10; i++) {
                HAL_GPIO_WritePin(solenoids[i].port, solenoids[i].pin, GPIO_PIN_RESET);
            }

            wait_ms(500); // wait for solenoids to reset before resetting the motors

            stop_playing = 0;
            left_motor.target_pos = 0.0f;
            right_motor.target_pos = 0.0f;
            wait_ms(500);
            pwm_mode = PWM_STOP;
            break;
        }

        // set the target position:
        left_motor.target_pos = chord_events[traversal_index].positions[0];
        right_motor.target_pos = chord_events[traversal_index].positions[1];

        wait_ms(5);

        // blocking: send bt data meanwhile wait controller update a while
        // these total use 700us blocking
        pkt_send_play_status(1, traversal_index, total_chords);
        pkt_send_heartbeat();

        while(left_motor_arrived == 0 || right_motor_arrived == 0);

        wait_ms(20);
        toggle_solenoid(traversal_index);

        traversal_index++;
    }

}

void traversal_debug_mode(void){

    pwm_mode = PWM_PID;
    uint16_t traversal_index = 0;
    if (chord_events[traversal_index].positions[0] == 0.0f && chord_events[traversal_index].positions[1] == 0.0f){
        LCD_draw_string(0, 9, "No Song loaded!     ", COLOR_BLACK, COLOR_WHITE);
        return;
    }

    /* Stay in DEBUG state so VOFA keeps flowing from controller_step() */
    LCD_draw_string(0, 1, "[Debug Play] :q stop    ", COLOR_BLACK, COLOR_WHITE);

    while (1){
        if (chord_events[traversal_index].positions[0] == 1000.0f && chord_events[traversal_index].positions[1] == 1000.0f){
            LCD_draw_string(0, 9, "Song ends     ", COLOR_BLACK, COLOR_WHITE);
            pwm_mode = PWM_STOP;
            break;
        }

        // listen for :q
        if (rx_complete) {
            uint16_t cmd_len = rx_valid;
            memcpy(cmd_listen, rx_message, cmd_len + 1);
            rx_complete = 0;
            cmd_listen[cmd_len] = '\0';
            if (cmd_len > 0 && cmd_listen[cmd_len - 1] == '\n') {
                cmd_listen[cmd_len - 1] = '\0';
                cmd_len--;
            }
            execute_command(cmd_listen);
        }

        if (stop_playing) {
            LCD_draw_string(0, 9, "Stopped by user     ", COLOR_BLACK, COLOR_WHITE);

            for (int i = 0; i < 10; i++) {
                HAL_GPIO_WritePin(solenoids[i].port, solenoids[i].pin, GPIO_PIN_RESET);
            }
            wait_ms(500);

            stop_playing = 0;
            left_motor.target_pos = 0.0f;
            right_motor.target_pos = 0.0f;
            wait_ms(500);
            pwm_mode = PWM_STOP;
            break;
        }

        // set the target position:
        left_motor.target_pos = chord_events[traversal_index].positions[0];
        right_motor.target_pos = chord_events[traversal_index].positions[1];

        wait_ms(5);
        while(left_motor_arrived == 0 || right_motor_arrived == 0);

        /* No packets here — VOFA is sent by controller_step() at 1kHz */
        wait_ms(20);
        toggle_solenoid(traversal_index);

        traversal_index++;
    }
}

void debug_traversal(void){
    pwm_mode = PWM_PID;
    while (right_motor.target_pos < 138.0f || left_motor.target_pos < 138.0f) {
        right_motor.target_pos += 21.0f;
        left_motor.target_pos += 21.0f;
        wait_ms(10); // small delay so controller gets updated
        while(!right_motor_arrived || !left_motor_arrived);

        // stop the motor when pressing
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 500);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500);

        for (int i = 0; i < 10; i++) {
            HAL_GPIO_WritePin(solenoids[i].port, solenoids[i].pin, GPIO_PIN_SET);
            wait_ms(300);
            HAL_GPIO_WritePin(solenoids[i].port, solenoids[i].pin, GPIO_PIN_RESET);
            wait_ms(300);
        }

        wait_ms(1000);
    }
    pwm_mode = PWM_STOP;
}
