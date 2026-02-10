/*
 * sdcard.c - SD Card operations using FatFs
 */

#include <sdcard.h>
#include <ILI9341.h>
#include <string.h>
#include <stdio.h>

/* Global variables */
FATFS fatFs;
FileEntry fileList[MAX_FILES];
uint8_t fileCount = 0;

int SD_IsCardPresent(SPI_HandleTypeDef* hspi, GPIO_TypeDef* detectPort, uint16_t detectPin){
    uint8_t tx, rx;

    // give SD card time to power up
    HAL_Delay(100);

    // start with deselect
    HAL_GPIO_WritePin(detectPort, detectPin, GPIO_PIN_SET);

    // wake up sd card by sending 80+ dummy clock cycles (10 bytes) with CS high
    tx = 0xFF;
    for (int i = 0; i < 10; i++) {
        HAL_SPI_TransmitReceive(hspi, &tx, &rx, 1, 100);
    }

    // select SD card
    HAL_GPIO_WritePin(detectPort, detectPin, GPIO_PIN_RESET);
    HAL_Delay(1);

    // send CMD0 (GO_IDLE_STATE)
    tx = 0x40 | 0;
    HAL_SPI_TransmitReceive(hspi, &tx, &rx, 1, 100);

    tx = 0x00;
    HAL_SPI_TransmitReceive(hspi, &tx, &rx, 1, 100);
    HAL_SPI_TransmitReceive(hspi, &tx, &rx, 1, 100);
    HAL_SPI_TransmitReceive(hspi, &tx, &rx, 1, 100);
    HAL_SPI_TransmitReceive(hspi, &tx, &rx, 1, 100);
    tx = 0x95;
    HAL_SPI_TransmitReceive(hspi, &tx, &rx, 1, 100);

    // wait for response (0x01 = idle state)
    tx = 0xFF;
    uint8_t response = 0xFF;
    for (int i = 0; i < 100; i++) {
        HAL_SPI_TransmitReceive(hspi, &tx, &response, 1, 100);
        if (response != 0xFF) {

        }
        if (response == 0x01) {
            break;
        }
    }

    // deselect the card
    HAL_GPIO_WritePin(detectPort, detectPin, GPIO_PIN_SET);
    HAL_SPI_TransmitReceive(hspi, &tx, &rx, 1, 100);

    return (response == 0x01) ? 1 : 0;
}

/* Initialize SD card and mount filesystem */
FRESULT SD_Init(void) {
    FRESULT res;

    /* Mount the filesystem */
    res = f_mount(&fatFs, "", 1);  /* 1 = mount now */

    return res;
}

/* Mount SD card */
FRESULT SD_Mount(void) {
    return f_mount(&fatFs, "", 1);
}

/* Unmount SD card */
FRESULT SD_Unmount(void) {
    return f_mount(NULL, "", 0);
}

/* Open file */
FRESULT SD_OpenFile(FIL* file, const char* path, BYTE mode) {
    return f_open(file, path, mode);
}

/* Close file */
FRESULT SD_CloseFile(FIL* file) {
    return f_close(file);
}

/* Read from file */
FRESULT SD_ReadFile(FIL* file, void* buff, UINT btr, UINT* br) {
    return f_read(file, buff, btr, br);
}

/* Write to file */
FRESULT SD_WriteFile(FIL* file, const void* buff, UINT btw, UINT* bw) {
    return f_write(file, buff, btw, bw);
}

/* Helper: Check if filename has .TXT extension */
static int isTxtFile(const char* name) {
    size_t len = strlen(name);
    if (len < 4) return 0;

    const char* ext = name + len - 4;
    return (ext[0] == '.' &&
            (ext[1] == 'T' || ext[1] == 't') &&
            (ext[2] == 'X' || ext[2] == 'x') &&
            (ext[3] == 'T' || ext[3] == 't'));
}

/* List all text files in root directory */
int SD_ListTxtFiles(void) {
    FRESULT res;
    DIR dir;
    FILINFO fno;

    fileCount = 0;

    res = f_opendir(&dir, "/");
    if (res != FR_OK) {
        return 0;
    }

    while (fileCount < MAX_FILES) {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) {
            break;  /* Error or end of directory */
        }

        /* Skip directories and hidden/system files */
        if (fno.fattrib & (AM_DIR | AM_HID | AM_SYS)) {
            continue;
        }

        /* Check if it's a .TXT file */
        if (isTxtFile(fno.fname)) {
            /* Copy filename (truncate if needed) */
            strncpy(fileList[fileCount].name, fno.fname, MAX_FILENAME_LEN - 1);
            fileList[fileCount].name[MAX_FILENAME_LEN - 1] = '\0';
            fileList[fileCount].size = fno.fsize;
            fileCount++;
        }
    }

    f_closedir(&dir);
    return fileCount;
}

/* Read text file into buffer */
int SD_ReadTxtFile(uint8_t fileIndex, uint8_t* buffer, uint32_t maxSize) {
    if (fileIndex >= fileCount) {
        return -1;
    }

    FIL file;
    FRESULT res;
    UINT bytesRead;

    res = f_open(&file, fileList[fileIndex].name, FA_READ);
    if (res != FR_OK) {
        return -1;
    }

    uint32_t readSize = (fileList[fileIndex].size < maxSize) ?
                         fileList[fileIndex].size : maxSize;

    res = f_read(&file, buffer, readSize, &bytesRead);
    f_close(&file);

    if (res != FR_OK) {
        return -1;
    }

    return (int)bytesRead;
}

/* Display text file list on OLED */
// void SD_DisplayTxtFileList(uint8_t startIndex, uint8_t selectedIndex) {
//     char displayName[20];

//     /* Display up to 5 text files on OLED pages 3-7 */
//     for (uint8_t i = 0; i < 5; i++) {
//         uint8_t fileIdx = startIndex + i;
//         uint8_t page = 3 + i;

//         OLED_SetCursor(0, page);

//         if (fileIdx < fileCount) {
//             /* Show selection indicator */
//             if (fileIdx == selectedIndex) {
//                 OLED_WriteChar('>');
//             } else {
//                 OLED_WriteChar(' ');
//             }

//             /* Display filename (truncate if needed) */
//             strncpy(displayName, fileList[fileIdx].name, 19);
//             displayName[19] = '\0';
//             OLED_WriteString(displayName);

//             /* Clear rest of line */
//             uint8_t nameLen = strlen(displayName) + 1;
//             for (uint8_t j = nameLen; j < 21; j++) {
//                 OLED_WriteChar(' ');
//             }
//         } else {
//             /* Clear empty line */
//             for (uint8_t j = 0; j < 21; j++) {
//                 OLED_WriteChar(' ');
//             }
//         }
//     }
// }

/* Get storage info */
FRESULT SD_GetStorageInfo(SD_StorageInfo* info) {
    FATFS* fs;
    DWORD freeClusters;
    FRESULT res;

    res = f_getfree("", &freeClusters, &fs);
    if (res != FR_OK) {
        return res;
    }

    /* Calculate sizes in KB */
    DWORD totalSectors = (fs->n_fatent - 2) * fs->csize;
    DWORD freeSectors = freeClusters * fs->csize;

    info->totalKB = totalSectors / 2;  /* 512 bytes per sector, /2 = KB */
    info->freeKB = freeSectors / 2;

    return FR_OK;
}
