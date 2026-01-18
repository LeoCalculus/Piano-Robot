#include <command.h>
#include <sdcard.h>
#include <hc05bt.h>
#include <file_transfer.h>
#include <stdio.h>
#include "stm32f1xx_hal.h"

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
    } else if (strcmp(command, "listFiles") == 0) {
        /* List MIDI files on SD card and send via Bluetooth */
        listMidiFiles();
    } else if (strcmp(command, "uploadAbort") == 0) {
        /* Abort ongoing file transfer */
        FT_Abort();
        HC05_SendInfo((uint8_t*)"UPLOAD_ABORTED\r\n");
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

void listMidiFiles(void){
    char buf[40];
    int count = SD_ListMidiFiles();

    /* Send file count header */
    snprintf(buf, sizeof(buf), "FILES:%d\r\n", count);
    HC05_SendInfo((uint8_t*)buf);
    HAL_Delay(20);

    /* Send each file entry: filename,size */
    for (int i = 0; i < count && i < fileCount; i++) {
        snprintf(buf, sizeof(buf), "%s,%lu\r\n",
                 fileList[i].name, fileList[i].size);
        HC05_SendInfo((uint8_t*)buf);
        HAL_Delay(20);  /* Small delay between messages */
    }

    /* Send end marker */
    HC05_SendInfo((uint8_t*)"END\r\n");
}
