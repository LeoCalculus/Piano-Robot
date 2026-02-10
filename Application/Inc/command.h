#ifndef __COMMAND_H
#define __COMMAND_H

#include <stdint.h>
#include <string.h>
#include <ILI9341.h>

/* parse command */
int parseCommand(uint8_t *cmd);
void executeCommand(uint8_t *command);

/* commands */

// print text on LCD screen
void setText(uint8_t *displayedText);

// set the target position (in cm)
void setPos(float targetPos);

// reset the old position
void resetPos(TIM_HandleTypeDef *htim);

// list TXT files on SD card over Bluetooth
void listFilesOverBT(void);

#endif
