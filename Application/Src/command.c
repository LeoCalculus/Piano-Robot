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
        case 'j':
            HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_4);
            HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_5);
            return 0;
        case 'w': /* :w — menu move up */
            menu_move_up = 1;
            return 0;
        case 's': /* :s — menu move down */
            menu_move_down = 1;
            return 0;
        case 'r': /* :r — reset encoder position */
            // reset_pos(&htim2);
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
            // reset_pos(&htim2);
            return 0;
        case 'l':
            // list_files_over_bt();
            return 0;

        case 'z':
            // here begin test traversal:
            // traversal();
            debug_traversal();
            return 0;

        default:
            return 1; /* unknown colon command */
        }
    }

    else if (cmd[0] == ';'){
        switch (cmd[1]){
        case 'p':{ /* :p <value> — set position in mm */
            char *arg = (char *)cmd + 3;
            while (*arg == ' ')
                arg++;
            if (*arg == '\0')
                return 3; /* incomplete */

            // separate left and right
            if (cmd[2] == 'l'){
                left_motor.Kp = strtof(arg, NULL);
            }
            else if (cmd[2] == 'r'){
                left_motor.Kp = strtof(arg, NULL);
            }
            else {
                return 1;
            }
            return 0;
        }
        case 'i':{ /* :p <value> — set position in mm */
            char *arg = (char *)cmd + 3;
            while (*arg == ' ')
                arg++;
            if (*arg == '\0')
                return 3; /* incomplete */

            // separate left and right
            if (cmd[2] == 'l'){
                left_motor.Ki = strtof(arg, NULL);
            }
            else if (cmd[2] == 'r'){
                left_motor.Ki = strtof(arg, NULL);
            }
            else {
                return 1;
            }
            return 0;
        }
        case 'd':{ /* :p <value> — set position in mm */
            char *arg = (char *)cmd + 3;
            while (*arg == ' ')
                arg++;
            if (*arg == '\0')
                return 3; /* incomplete */

            // separate left and right
            if (cmd[2] == 'l'){
                left_motor.Kd = strtof(arg, NULL);
            }
            else if (cmd[2] == 'r'){
                left_motor.Kd = strtof(arg, NULL);
            }
            else {
                return 1;
            }
            return 0;
        }
        }
    }
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
    // encoder_old_position_mm = 0.0f;
    __HAL_TIM_SET_COUNTER(htim, 0);
}

/*
 * Send the TXT file list over Bluetooth in the format expected by hc04.py:
 *   FILES:<count>\n
 *   <filename>,<size>\n
 *   ...
 *   END\n
 */
void list_files_over_bt(void)
{
    // char buf[40];

    // /* Refresh file list from SD card */
    // int count = sd_list_txt_files();

    // snprintf(buf, sizeof(buf), "FILES:%d\n", count);
    // HC04_SendString(buf);

    // for (int i = 0; i < count; i++)
    // {
    //     snprintf(buf, sizeof(buf), "%s,%lu\n",
    //              fileList[i].name, (unsigned long)fileList[i].size);
    //     HC04_SendString(buf);
    // }

    hc04_send_string((uint8_t*)"END\n");
}

void toggle_solenoid(uint16_t traversal_index){
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
    
    if (chord_events[traversal_index].long_pressed[1]){ // right hand long press, reset left hand solenoids
        for (int i = 7; i < 10; i++) {
            HAL_GPIO_WritePin(solenoids[i].port, solenoids[i].pin, chord_events[traversal_index].pressed[i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
    } else { // short click for right
        for (int i = 7; i < 10; i++) {
            HAL_GPIO_WritePin(solenoids[i].port, solenoids[i].pin, GPIO_PIN_RESET);
        }
    }

}

void traversal(void){
    // while (right_motor.target_pos < 300.0f) {
    //     right_motor.target_pos += 21.0f;
    //     wait_ms(10); // small delay so controller gets updated
    //     while(!right_motor_arrived);

    //     // stop the motor when pressing
    //     __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 500);
    //     __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500);

    //     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
    //     wait_ms(300);
    //     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);
    //     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
    //     wait_ms(300);
    //     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
    //     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
    //     wait_ms(300);
    //     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
    //     wait_ms(200);
        
    //     wait_ms(1000);
    // }
    uint16_t traversal_index = 0;
    while (1){
        if (chord_events[traversal_index].positions[0] == 0.0f && chord_events[traversal_index].positions[1] == 0.0f){
            LCD_draw_string(0, 9, "No song in RAM!       ", COLOR_BLACK, COLOR_WHITE);
            break;
        }

        // otherwise set the target position:
        left_motor.target_pos = chord_events[traversal_index].positions[0];
        right_motor.target_pos = chord_events[traversal_index].positions[1];

        // wait for both to arrive - disable to check traversal
        // while(!left_motor_arrived || !right_motor_arrived);
        // // exit reset two flag:
        // left_motor_arrived = 0;
        // right_motor_arrived = 0;

        // stop the motor when pressing
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 500);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500);

        // here just wait for pressing:
        toggle_solenoid(traversal_index);

        wait_ms(chord_events[traversal_index].duration_ms);

        // in the end will just increment the index:
        traversal_index++;
    }

}

void debug_traversal(void){
    while (right_motor.target_pos < 138.0f || left_motor.target_pos < 138.0f) {
        right_motor.target_pos += 21.0f;
        left_motor.target_pos += 21.0f;
        wait_ms(10); // small delay so controller gets updated
        while(!right_motor_arrived & !left_motor_arrived);

        // stop the motor when pressing
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 500);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500);

        for (int i = 0; i < 11; i++) {
            HAL_GPIO_WritePin(solenoids[i].port, solenoids[i].pin, GPIO_PIN_SET);
            wait_ms(300);
            HAL_GPIO_WritePin(solenoids[i].port, solenoids[i].pin, GPIO_PIN_RESET);
            wait_ms(300);
        }
        
        wait_ms(1000);
    }
}
