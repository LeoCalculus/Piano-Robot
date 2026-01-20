#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <utils.h>
#include <oled1315.h>
#ifdef USINGMPU6050
#include <mpu6050sensor.h>
#endif
#include <hc05bt.h>
#include <config.h>
#include <command.h>
#include <math.h>
#include <stdlib.h>
#include "usart.h"
#include <sdcard.h>
#include <fatfs_sd.h>
#include <ILI9341.h>

#define VOFA_TAIL {0x00, 0x00, 0x80, 0x7f}

// send from vofa to adjust pid, in this project, rn we only need tune pid for two hand's motor
#define CMD_SET_LEFT_P    0x01
#define CMD_SET_LEFT_I    0x02
#define CMD_SET_LEFT_D    0x03
#define CMD_SET_RIGHT_P   0x11
#define CMD_SET_RIGHT_I   0x12
#define CMD_SET_RIGHT_D   0x13

typedef struct __attribute__((packed)) VOFA_REPORT{
    float val[10];  // Only 3 channels for accel X/Y/Z
    unsigned char vofaTail[4];
}VOFA_REPORT;

extern VOFA_REPORT vofa;
extern uint8_t VOFA_SEND_BUFFER[64];
extern PID_t leftHandMotor;
extern PID_t rightHandMotor;

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

void controllerUpdate(const float dt);
void smoothVelocity(float currentPostion, float dt);
void boostKp(float targetPos, float currentPos, float tolerance, PID_t target); // speed up approaching

void init();

#endif