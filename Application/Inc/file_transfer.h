#ifndef __FILE_TRANSFER_H
#define __FILE_TRANSFER_H

#include <stdint.h>
#include <ff.h>

/* Protocol Command Codes (PC -> STM32) */
#define FT_CMD_FILE_START 0xF0
#define FT_CMD_FILE_DATA 0xF1
#define FT_CMD_FILE_END 0xF2
#define FT_CMD_FILE_ABORT 0xF3

/* RAM protocol commands */
#define FT_CMD_RAM_START 0xE0
#define FT_CMD_RAM_DATA 0xE1
#define FT_CMD_RAM_END 0xE2
#define RAM_SENTINEL_START 0x78
#define RAM_SENTINEL_END 0x91

/* Protocol Response Codes (STM32 -> PC) */
#define FT_RSP_ACK 0xA0
#define FT_RSP_NAK 0xA1
#define FT_RSP_READY 0xA2
#define FT_RSP_ERROR 0xA3
#define FT_RSP_SUCCESS 0xA4

/* Error Codes */
#define FT_ERR_NONE 0x00
#define FT_ERR_CHECKSUM 0x01
#define FT_ERR_SEQUENCE 0x02
#define FT_ERR_SD_WRITE 0x03
#define FT_ERR_SD_FULL 0x04
#define FT_ERR_FILE_EXISTS 0x05
#define FT_ERR_INVALID_NAME 0x06
#define FT_ERR_TIMEOUT 0x07
#define FT_ERR_OVERFLOW 0x08

/* Protocol Constants */
#define FT_MAX_CHUNK_SIZE 64
#define FT_MAX_FILENAME 8
#define FT_MAX_EXTENSION 4
#define FT_TIMEOUT_MS 30000 /* 30s - generous for BT SPP latency */
#define FT_RX_BUFFER_SIZE 80

/* Packet Sizes */
#define FT_FILE_START_SIZE 18
#define FT_FILE_END_SIZE 4
#define FT_FILE_ABORT_SIZE 2

// new added 2026.03.05 - Check sum only. applied to RAM based file transfer to avoid data lost.

/* Transfer State Machine */
typedef enum
{
    FT_STATE_IDLE,
    FT_STATE_RECEIVING,
    FT_STATE_COMPLETE,
    FT_STATE_ERROR
} FT_State_t;

/* File Transfer Context */
typedef struct
{
    FT_State_t state;
    FIL file;
    char filename[13]; /* 8.3 format + null */
    uint32_t expectedSize;
    uint32_t bytesReceived;
    uint16_t totalChunks;
    uint16_t currentChunk;
    uint32_t lastActivityTime;
    uint8_t errorCode;
} FT_Context_t;

// public API for file transfer / RAM transfer
void FT_Init(void);
void FT_ProcessPacket(uint8_t *packet, uint16_t length);
void RAM_ProcessPacket(uint8_t *packet, uint16_t length);
void FT_TimeoutCheck(void);
void FT_Abort(void);
FT_State_t FT_GetState(void);
uint8_t FT_GetProgress(void);

/* Internal helper functions */
uint8_t FT_CalculateChecksum(uint8_t *data, uint16_t length);
uint8_t RAM_CalculateChecksum(uint8_t *data, uint16_t length);
void FT_SendResponse(uint8_t responseCode, uint8_t *data, uint8_t dataLen);

#endif /* __FILE_TRANSFER_H */
