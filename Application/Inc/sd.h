#ifndef __SD_H
#define __SD_H

#include <ff.h>
#include <hc04.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SD_MAX_FILES 32

typedef struct {
    char name[13]; /* 8.3 + null */
    uint32_t size;
} sd_file_info_t;

extern sd_file_info_t fileList[SD_MAX_FILES];
extern int fileCount;

// scan SD card root for .txt files and populate fileList/fileCount
void sd_list_txt_files(void);

// used to list all files in the SD card and print via bluetooth
void sd_list_files(void);

// DEBUG: used to print the content of a file, given the filename
void sd_print_file(const char *filename);

// DEBUG: used to write a string to a file, given the filename and text
void sd_write_file(const char *filename, const char *text);

// parse the data array in the given file, and store in MCU's RAM
void sd_parse_array(const char *filename);

#endif /* __SD_H */