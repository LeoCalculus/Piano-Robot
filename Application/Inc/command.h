#ifndef __COMMAND_H
#define __COMMAND_H

#include <stdint.h>
#include <string.h>
#include <oled1315.h>

// parse command
int parseCommand(uint8_t* cmd);
void executeCommand(uint8_t* command);

// commands
void setText(uint8_t* displayedText);


#endif