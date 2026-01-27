#include <command.h>
#include <stdio.h>
#include "stm32h5xx_hal.h"
#include <application.h>

int parseCommand(uint8_t* cmd) {
    if (cmd[0] != '/') {
        return 1; // error command
    }

    // available commands:
    char* command = strtok((char*)cmd+1, " ");

    if (command == NULL) {
        return 2; // empty command
    }

    char* argument1 = strtok(NULL, " ");

    if (strcmp(command, "setText") == 0) {
        if (argument1 == NULL) {
            return 3; // incomplete command
        } else {
            setText((uint8_t*)argument1);
        }
    } else if (strcmp(command, "setPos") == 0) {
        if (argument1 == NULL) {
            return 3;
        } else {
            setPos((float*)argument1); // convert to input with float 
        }
    } else if (strcmp(command, "resetPos") == 0){
        resetPos();
    }

    return 0;
}

void executeCommand(uint8_t* command){
    int result = parseCommand(command);
    if (result == 0) {
        LCD_draw_string(&lcd_config, 0, 14, "OK.                      ", COLOR_BLACK, COLOR_WHITE);
    } else if (result == 1){
        LCD_draw_string(&lcd_config, 0, 14, "Cannot parse!            ", COLOR_BLACK, COLOR_WHITE);
    } else if (result == 2){
        LCD_draw_string(&lcd_config, 0, 14, "Empty command!           ", COLOR_BLACK, COLOR_WHITE);
    } else if (result == 3){
        LCD_draw_string(&lcd_config, 0, 14, "Incomplete command!      ", COLOR_BLACK, COLOR_WHITE);
    }
}

void setText(uint8_t* displayedText){
    LCD_draw_string(&lcd_config, 0, 13, (char*)displayedText, COLOR_BLACK, COLOR_WHITE);
}

void setPos(float* targetPos){
    target_position_mm = *targetPos;
}

void resetPos(){
    encoder_old_position_mm = 0.0f;
}

