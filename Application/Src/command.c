#include <command.h>

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
    }

    return 0;
}

void executeCommand(uint8_t* command){
    int result = parseCommand(command);
    OLED_SetCursor(0, 7);
    if (result == 0) {
        OLED_WriteString("OK.                ");
    } else if (result == 1){
        OLED_WriteString("Cannot parse!      ");
    } else if (result == 2){
        OLED_WriteString("Empty Command!     ");
    } else if (result == 3){
        OLED_WriteString("Incomplete command!");
    }
}

void setText(uint8_t* displayedText){
    OLED_SetCursor(0, 5);
    OLED_WriteString((char*)displayedText);
}
