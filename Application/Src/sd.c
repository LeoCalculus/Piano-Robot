#include <sd.h>

static char sd_buf[128];
static char sd_msg[280];

sd_file_info_t fileList[SD_MAX_FILES];
int fileCount = 0;

void sd_list_txt_files(void) {
    DIR dir;
    FILINFO fno;
    FRESULT res;

    fileCount = 0;
    res = f_opendir(&dir, "/");
    if (res != FR_OK) return;

    while (fileCount < SD_MAX_FILES) {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) break;
        if (fno.fattrib & AM_DIR) continue;

        /* Check for .txt extension */
        const char *dot = strrchr(fno.fname, '.');
        if (dot && (strcmp(dot, ".txt") == 0 || strcmp(dot, ".TXT") == 0)) {
            strncpy(fileList[fileCount].name, fno.fname, 12);
            fileList[fileCount].name[12] = '\0';
            fileList[fileCount].size = fno.fsize;
            fileCount++;
        }
    }
    f_closedir(&dir);
}

void sd_list_files(void) {
    DIR dir;
    FILINFO fno;
    FRESULT res;

    res = f_opendir(&dir, "/");
    if (res != FR_OK) {
        snprintf(sd_msg, sizeof(sd_msg), "opendir failed: %d", res);
        hc04_send_string((uint8_t*)sd_msg);
        return;
    }

    hc04_send_string((uint8_t*)"=== Files on SD ===");
    while (1) {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) break;

        snprintf(sd_msg, sizeof(sd_msg), "%s  %lu bytes\n",
                 fno.fname, fno.fsize);
        hc04_send_string((uint8_t*)sd_msg);
        HAL_Delay(50);
    }
    f_closedir(&dir);
}

void sd_print_file(const char *filename) {
    FIL file;
    FRESULT res;

    res = f_open(&file, filename, FA_READ);
    if (res != FR_OK) {
        snprintf(sd_msg, sizeof(sd_msg), "open failed: %d", res);
        hc04_send_string((uint8_t*)sd_msg);
        return;
    }

    snprintf(sd_msg, sizeof(sd_msg), "=== %s ===\n", filename);
    hc04_send_string((uint8_t*)sd_msg);

    // read file line by line
    while (f_gets(sd_buf, sizeof(sd_buf), &file) != NULL) {
        hc04_send_string((uint8_t*)sd_buf);
        HAL_Delay(50);
    }
    f_close(&file);
}

void sd_write_file(const char *filename, const char *text) {
    FIL file;
    FRESULT res;
    UINT bytes_written;

    res = f_open(&file, filename, FA_WRITE | FA_CREATE_ALWAYS);
    if (res != FR_OK) {
        snprintf(sd_msg, sizeof(sd_msg), "create failed: %d", res);
        hc04_send_string((uint8_t*)sd_msg);
        return;
    }

    f_write(&file, text, strlen(text), &bytes_written);
    f_close(&file);

    snprintf(sd_msg, sizeof(sd_msg), "Wrote %u bytes to %s", bytes_written, filename);
    hc04_send_string((uint8_t*)sd_msg);
}

void sd_parse_array(const char *filename){
    FIL file;
    FRESULT res;
    int index = 0; // used to index for chord_events array

    res = f_open(&file, filename, FA_READ);
    if (res != FR_OK) {
        snprintf(sd_msg, sizeof(sd_msg), "open failed: %d", res);
        hc04_send_string((uint8_t*)sd_msg);
        return;
    }

    // parse and store chord events in the file, format example:
    while (f_gets(sd_buf, sizeof(sd_buf), &file) != NULL) {
        // skip empty / whitespace-only lines
        int blank = 1;
        for (int i = 0; sd_buf[i]; i++) {
            if (sd_buf[i] != ' ' && sd_buf[i] != '\t' && sd_buf[i] != '\r' && sd_buf[i] != '\n') {
                blank = 0;
                break;
            }
        }
        if (blank) continue;

        // 1.5 3.2 row_high row_low 1 0 1 0 0 1 0 0 1 0 500 as example, using strtok to parse the line
        char *token = strtok(sd_buf, " ");
        while (token != NULL) {
            // parse the token and store in chord_events[index]
            // for simplicity, we assume the format is always correct and there are always 14 tokens per line
            // in real application, you should add error checking here
            int token_index = 0;
            while (token != NULL) {
                if (token_index < 2) { // first 2 tokens are positions, using strtof to convert string to float
                    chord_events[index].positions[token_index] = strtof(token, NULL);
                } else if (token_index < 2 + MAX_CHORD_NOTES) {
                    // use atoi to convert "0" or "1" to int, and then store as bool
                    chord_events[index].pressed[token_index - 2] = atoi(token) != 0;
                } else if (token_index == 2 + MAX_CHORD_NOTES) {
                    chord_events[index].duration_ms = atoi(token);
                } else if (token_index == 3 + MAX_CHORD_NOTES) {
                    chord_events[index].long_pressed[0] = atoi(token) != 0;
                } else if (token_index == 4 + MAX_CHORD_NOTES) {
                    chord_events[index].long_pressed[1] = atoi(token) != 0;
                }
                token = strtok(NULL, " ");
                token_index++;
            }
        }

        index++;
        HAL_Delay(50);
    }
    f_close(&file);

}