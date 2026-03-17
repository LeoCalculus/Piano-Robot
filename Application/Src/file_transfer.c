#include <file_transfer.h>
#include <hc04.h>
#include <diskio.h>
#include <string.h>
#include <stdio.h>
#include "stm32h5xx_hal.h"

/* Global transfer context */
static FT_Context_t ftContext;

/* Write buffer for batching SD writes */
#define FT_WRITE_BUFFER_SIZE 512 /* Match SD sector size */
static uint8_t ftWriteBuffer[FT_WRITE_BUFFER_SIZE];
static uint16_t ftWriteBufferIndex = 0;

/* Track last completed transfer for duplicate FILE_END handling */
static uint16_t lastCompletedBytes = 0;
static uint32_t lastCompletedTime = 0;
volatile int RAM_track_index = 0;
volatile int RAM_transfer_complete = 0;

/* Flush write buffer to SD card */
static int FT_FlushWriteBuffer(void)
{
    if (ftWriteBufferIndex == 0)
        return 0;

    UINT bytesWritten;
    FRESULT res = f_write(&ftContext.file, ftWriteBuffer, ftWriteBufferIndex, &bytesWritten);
    if (res != FR_OK || bytesWritten != ftWriteBufferIndex)
    {
        return -1;
    }
    ftWriteBufferIndex = 0;
    return 0;
}

/* Initialize file transfer module */
void FT_Init(void)
{
    memset(&ftContext, 0, sizeof(ftContext));
    ftContext.state = FT_STATE_IDLE;
    ftContext.errorCode = FT_ERR_NONE;
    ftWriteBufferIndex = 0;
}

/* Calculate XOR checksum */
uint8_t FT_CalculateChecksum(uint8_t *data, uint16_t length)
{
    uint8_t checksum = 0;
    for (uint16_t i = 0; i < length; i++)
    {
        checksum ^= data[i];
    }
    return checksum;
}


/* Calculate XOR checksum for RAM-based file transfer */
uint8_t RAM_CalculateChecksum(uint8_t *data, uint16_t length){
    uint8_t checksum = 0;
    for (uint16_t i = 0; i < length; i++)
    {
        checksum ^= data[i];
    }
    return checksum;
}

/* Send response packet to PC (blocking – responses are tiny, 2-8 bytes) */
void FT_SendResponse(uint8_t responseCode, uint8_t *data, uint8_t dataLen)
{
    uint8_t response[8];
    uint8_t idx = 0;

    response[idx++] = responseCode;
    for (uint8_t i = 0; i < dataLen; i++)
    {
        response[idx++] = data[i];
    }
    response[idx] = FT_CalculateChecksum(response, idx);
    idx++;

    HAL_UART_Transmit(&huart1, response, idx, 50);
}

/* Handle FILE_START packet */
static int FT_HandleFileStart(uint8_t *packet, uint16_t length)
{
    if (length < FT_FILE_START_SIZE)
    {
        FT_SendResponse(FT_RSP_ERROR, (uint8_t[]){FT_ERR_OVERFLOW}, 1);
        return -1;
    }

    /* Verify checksum */
    if (FT_CalculateChecksum(packet, length - 1) != packet[length - 1])
    {
        FT_SendResponse(FT_RSP_NAK, (uint8_t[]){0, 0, FT_ERR_CHECKSUM}, 3);
        return -1;
    }

    /* If already receiving and got another FILE_START, PC didn't get our READY */
    /* Re-send READY if we're at chunk 0 (just started), otherwise abort and restart */
    if (ftContext.state == FT_STATE_RECEIVING)
    {
        if (ftContext.currentChunk == 0)
        {
            /* Just re-send READY - PC retried because it didn't get our response */
            FT_SendResponse(FT_RSP_READY, NULL, 0);
            ftContext.lastActivityTime = HAL_GetTick();
            return 0;
        }
        else
        {
            /* Mid-transfer FILE_START means something went wrong - abort and restart */
            f_close(&ftContext.file);
            f_unlink(ftContext.filename);
            ftContext.state = FT_STATE_IDLE;
            ftWriteBufferIndex = 0;
        }
    }

    /* Parse file info */
    uint16_t fileSize = packet[1] | (packet[2] << 8);
    uint16_t totalChunks = packet[3] | (packet[4] << 8);

    /* Build filename from 8.3 format */
    char name[9], ext[5];
    memcpy(name, &packet[5], 8);
    name[8] = '\0';
    memcpy(ext, &packet[13], 4);
    ext[4] = '\0';

    /* Trim trailing spaces/nulls from name */
    for (int i = 7; i >= 0; i--)
    {
        if (name[i] == ' ' || name[i] == '\0')
        {
            name[i] = '\0';
        }
        else
        {
            break;
        }
    }

    /* Trim trailing spaces/nulls from extension */
    for (int i = 3; i >= 0; i--)
    {
        if (ext[i] == ' ' || ext[i] == '\0')
        {
            ext[i] = '\0';
        }
        else
        {
            break;
        }
    }

    /* Combine name and extension */
    snprintf(ftContext.filename, sizeof(ftContext.filename), "%s%s", name, ext);

    /* Open file for writing (create or overwrite) */
    FRESULT res = f_open(&ftContext.file, ftContext.filename,
                         FA_WRITE | FA_CREATE_ALWAYS);
    if (res != FR_OK)
    {
        /* Send error with the actual FRESULT so we can debug */
        uint8_t errData[2];
        errData[0] = (res == FR_DISK_ERR) ? FT_ERR_SD_WRITE : FT_ERR_SD_FULL;
        errData[1] = (uint8_t)res; /* raw FRESULT for debug */
        FT_SendResponse(FT_RSP_ERROR, errData, 2);
        return -1;
    }

    /* Initialize context */
    ftContext.state = FT_STATE_RECEIVING;
    ftContext.expectedSize = fileSize;
    ftContext.totalChunks = totalChunks;
    ftContext.currentChunk = 0;
    ftContext.bytesReceived = 0;
    ftContext.lastActivityTime = HAL_GetTick();
    ftContext.errorCode = FT_ERR_NONE;

    FT_SendResponse(FT_RSP_READY, NULL, 0);
    return 0;
}

/* Handle FILE_DATA packet */
static int FT_HandleFileData(uint8_t *packet, uint16_t length)
{
    if (ftContext.state != FT_STATE_RECEIVING)
    {
        FT_SendResponse(FT_RSP_ERROR, (uint8_t[]){FT_ERR_SEQUENCE}, 1);
        return -1;
    }

    if (length < 5)
    {
        FT_SendResponse(FT_RSP_ERROR, (uint8_t[]){FT_ERR_OVERFLOW}, 1);
        return -1;
    }

    uint16_t chunkIndex = packet[1] | (packet[2] << 8);
    uint8_t dataLen = packet[3];

    /* Verify we have enough data */
    if (length < (uint16_t)(5 + dataLen))
    {
        uint8_t nakData[] = {chunkIndex & 0xFF, chunkIndex >> 8, FT_ERR_OVERFLOW};
        FT_SendResponse(FT_RSP_NAK, nakData, 3);
        return -1;
    }

    /* Verify checksum */
    if (FT_CalculateChecksum(packet, 4 + dataLen) != packet[4 + dataLen])
    {
        uint8_t nakData[] = {chunkIndex & 0xFF, chunkIndex >> 8, FT_ERR_CHECKSUM};
        FT_SendResponse(FT_RSP_NAK, nakData, 3);
        return -1;
    }

    /* Handle DUPLICATE packet (PC retried because ACK was lost) */
    /* If we receive the chunk we JUST processed, re-ACK without re-writing */
    if (chunkIndex == ftContext.currentChunk - 1 && ftContext.currentChunk > 0)
    {
        /* Duplicate of last successful chunk - just re-send ACK */
        uint8_t ackData[] = {chunkIndex & 0xFF, chunkIndex >> 8};
        FT_SendResponse(FT_RSP_ACK, ackData, 2);
        ftContext.lastActivityTime = HAL_GetTick();
        return 0; /* Success - no data written */
    }

    /* Verify sequence - must be the expected chunk */
    if (chunkIndex != ftContext.currentChunk)
    {
        uint8_t nakData[] = {chunkIndex & 0xFF, chunkIndex >> 8, FT_ERR_SEQUENCE};
        FT_SendResponse(FT_RSP_NAK, nakData, 3);
        return -1;
    }

    /* Copy data to write buffer */
    memcpy(&ftWriteBuffer[ftWriteBufferIndex], &packet[4], dataLen);
    ftWriteBufferIndex += dataLen;
    ftContext.bytesReceived += dataLen;

    /* Flush buffer when full (512 bytes = 1 SD sector) */
    if (ftWriteBufferIndex >= FT_WRITE_BUFFER_SIZE)
    {
        if (FT_FlushWriteBuffer() != 0)
        {
            uint8_t nakData[] = {chunkIndex & 0xFF, chunkIndex >> 8, FT_ERR_SD_WRITE};
            FT_SendResponse(FT_RSP_NAK, nakData, 3);
            return -1;
        }
    }

    /* Update context */
    ftContext.currentChunk++;
    ftContext.lastActivityTime = HAL_GetTick();

    /* Send ACK */
    uint8_t ackData[] = {chunkIndex & 0xFF, chunkIndex >> 8};
    FT_SendResponse(FT_RSP_ACK, ackData, 2);
    return 0;
}

/* Handle RAM packet data
   Packet: [0xE1] [row_lo] [row_hi] [56 bytes = 14 floats] [checksum]
   Total = 60 bytes
*/
static int FT_HandleRAMData(uint8_t *packet, uint16_t length){
    // Packet must be exactly 60 bytes: cmd(1) + row(2) + data(56) + checksum(1)
    if (length != 60)
    {
        uint8_t nakData[] = {0, 0, FT_ERR_OVERFLOW};
        FT_SendResponse(FT_RSP_NAK, nakData, 3);
        return -1;
    }

    // Extract row index early so NAK responses can include it
    uint16_t rowIndex = packet[1] | (packet[2] << 8);

    // Verify checksum: XOR of bytes [0..58], compare with byte [59]
    if (RAM_CalculateChecksum(packet, length - 1) != packet[length - 1])
    {
        uint8_t nakData[] = {rowIndex & 0xFF, rowIndex >> 8, FT_ERR_CHECKSUM};
        FT_SendResponse(FT_RSP_NAK, nakData, 3);
        return -1;
    }

    // Validate row index (0-364)
    if (rowIndex >= 365)
    {
        uint8_t nakData[] = {rowIndex & 0xFF, rowIndex >> 8, FT_ERR_OVERFLOW};
        FT_SendResponse(FT_RSP_NAK, nakData, 3);
        return -1;
    }

    // Copy 14 floats (56 bytes) into song_ram[rowIndex]
    memcpy(song_ram[rowIndex], &packet[3], 56);
    ram_rx_offset += 56;

    RAM_track_index++;
    // Tell PC this row is OK
    uint8_t ackData[] = {rowIndex & 0xFF, rowIndex >> 8};
    FT_SendResponse(FT_RSP_ACK, ackData, 2);
    return 0;
}

/* Handle FILE_END packet */
static int FT_HandleFileEnd(uint8_t *packet, uint16_t length)
{
    if (length < FT_FILE_END_SIZE)
    {
        FT_SendResponse(FT_RSP_ERROR, (uint8_t[]){FT_ERR_OVERFLOW}, 1);
        return -1;
    }

    /* Verify checksum */
    if (FT_CalculateChecksum(packet, length - 1) != packet[length - 1])
    {
        FT_SendResponse(FT_RSP_ERROR, (uint8_t[]){FT_ERR_CHECKSUM}, 1);
        return -1;
    }

    uint16_t reportedBytes = packet[1] | (packet[2] << 8);

    /* Handle DUPLICATE FILE_END (PC retried because SUCCESS was lost) */
    /* If we're IDLE but recently completed a transfer with matching size, re-send SUCCESS */
    if (ftContext.state == FT_STATE_IDLE)
    {
        uint32_t elapsed = HAL_GetTick() - lastCompletedTime;
        if (elapsed < 10000 && reportedBytes == lastCompletedBytes)
        {
            /* Recent completion with matching size - re-send SUCCESS */
            uint8_t successData[] = {lastCompletedBytes & 0xFF, (lastCompletedBytes >> 8) & 0xFF};
            FT_SendResponse(FT_RSP_SUCCESS, successData, 2);
            return 0;
        }
        /* Not a duplicate - wrong state */
        FT_SendResponse(FT_RSP_ERROR, (uint8_t[]){FT_ERR_SEQUENCE}, 1);
        return -1;
    }

    if (ftContext.state != FT_STATE_RECEIVING)
    {
        FT_SendResponse(FT_RSP_ERROR, (uint8_t[]){FT_ERR_SEQUENCE}, 1);
        return -1;
    }

    /* Flush remaining data in buffer */
    if (FT_FlushWriteBuffer() != 0)
    {
        f_close(&ftContext.file);
        f_unlink(ftContext.filename);
        FT_SendResponse(FT_RSP_ERROR, (uint8_t[]){FT_ERR_SD_WRITE}, 1);
        ftContext.state = FT_STATE_ERROR;
        return -1;
    }

    /* Sync and close file */
    f_sync(&ftContext.file);
    f_close(&ftContext.file);

    /* Verify byte count */
    if (reportedBytes != (ftContext.bytesReceived & 0xFFFF))
    {
        /* Delete incomplete file */
        f_unlink(ftContext.filename);
        FT_SendResponse(FT_RSP_ERROR, (uint8_t[]){FT_ERR_SD_WRITE}, 1);
        ftContext.state = FT_STATE_ERROR;
        return -1;
    }

    ftContext.state = FT_STATE_COMPLETE;

    /* Track this completion for duplicate handling */
    lastCompletedBytes = ftContext.bytesReceived & 0xFFFF;
    lastCompletedTime = HAL_GetTick();

    /* Refresh TXT file list */
    sd_list_txt_files();

    /* Send success with bytes received */
    uint8_t successData[] = {
        ftContext.bytesReceived & 0xFF,
        (ftContext.bytesReceived >> 8) & 0xFF};
    FT_SendResponse(FT_RSP_SUCCESS, successData, 2);

    /* Reset for next transfer */
    FT_Init();
    return 0;
}

/* Handle FILE_ABORT packet */
static int FT_HandleFileAbort(uint8_t *packet, uint16_t length)
{
    (void)packet;
    (void)length;

    FT_Abort();
    return 0;
}

/* SD card FT - Process received packet */
void FT_ProcessPacket(uint8_t *packet, uint16_t length)
{
    if (length == 0)
        return;

    uint8_t command = packet[0];

    switch (command)
    {
    case FT_CMD_FILE_START:
        FT_HandleFileStart(packet, length);
        break;

    case FT_CMD_FILE_DATA:
        FT_HandleFileData(packet, length);
        break;

    case FT_CMD_FILE_END:
        FT_HandleFileEnd(packet, length);
        break;

    case FT_CMD_FILE_ABORT:
        FT_HandleFileAbort(packet, length);
        break;

    default:
        FT_SendResponse(FT_RSP_ERROR, (uint8_t[]){FT_ERR_SEQUENCE}, 1);
        break;
    }
}

/* RAM FT - Process received packet */
void RAM_ProcessPacket(uint8_t *packet, uint16_t length)
{
    // Minimal length is 2, smaller means corupted data packet
    if (length < 2)
        return;

    uint8_t command = packet[0];

    switch (command)
    {
        case FT_CMD_RAM_START: // [0xE0] [Start Signal]
            if (packet[1] == RAM_SENTINEL_START){
                memset(song_ram, 0, RAM_SONG_MAX_BYTES);
                ram_rx_started = 1;
                ram_rx_complete = 0;
                ram_rx_offset = 0;
                RAM_track_index = 0; // reset track index at start of new RAM transfer
                FT_SendResponse(FT_RSP_READY, NULL, 0);
            } else {
                FT_SendResponse(FT_RSP_NAK, (uint8_t[]){0, 0, FT_ERR_SEQUENCE}, 3);
            }
            break;

        case FT_CMD_RAM_DATA: // [0xE1] [row_lo] [row_hi] [56 bytes] [checksum]
            FT_HandleRAMData(packet, length);
            break;

        case FT_CMD_RAM_END: // [0xE2] [End Signal]
            if (packet[1] == RAM_SENTINEL_END){
                ram_rx_complete = 1;
                RAM_transfer_complete = 1;
                FT_SendResponse(FT_RSP_SUCCESS, (uint8_t[]){0, 0}, 2);
            } else {
                FT_SendResponse(FT_RSP_NAK, (uint8_t[]){0, 0, FT_ERR_SEQUENCE}, 3);
            }
            break;

        default:
            // Handle unknown command
            break;
    }
}

/* Check for timeout during transfer */
void FT_TimeoutCheck(void)
{
    if (ftContext.state == FT_STATE_RECEIVING)
    {
        uint32_t elapsed = HAL_GetTick() - ftContext.lastActivityTime;
        if (elapsed > FT_TIMEOUT_MS)
        {
            /* Timeout - abort transfer */
            FT_Abort();
            FT_SendResponse(FT_RSP_ERROR, (uint8_t[]){FT_ERR_TIMEOUT}, 1);
        }
    }
}

/* Abort current transfer */
void FT_Abort(void)
{
    if (ftContext.state == FT_STATE_RECEIVING)
    {
        /* Close and delete incomplete file */
        f_close(&ftContext.file);
        f_unlink(ftContext.filename);
    }

    ftContext.state = FT_STATE_IDLE;
    ftContext.errorCode = FT_ERR_NONE;
}

/* Get current transfer state */
FT_State_t FT_GetState(void)
{
    return ftContext.state;
}

/* Get transfer progress (0-100) */
uint8_t FT_GetProgress(void)
{
    if (ftContext.state != FT_STATE_RECEIVING || ftContext.totalChunks == 0)
    {
        return 0;
    }
    return (uint8_t)((ftContext.currentChunk * 100) / ftContext.totalChunks);
}
