#include <sdcard.h>
#include <string.h>

uint8_t SDBuffer[512];

uint8_t SD_SendCommand(uint8_t* cmd, uint8_t len) {
    uint8_t response = 0x00;
    uint8_t dummy = 0xFF;

    // iterate to transmite the spi data
    for (int i = 0; i < len; i++) {
        HAL_SPI_TransmitReceive(&hspi2, &cmd[i], &response, 1, 100);
    }

    // wait for response from SD card since SD card has delay (bit 7 = 0)
    for (int i = 0; i < 10; i++) { // response within 10 iterations
        HAL_SPI_TransmitReceive(&hspi2, &dummy, &response, 1, 100);
        if ((response & 0x80) == 0) {
            return response;
        }
    }
    return 0xFF; // cannot get command response
}

int SD_readData(uint32_t blockAddr, uint8_t* buffer) {
    // read 512 byte from a statring address
    uint8_t response = 0x00;
    uint8_t dummy = 0xFF;
    uint8_t cmd17[] = {0x51,(blockAddr >> 24) & 0xFF, (blockAddr >> 16) & 0xFF, (blockAddr >> 8) & 0xFF, blockAddr & 0xFF, 0xFF};

    // Select card and send CMD17 (READ_SINGLE_BLOCK)
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, RESET);
    HAL_SPI_TransmitReceive(&hspi2, &dummy, &response, 1, 100);  // sync after CS low
    response = SD_SendCommand(cmd17, sizeof(cmd17));

    if (response != 0x00) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, SET);
        return 1;  // first need to check whether the SD card has response
    }

    // then wait for FE which means SD card starts to report 
    for (int i = 0; i < 10000; i++) {
        HAL_SPI_TransmitReceive(&hspi2, &dummy, &response, 1, 100);
        if (response == 0xFE) {
            break;
        }
    }

    if (response != 0xFE) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, SET);
        return 1;  // failed to report
    }

    HAL_SPI_Receive(&hspi2, buffer, 512, 5000); // receive and get data from card

    // consume 2-byte CRC (must read these or next command will be corrupted)
    // Every SD card data block is: 0xFE + 512 data bytes + 2 CRC bytes
    HAL_SPI_TransmitReceive(&hspi2, &dummy, &response, 1, 100);
    HAL_SPI_TransmitReceive(&hspi2, &dummy, &response, 1, 100);

    // deselect card
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, SET);
    HAL_SPI_Transmit(&hspi2, &dummy, 1, 100);  // extra clock to finish
    return 0;
}

// init SD card and read FAT32 boot sector
int initSDCard(void) {
    uint8_t response;
    uint8_t dummy = 0xFF;
    uint8_t cmd0[] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
    uint8_t cmd8[] = {0x48, 0x00, 0x00, 0x01, 0xAA, 0x87};
    uint8_t cmd55[] = {0x77, 0x00, 0x00, 0x00, 0x00, 0x65};
    uint8_t acmd41[] = {0x69, 0x40, 0x00, 0x00, 0x00, 0x77};
    
    // wait for power
    HAL_Delay(500);  

    // send clock impluse (requirement -> 74+ cycle)
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, SET);
    for (int i = 0; i < 10; i++) {
        HAL_SPI_TransmitReceive(&hspi2, &dummy, &response, 1, 100);
    }

    // CMD0: go idle
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, RESET);
    HAL_SPI_TransmitReceive(&hspi2, &dummy, &response, 1, 100);
    response = SD_SendCommand(cmd0, sizeof(cmd0));
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, SET);
    HAL_SPI_Transmit(&hspi2, &dummy, 1, 100);
    if (response != 0x01) {
        return 1;
    }

    // CMD8: check voltage
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, RESET);
    HAL_SPI_TransmitReceive(&hspi2, &dummy, &response, 1, 100);
    response = SD_SendCommand(cmd8, sizeof(cmd8));
    for (int i = 0; i < 4; i++) {
        HAL_SPI_TransmitReceive(&hspi2, &dummy, &response, 1, 100);
    }
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, SET);
    HAL_SPI_Transmit(&hspi2, &dummy, 1, 100);

    // init card
    for (int retry = 0; retry < 100; retry++) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, RESET);
        HAL_SPI_TransmitReceive(&hspi2, &dummy, &response, 1, 100);
        SD_SendCommand(cmd55, sizeof(cmd55));
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, SET);
        HAL_SPI_Transmit(&hspi2, &dummy, 1, 100);

        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, RESET);
        HAL_SPI_TransmitReceive(&hspi2, &dummy, &response, 1, 100);
        response = SD_SendCommand(acmd41, sizeof(acmd41));
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, SET);
        HAL_SPI_Transmit(&hspi2, &dummy, 1, 100);

        if (response == 0x00) { // card init ok
            if (SD_readData(0, SDBuffer) != 0) { // check data there
                return 2;  // MBR read failed, check whether it can read
            }

            // LBA start at offset 0x1C6 (little endian)  FAT32 partition starts at sector 64
            uint32_t partitionStart = SDBuffer[0x1C6] | (SDBuffer[0x1C7] << 8) | (SDBuffer[0x1C8] << 16) | (SDBuffer[0x1C9] << 24);

            HAL_Delay(10);
            for (int d = 0; d < 16; d++) {
                HAL_SPI_Transmit(&hspi2, &dummy, 1, 100);
            }

            // read FAT32 boot sector
            return SD_readData(partitionStart, SDBuffer);
        }
        HAL_Delay(10);
    }

    return 3;  // ACMD41 timeout
}
