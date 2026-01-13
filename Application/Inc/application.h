#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <utils.h>
#include <oled1315.h>
#include <mpu6050sensor.h>
#include <hc05bt.h>
#include <config.h>
#include <command.h>
#include "usart.h"

#define VOFA_TAIL {0x00, 0x00, 0x80, 0x7f}

typedef struct __attribute__((packed)) VOFA_REPORT{
    float val[10];  // Only 3 channels for accel X/Y/Z
    unsigned char vofaTail[4];
}VOFA_REPORT;

extern VOFA_REPORT vofa;

void controllerUpdate(const float dt);

void init();

#endif