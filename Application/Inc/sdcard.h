#ifndef __SDCARD_H
#define __SDCARD_H

#include <gpio.h>
#include <spi.h>
#include <stdint.h>

// SD card uses SPI2: PB13=SCK, PB14=MISO, PB15=MOSI
// CS pin: PC14 (directly controlled via GPIO)

extern uint8_t SDBuffer[512];

// Initialize SD card and load FAT32 boot sector into SDBuffer
// Returns: 0=success, 1=CMD0 fail, 2=MBR read fail, 3=ACMD41 timeout
int initSDCard(void);

// Read a 512-byte block from SD card
// blockAddr: block number (for SDHC cards, this is the sector number)
// buffer: pointer to 512-byte buffer
// Returns: 0=success, 1=command fail, 2=no data token
int SD_readData(uint32_t blockAddr, uint8_t* buffer);

// Send command to SD card and get response
uint8_t SD_SendCommand(uint8_t* cmd, uint8_t len);

#endif
