#include <command.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32h5xx_hal.h"
#include <application.h>
#include <diskio.h>
#include <sd.h>
#include <hc04.h>
#include <menu.h>

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
