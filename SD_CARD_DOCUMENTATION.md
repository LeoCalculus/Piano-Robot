# SD Card & File Transfer Module Documentation

## Overview

SD card support ported from the STM32F103 project to the STM32H533 Nucleo board. Uses SPI3 in SPI mode to communicate with an SD card, FatFs for the filesystem, and HC-04 Bluetooth (921600 baud) for wireless file transfer.

### Hardware Configuration

| Signal | Pin  | Peripheral |
|--------|------|------------|
| MOSI   | PC12 | SPI3       |
| MISO   | PB0  | SPI3       |
| SCK    | PB1  | SPI3       |
| CS     | PC14 | GPIO Output (Software CS) |
| BT TX  | PA9  | USART1     |
| BT RX  | PA10 | USART1     |

---

## Module: `fatfs_sd` (Low-Level SPI SD Card Driver)

**Files:** `Application/Src/fatfs_sd.c`, `Application/Inc/fatfs_sd.h`

Implements the low-level SPI communication protocol with the SD card. This is the bridge between FatFs `diskio` layer and the actual hardware.

### Configuration Defines

| Define            | Value          | Description                        |
|-------------------|----------------|------------------------------------|
| `SD_SPI_HANDLE`   | `hspi3`        | HAL SPI handle for SD card         |
| `SD_CS_GPIO_Port` | `GPIOC`        | GPIO port for chip select          |
| `SD_CS_Pin`       | `GPIO_PIN_14`  | GPIO pin for chip select (PC14)    |
| `SPI_TIMEOUT`     | `100`          | HAL SPI timeout in ms              |

### SD Command Defines

| Command | SD CMD | Description              |
|---------|--------|--------------------------|
| `CMD0`  | 0      | GO_IDLE_STATE            |
| `CMD1`  | 1      | SEND_OP_COND             |
| `CMD8`  | 8      | SEND_IF_COND             |
| `CMD9`  | 9      | SEND_CSD                 |
| `CMD10` | 10     | SEND_CID                 |
| `CMD12` | 12     | STOP_TRANSMISSION        |
| `CMD16` | 16     | SET_BLOCKLEN             |
| `CMD17` | 17     | READ_SINGLE_BLOCK        |
| `CMD18` | 18     | READ_MULTIPLE_BLOCK      |
| `CMD23` | 23     | SET_BLOCK_COUNT          |
| `CMD24` | 24     | WRITE_BLOCK              |
| `CMD25` | 25     | WRITE_MULTIPLE_BLOCK     |
| `CMD41` | 41     | SEND_OP_COND (ACMD)      |
| `CMD55` | 55     | APP_CMD                  |
| `CMD58` | 58     | READ_OCR                 |

### Functions

#### `DSTATUS SD_disk_initialize(BYTE drv)`
Initializes the SD card via SPI. Sends the power-on sequence (80+ clock cycles with CS high), then performs the SD card initialization handshake: CMD0 (idle), CMD8 (voltage check), ACMD41 (activate), CMD58 (read OCR). Detects card type (SDv1, SDv2, SDHC).
- **Parameters:** `drv` - Drive number (must be 0)
- **Returns:** Disk status flags (`STA_NOINIT` cleared on success)

#### `DSTATUS SD_disk_status(BYTE drv)`
Returns the current disk status flags.
- **Parameters:** `drv` - Drive number (must be 0)
- **Returns:** Current `DSTATUS` flags

#### `DRESULT SD_disk_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count)`
Reads one or more 512-byte sectors from the SD card. Uses CMD17 for single-block reads and CMD18+CMD12 for multi-block reads.
- **Parameters:** `pdrv` - Drive number, `buff` - Output buffer, `sector` - Starting sector, `count` - Number of sectors
- **Returns:** `RES_OK` on success, `RES_ERROR`/`RES_PARERR`/`RES_NOTRDY` on failure

#### `DRESULT SD_disk_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)`
Writes one or more 512-byte sectors to the SD card. Uses CMD24 for single-block and CMD25 for multi-block writes.
- **Parameters:** `pdrv` - Drive number, `buff` - Input data, `sector` - Starting sector, `count` - Number of sectors
- **Returns:** `RES_OK` on success

#### `DRESULT SD_disk_ioctl(BYTE drv, BYTE ctrl, void *buff)`
Handles disk control operations. Supports `GET_SECTOR_COUNT`, `GET_SECTOR_SIZE`, `CTRL_SYNC`, `MMC_GET_CSD`, `MMC_GET_CID`, `MMC_GET_OCR`, and `CTRL_POWER`.
- **Parameters:** `drv` - Drive number, `ctrl` - Control code, `buff` - Parameter buffer
- **Returns:** `RES_OK` on success

---

## Module: `sdcard` (High-Level SD Card Operations)

**Files:** `Application/Src/sdcard.c`, `Application/Inc/sdcard.h`

Provides high-level file operations on top of FatFs. Manages TXT file listing and reading for the song selection menu.

### Configuration

| Define             | Value | Description                          |
|--------------------|-------|--------------------------------------|
| `MAX_FILES`        | 100   | Maximum number of files in listing   |
| `MAX_FILENAME_LEN` | 13    | 8.3 filename + null terminator       |

### Types

#### `FileEntry`
```c
typedef struct {
    char     name[MAX_FILENAME_LEN];  // 8.3 filename
    uint32_t size;                     // File size in bytes
} FileEntry;
```

#### `SD_StorageInfo`
```c
typedef struct {
    uint32_t totalKB;  // Total storage in KB
    uint32_t freeKB;   // Free storage in KB
} SD_StorageInfo;
```

### Global Variables

| Variable    | Type                       | Description                     |
|-------------|----------------------------|---------------------------------|
| `fatFs`     | `FATFS`                    | FatFs filesystem object         |
| `fileList`  | `FileEntry[MAX_FILES]`     | Array of discovered TXT files   |
| `fileCount` | `uint8_t`                  | Number of TXT files found       |

### Functions

#### `FRESULT SD_Init(void)`
Mounts the SD card filesystem using `f_mount()` with immediate mount flag.
- **Returns:** `FR_OK` on success, FatFs error code on failure

#### `FRESULT SD_Mount(void)` / `FRESULT SD_Unmount(void)`
Mount or unmount the SD card filesystem.
- **Returns:** FatFs result code

#### `FRESULT SD_OpenFile(FIL* file, const char* path, BYTE mode)`
Opens a file on the SD card. Wraps `f_open()`.
- **Parameters:** `file` - FatFs file object, `path` - File path, `mode` - Open mode flags (e.g., `FA_READ`, `FA_WRITE | FA_CREATE_NEW`)
- **Returns:** FatFs result code

#### `FRESULT SD_CloseFile(FIL* file)`
Closes an open file. Wraps `f_close()`.

#### `FRESULT SD_ReadFile(FIL* file, void* buff, UINT btr, UINT* br)`
Reads data from an open file. Wraps `f_read()`.
- **Parameters:** `file` - File object, `buff` - Output buffer, `btr` - Bytes to read, `br` - Pointer to actual bytes read

#### `FRESULT SD_WriteFile(FIL* file, const void* buff, UINT btw, UINT* bw)`
Writes data to an open file. Wraps `f_write()`.
- **Parameters:** `file` - File object, `buff` - Data to write, `btw` - Bytes to write, `bw` - Pointer to actual bytes written

#### `int SD_ListTxtFiles(void)`
Scans the root directory of the SD card for `.TXT` files (case-insensitive extension check). Populates the global `fileList[]` array and updates `fileCount`.
- **Returns:** Number of TXT files found

#### `int SD_ReadTxtFile(uint8_t fileIndex, uint8_t* buffer, uint32_t maxSize)`
Reads an entire TXT file into a buffer by its index in `fileList[]`.
- **Parameters:** `fileIndex` - Index into `fileList[]`, `buffer` - Output buffer, `maxSize` - Maximum bytes to read
- **Returns:** Number of bytes read, or `-1` on error

#### `FRESULT SD_GetStorageInfo(SD_StorageInfo* info)`
Queries the SD card for total and free storage capacity.
- **Parameters:** `info` - Pointer to `SD_StorageInfo` struct to fill
- **Returns:** FatFs result code

---

## Module: `file_transfer` (Binary File Transfer Protocol)

**Files:** `Application/Src/file_transfer.c`, `Application/Inc/file_transfer.h`

Implements a chunked binary file transfer protocol over HC-04 Bluetooth at 921600 baud. Allows a PC/phone app to upload TXT files to the SD card reliably with checksums, ACK/NAK, and timeout handling.

### Protocol Overview

```
PC/Phone                    STM32 (HC-04)
   |                            |
   |--- FILE_START (0xF0) ---->|  (filename, size, chunks)
   |<--- READY (0xA2) ---------|
   |                            |
   |--- FILE_DATA (0xF1) ----->|  (chunk index + data + checksum)
   |<--- ACK (0xA0) -----------|  (chunk index)
   |     ... repeat ...         |
   |                            |
   |--- FILE_END (0xF2) ------>|  (total bytes for verification)
   |<--- SUCCESS (0xA4) -------|  (confirmed byte count)
```

### Command Codes (PC -> STM32)

| Code   | Name             | Description                           |
|--------|------------------|---------------------------------------|
| `0xF0` | `FT_CMD_FILE_START` | Start new file transfer            |
| `0xF1` | `FT_CMD_FILE_DATA`  | Send data chunk                    |
| `0xF2` | `FT_CMD_FILE_END`   | End transfer, verify byte count    |
| `0xF3` | `FT_CMD_FILE_ABORT` | Abort current transfer             |

### Response Codes (STM32 -> PC)

| Code   | Name             | Description                           |
|--------|------------------|---------------------------------------|
| `0xA0` | `FT_RSP_ACK`    | Chunk received OK (contains chunk index) |
| `0xA1` | `FT_RSP_NAK`    | Chunk rejected (contains error code)     |
| `0xA2` | `FT_RSP_READY`  | Ready to receive data chunks             |
| `0xA3` | `FT_RSP_ERROR`  | Fatal error, transfer failed             |
| `0xA4` | `FT_RSP_SUCCESS`| File saved successfully                  |

### Error Codes

| Code   | Name                | Description                          |
|--------|---------------------|--------------------------------------|
| `0x00` | `FT_ERR_NONE`       | No error                             |
| `0x01` | `FT_ERR_CHECKSUM`   | XOR checksum mismatch                |
| `0x02` | `FT_ERR_SEQUENCE`   | Unexpected chunk index               |
| `0x03` | `FT_ERR_SD_WRITE`   | SD card write failure                |
| `0x04` | `FT_ERR_SD_FULL`    | SD card full                         |
| `0x05` | `FT_ERR_FILE_EXISTS`| File already exists                  |
| `0x06` | `FT_ERR_INVALID_NAME`| Invalid filename                    |
| `0x07` | `FT_ERR_TIMEOUT`    | 5-second inactivity timeout          |
| `0x08` | `FT_ERR_OVERFLOW`   | Packet too short / buffer overflow   |

### Packet Formats

**FILE_START (0xF0) - 18 bytes:**
```
[0xF0] [sizeL] [sizeH] [chunksL] [chunksH] [name 8 bytes] [ext 4 bytes] [checksum]
```

**FILE_DATA (0xF1) - variable length:**
```
[0xF1] [chunkL] [chunkH] [dataLen] [data...] [checksum]
```
Max data per chunk: 64 bytes (`FT_MAX_CHUNK_SIZE`)

**FILE_END (0xF2) - 4 bytes:**
```
[0xF2] [bytesL] [bytesH] [checksum]
```

**FILE_ABORT (0xF3) - 2 bytes:**
```
[0xF3] [checksum]
```

### Transfer State Machine

| State              | Description                                  |
|--------------------|----------------------------------------------|
| `FT_STATE_IDLE`    | No transfer in progress, waiting for FILE_START |
| `FT_STATE_RECEIVING` | Actively receiving data chunks              |
| `FT_STATE_COMPLETE`  | Transfer completed successfully (transient) |
| `FT_STATE_ERROR`     | Transfer failed (transient)                 |

### Functions

#### `void FT_Init(void)`
Initializes the file transfer module. Resets context to idle state and clears the write buffer.

#### `void FT_ProcessPacket(uint8_t* packet, uint16_t length)`
Main packet router. Dispatches received binary packets to the appropriate handler based on the command byte (0xF0-0xF3).
- **Parameters:** `packet` - Raw received packet, `length` - Packet length in bytes

#### `void FT_TimeoutCheck(void)`
Called periodically from the main loop. If a transfer has been idle for more than 5 seconds (`FT_TIMEOUT_MS`), aborts the transfer and sends a timeout error response. Should be called every iteration of the main loop.

#### `void FT_Abort(void)`
Aborts the current transfer. Closes and deletes the partially written file, resets state to idle.

#### `FT_State_t FT_GetState(void)`
Returns the current transfer state machine state.
- **Returns:** `FT_STATE_IDLE`, `FT_STATE_RECEIVING`, `FT_STATE_COMPLETE`, or `FT_STATE_ERROR`

#### `uint8_t FT_GetProgress(void)`
Returns the transfer progress as a percentage (0-100).
- **Returns:** Progress percentage based on chunks received vs total chunks

#### `uint8_t FT_CalculateChecksum(uint8_t* data, uint16_t length)`
Calculates XOR checksum over a byte array. Used for all packet integrity verification.
- **Parameters:** `data` - Data to checksum, `length` - Number of bytes
- **Returns:** XOR checksum byte

#### `void FT_SendResponse(uint8_t responseCode, uint8_t* data, uint8_t dataLen)`
Builds and sends a response packet via HC-04 Bluetooth (blocking send). Appends XOR checksum automatically.
- **Parameters:** `responseCode` - One of `FT_RSP_*` codes, `data` - Optional payload, `dataLen` - Payload length

---

## Module: `hc04bt` (HC-04 Bluetooth Communication)

**Files:** `Application/Src/hc04bt.c`, `Application/Inc/hc04bt.h`

Handles communication with the HC-04 Bluetooth module over USART1 at 921600 baud rate.

### Functions

#### `void BT_config(UART_HandleTypeDef* huart)`
Configures the HC-04 module. Sends `AT` to check presence, then `AT+BAUD=921600` to set the baud rate. Displays status on the ILI9341 LCD. Note: HC-04 auto-restarts on baud rate change, so no further UART commands will work after this function.
- **Parameters:** `huart` - UART handle (USART1)

#### `void BT_send_info(UART_HandleTypeDef* huart, uint8_t* info, uint16_t info_size)`
Non-blocking send via interrupt mode (`HAL_UART_Transmit_IT`). Checks UART state before transmitting to avoid conflicts with DMA reception.
- **Parameters:** `huart` - UART handle, `info` - Data to send, `info_size` - Data length

#### `int HC04_SendBytes(UART_HandleTypeDef* huart, uint8_t* buffer, uint16_t len)`
Blocking send for file transfer responses. Ensures the response is fully transmitted before returning. Timeout scales with data length for reliability at 921600 baud.
- **Parameters:** `huart` - UART handle, `buffer` - Data to send, `len` - Data length
- **Returns:** `1` on success, `0` on failure

#### `int HC04_SendBytes_IT(UART_HandleTypeDef* huart, uint8_t* buffer, uint16_t len)`
Non-blocking send with return status. Checks UART state before initiating interrupt-based transmit.
- **Parameters:** `huart` - UART handle, `buffer` - Data to send, `len` - Data length
- **Returns:** `1` if transmit started, `0` if UART was busy

#### `void HC04_RestartDMA(UART_HandleTypeDef* huart, uint8_t* dma_buffer, uint16_t size)`
Aborts current DMA reception and restarts it. Used to recover from DMA errors or to reset the receive buffer.
- **Parameters:** `huart` - UART handle, `dma_buffer` - DMA receive buffer, `size` - Buffer size

---

## Module: `command` (Command Parser)

**Files:** `Application/Src/command.c`, `Application/Inc/command.h`

Parses and executes text commands received via Bluetooth. Also routes binary file transfer packets.

### Short Commands (`:` prefix)

| Command | Description                          |
|---------|--------------------------------------|
| `:p<N>` | Set target position to N mm          |
| `:w`    | Navigate menu up                     |
| `:s`    | Navigate menu down                   |
| `:e`    | Enter / select current menu item     |
| `:b`    | Go back to parent menu               |
| `:r`    | Reset encoder position to 0          |

### Slash Commands (`/` prefix)

| Command          | Description                                    |
|------------------|------------------------------------------------|
| `/setText <text>` | Display text on LCD row 13                    |
| `/setPos <float>` | Set target position in cm                     |
| `/resetPos`       | Reset encoder position to 0                   |
| `/listFiles`      | List SD card TXT files over Bluetooth          |
| `/uploadAbort`    | Abort current file transfer                    |

### Functions

#### `void executeCommandWithLen(uint8_t* command, uint16_t len)`
Main entry point for all received data. Checks if the first byte is a binary file transfer command (0xF0-0xF3) and routes to `FT_ProcessPacket()`. Otherwise falls through to text command parsing via `executeCommand()`.
- **Parameters:** `command` - Raw received data, `len` - Data length

#### `void executeCommand(uint8_t* command)`
Parses a text command and displays the result on the LCD (OK, Cannot parse, Empty command, or Incomplete command).
- **Parameters:** `command` - Null-terminated command string

#### `int parseCommand(uint8_t* cmd)`
Core command parser. Handles both `:` short commands and `/` slash commands.
- **Parameters:** `cmd` - Command buffer
- **Returns:** `0` = OK, `1` = parse error, `2` = empty, `3` = incomplete

#### `void setText(uint8_t* displayedText)`
Draws text on LCD row 13.

#### `void setPos(float targetPos)`
Sets the `target_position_cm` global variable for the PID motor controller.

#### `void resetPos(TIM_HandleTypeDef* htim)`
Resets encoder position to 0 by clearing the timer counter and `encoder_old_position_cm`.

#### `void listTxtFiles(void)`
Scans SD card for TXT files and sends the list over Bluetooth in the format:
```
FILES:<count>\r\n
<filename>,<size>\r\n
...
END\r\n
```

---

## Module: `menu` (LCD Menu System)

**Files:** `Application/Src/menu.c`, `Application/Inc/menu.h`

Implements a two-level menu system on the ILI9341 LCD display, navigated via Bluetooth commands (`:w` up, `:s` down, `:e` enter, `:b` back).

### Menu Structure

```
Main Menu (menu_state = 0)
├── 1. Play Song
├── 2. Transmit Song
├── 3. Select Song  ──→  Select Song Submenu (menu_state = 1)
│                         ├── <TXT file 1>
│                         ├── <TXT file 2>
│                         └── ... (scrollable, 8 rows visible)
└── 4. Homing
```

### State Variables

| Variable              | Type  | Description                                |
|-----------------------|-------|--------------------------------------------|
| `menu_state`          | `int` | 0 = main menu, 1 = select song submenu    |
| `menu_index`          | `int` | Currently highlighted main menu item (0-3) |
| `menu_move_down`      | `int` | Flag: navigate down (set by `:s` command)  |
| `menu_move_up`        | `int` | Flag: navigate up (set by `:w` command)    |
| `menu_enter`          | `int` | Flag: enter/select (set by `:e` command)   |
| `menu_back`           | `int` | Flag: go back (set by `:b` command)        |
| `song_scroll_offset`  | `int` | First visible song index in scrolling list |
| `song_selected_index` | `int` | Currently highlighted song in list         |

### Functions

#### `void menu_init(void)`
Initializes the menu system. Draws the title "ELEC391 Piano Bot" and shows the main menu with item 1 selected.

#### `void menu_update(void)`
Main menu state machine. Called each iteration of the main loop. Reads the navigation flags, updates state, and redraws the appropriate menu. Resets all flags after processing.

#### `void menu_index_0()` / `menu_index_1()` / `menu_index_2()` / `menu_index_3()`
Draws the main menu with the `>` cursor on item 1, 2, 3, or 4 respectively.

#### `void menu_select_song_enter(void)`
Enters the Select Song submenu. Calls `SD_ListTxtFiles()` to scan the SD card, clears the menu area, and draws the song list header. If no files are found, displays a "No songs found!" message.

#### `void menu_select_song_update(void)`
Handles up/down navigation and enter within the song list. Supports wrapping around at top/bottom and automatic scrolling when the selection moves outside the visible 8-row window.

#### `void menu_select_song_draw(void)`
Renders the visible portion of the song list on the LCD (rows 2-9). Shows a `>` indicator next to the selected song. Handles cases where the list is shorter than the display area.

---

## FatFs Library

**Files:** `Application/FatFs/ff.c`, `Application/FatFs/ff.h`, `Application/FatFs/ffconf.h`, `Application/FatFs/diskio.c`, `Application/FatFs/diskio.h`, `Application/FatFs/integer.h`

Chan's FatFs R0.11 generic FAT filesystem module. Configuration in `ffconf.h`:

| Setting          | Value | Description                              |
|------------------|-------|------------------------------------------|
| `_FS_READONLY`   | 0     | Read/write enabled                       |
| `_FS_MINIMIZE`   | 0     | Full API available                       |
| `_USE_STRFUNC`   | 0     | No string functions                      |
| `_USE_MKFS`      | 0     | No mkfs support                          |
| `_USE_FASTSEEK`  | 0     | No fast seek                             |
| `_USE_LFN`       | 0     | No long filenames (8.3 only)             |
| `_CODE_PAGE`     | 437   | US English code page                     |
| `_VOLUMES`       | 1     | Single volume                            |
| `_MAX_SS`        | 512   | 512-byte sector size                     |

---

## CubeMX Changes Required

After porting, these CubeMX settings need to be updated in `h533.ioc`:

| Peripheral | Setting      | Current Value | Required Value | Notes                              |
|------------|-------------|---------------|----------------|------------------------------------|
| SPI3       | Data Size   | 4 Bits        | **8 Bits**     | SD card requires 8-bit SPI frames  |
| SPI3       | NSSP Mode   | Enabled       | **Disabled**   | Using software CS on PB14          |

All other peripherals (USART1 @ 921600, SPI3 pins, PB14 GPIO, GPDMA) are already correctly configured.
