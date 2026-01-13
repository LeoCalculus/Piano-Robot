#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <utils.h>
#include <oled1315.h>
#include <mpu6050sensor.h>
#include <hc05bt.h>
#include <config.h>
#include <command.h>
#include <math.h>
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

    float integral_limit;
    float accel_reject_thresh;
    float gyro_boost_thresh;
    float boost_duration;
    float boost_timer;
    float last_gyro_norm;

    float gyro_drop_trigger;
    float gyro_dot_trigger;
    float centripetal_omega_thresh;
    float centripetal_trust;

    float accel_norm_lpf;
    float accel_lpf_alpha;

    // gyro LPF to reduce high-frequency noise
    float gyro_lpf[3];
    float gyro_lpf_alpha;

    // fast settle
    float fast_settle_timer;
    float fast_settle_duration;

    // Kp smoothing state
    float kp_state;
    float kp_ramp_time;

    // last error and edot LPF
    float last_ex, last_ey, last_ez;
    float edot_lpf_x, edot_lpf_y, edot_lpf_z;
    float edot_lpf_alpha;

    // error LPF (to remove step-injection into correction)
    float err_lpf_x, err_lpf_y, err_lpf_z;
    float err_lpf_alpha;

    // boost shape power for smooth decay
    float boost_shape_power;

    float roll;
    float pitch;
    float yaw;
} MahonyAHRS;

extern MahonyAHRS ahrs;

void Mahony_Init(MahonyAHRS *mahony, float Kp, float Ki);
void Mahony_Update(MahonyAHRS *mahony, const float acc[3], const float gyro[3], float dt);

void controllerUpdate(const float dt);

void init();

#endif