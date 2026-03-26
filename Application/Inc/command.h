#ifndef __COMMAND_H
#define __COMMAND_H

#include <stdint.h>
#include <string.h>
#include <ILI9225.h>

/* parse command */
int parse_command(uint8_t *cmd);
void execute_command(uint8_t *command);

/* commands */

// print text on LCD screen
void set_text(uint8_t *displayedText);

// set the target position (in mm)
void set_pos(float target_pos);

// reset the old position
void reset_pos(TIM_HandleTypeDef *htim);

// list TXT files on SD card over Bluetooth
void list_files_over_bt(void);

void traversal(void);

#endif
