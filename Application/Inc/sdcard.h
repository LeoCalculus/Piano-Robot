#ifndef __SDCARD_H
#define __SDCARD_H

#include <main.h>
#include <ff.h>
#include <stdint.h>

/* SD Card Configuration */
#define MAX_FILENAME_LEN 13  /* 8.3 format + null terminator */
#define MAX_FILES        100

/* File entry structure for MIDI file listing */
typedef struct {
    char     name[MAX_FILENAME_LEN];
    uint32_t size;
} FileEntry;

/* Global variables */
extern FATFS fatFs;           /* FatFs filesystem object */
extern FileEntry fileList[MAX_FILES];
extern uint8_t fileCount;

/* SD Card / FatFs initialization */
/* Returns: FR_OK (0) on success, error code otherwise */
FRESULT SD_Init(void);

/* Mount/Unmount SD card */
FRESULT SD_Mount(void);
FRESULT SD_Unmount(void);

/* File operations using FatFs */
FRESULT SD_OpenFile(FIL* file, const char* path, BYTE mode);
FRESULT SD_CloseFile(FIL* file);
FRESULT SD_ReadFile(FIL* file, void* buff, UINT btr, UINT* br);
FRESULT SD_WriteFile(FIL* file, const void* buff, UINT btw, UINT* bw);

/* MIDI file listing (backward compatible) */
int SD_ListMidiFiles(void);

/* Read MIDI file into buffer */
/* fileIndex: index into fileList array */
/* buffer: destination buffer */
/* maxSize: maximum bytes to read */
/* Returns: bytes read, or -1 on error */
int SD_ReadMidiFile(uint8_t fileIndex, uint8_t* buffer, uint32_t maxSize);

/* Display song list on OLED */
void SD_DisplaySongList(uint8_t startIndex, uint8_t selectedIndex);

/* Get storage info */
typedef struct {
    uint32_t totalKB;
    uint32_t freeKB;
} SD_StorageInfo;

FRESULT SD_GetStorageInfo(SD_StorageInfo* info);

#endif /* __SDCARD_H */
