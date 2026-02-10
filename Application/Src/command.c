#include <command.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32h5xx_hal.h"
#include <application.h>
#include <sdcard.h>
#include <hc04bt.h>
#include <menu.h>

int parseCommand(uint8_t *cmd)
{
    /* ---- Colon commands (:p, :w, :s, :r, :d, :a) ---- */
    if (cmd[0] == ':')
    {
        switch (cmd[1])
        {
        case 'p':
        { /* :p <value> — set position in mm */
            char *arg = (char *)cmd + 2;
            while (*arg == ' ')
                arg++;
            if (*arg == '\0')
                return 3; /* incomplete */
            float pos = strtof(arg, NULL);
            setPos(pos);
            return 0;
        }
        case 'w': /* :w — menu move up */
            menu_move_up = 1;
            return 0;
        case 's': /* :s — menu move down */
            menu_move_down = 1;
            return 0;
        case 'r': /* :r — reset encoder position */
            resetPos(&htim8);
            return 0;
        case 'd': /* :d — enter / select current menu item */
            menu_enter = 1;
            return 0;
        case 'a': /* :a — back to main menu */
            menu_back = 1;
            return 0;
        default:
            return 1; /* unknown colon command */
        }
    }

    /* ---- Slash commands (/setText, /setPos, /resetPos, /listFiles) ---- */
    if (cmd[0] != '/')
    {
        return 1; /* unrecognised */
    }

    char *command = strtok((char *)cmd + 1, " ");
    if (command == NULL)
        return 2; /* empty */

    char *argument1 = strtok(NULL, " ");

    if (strcmp(command, "setText") == 0)
    {
        if (argument1 == NULL)
            return 3;
        setText((uint8_t *)argument1);
    }
    else if (strcmp(command, "setPos") == 0)
    {
        if (argument1 == NULL)
            return 3;
        float pos = strtof(argument1, NULL);
        setPos(pos);
    }
    else if (strcmp(command, "resetPos") == 0)
    {
        resetPos(&htim8);
    }
    else if (strcmp(command, "listFiles") == 0)
    {
        listFilesOverBT();
    }
    else
    {
        return 1; /* unknown command */
    }

    return 0;
}

void executeCommand(uint8_t *command)
{
    int result = parseCommand(command);
    if (result == 0)
    {
        LCD_draw_string(&lcd_config, 0, 14, "OK.                      ", COLOR_BLACK, COLOR_WHITE);
    }
    else if (result == 1)
    {
        LCD_draw_string(&lcd_config, 0, 14, "Cannot parse!            ", COLOR_BLACK, COLOR_WHITE);
    }
    else if (result == 2)
    {
        LCD_draw_string(&lcd_config, 0, 14, "Empty command!           ", COLOR_BLACK, COLOR_WHITE);
    }
    else if (result == 3)
    {
        LCD_draw_string(&lcd_config, 0, 14, "Incomplete command!      ", COLOR_BLACK, COLOR_WHITE);
    }
}

void setText(uint8_t *displayedText)
{
    LCD_draw_string(&lcd_config, 0, 13, (char *)displayedText, COLOR_BLACK, COLOR_WHITE);
}

void setPos(float targetPos)
{
    target_position_cm = targetPos;
}

void resetPos(TIM_HandleTypeDef *htim)
{
    encoder_old_position_cm = 0.0f;
    __HAL_TIM_SET_COUNTER(htim, 0);
}

/*
 * Send the TXT file list over Bluetooth in the format expected by hc04.py:
 *   FILES:<count>\n
 *   <filename>,<size>\n
 *   ...
 *   END\n
 */
void listFilesOverBT(void)
{
    char buf[40];

    /* Refresh file list from SD card */
    int count = SD_ListTxtFiles();

    snprintf(buf, sizeof(buf), "FILES:%d\n", count);
    HC04_SendString(buf);

    for (int i = 0; i < count; i++)
    {
        snprintf(buf, sizeof(buf), "%s,%lu\n",
                 fileList[i].name, (unsigned long)fileList[i].size);
        HC04_SendString(buf);
    }

    HC04_SendString("END\n");
}
