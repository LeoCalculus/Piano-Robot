#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <utils.h>
#include <oled1315.h>
#include <mpu6050sensor.h>
#include <hc05bt.h>
#include <config.h>
#include <command.h>
#include <math.h>
#include <stdlib.h>
#include "usart.h"

#define VOFA_TAIL {0x00, 0x00, 0x80, 0x7f}

typedef struct __attribute__((packed)) VOFA_REPORT{
    float val[10];  // Only 3 channels for accel X/Y/Z
    unsigned char vofaTail[4];
}VOFA_REPORT;

extern VOFA_REPORT vofa;

// Mahony for pos
typedef struct {
    float q[4];
    float integralFB[3];

    float Kp;
    float Kp_max;
    float Ki;
    float Kd;          

    float roll;
    float pitch;
    float yaw;
} MahonyAHRS;

typedef struct 
{
    float lastPosition; // mm

    float velocityLPF; // mm/s
    float velocityAlpha; // 

} controllerProperty;


extern MahonyAHRS ahrs;
extern controllerProperty cp;

void Mahony_Init(MahonyAHRS *mahony, float Kp, float Ki);
void Mahony_Update(MahonyAHRS *mahony, const float acc[3], const float gyro[3], float dt);

void controllerUpdate(const float dt);
void smoothVelocity(float currentPostion, float dt);
void boostKp(float targetPos, float currentPos, float tolerance, PID_t target); // speed up approaching

void init();

#endif