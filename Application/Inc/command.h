#ifndef __COMMAND_H
#define __COMMAND_H

#include <stdint.h>
#include <string.h>
#include <ILI9225.h>
#include <application.h>

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

// this is a public function can be used any place
void traversal(void);

void debug_traversal(void);

// traversal in debug mode: no packets, keeps VOFA flowing for oscilloscope
void traversal_debug_mode(void);

void toggle_solenoid(uint16_t traversal_index);

#endif
