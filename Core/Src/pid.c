#include "main.h"
#include "tim.h"
#include "usart.h"
#include "pid.h"
#include "elec391.h"
#include "VOFA.h"
#include <stdlib.h>

#define I_MAX  1000
#define I_MIN -1000

#define I_ON_ZONE 1500

#define PWM_DEAD_ZONE 3
#define PWM_DEAD_ZONE_OFFSET 70

#define DEAD_BAND 700

void pid_control(void){

    int32_t pout_1000, dout_1000, iout_1000;
    int32_t pid_pwm;
    int32_t pid_temp = 500;
    uint8_t i_flag;

    err_1000 = target_1000 - location_1000;
    err_acc_1000 += err_1000;

    // Dead band
    if(abs(err_1000) < DEAD_BAND){
        err_acc_1000 = 0;
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500);
        VOFA_JustFloat_Send(&huart2, (float)location_1000, (float)target_1000, 500.0, 0, 0, 0);
        return;
    }

    // PD: for begining, PID for almost there
    if (abs(err_1000) < I_ON_ZONE) {
        err_acc_1000 += err_1000;
        // limit for integrals
        if (err_acc_1000 > I_MAX) err_acc_1000 = I_MAX;
        if (err_acc_1000 < I_MIN) err_acc_1000 = I_MIN;
        i_flag = 1;
    } else {
        err_acc_1000 = 0;
        i_flag = 0;
    }

    pout_1000 = Kp * err_1000;
    iout_1000 = i_flag * Ki * err_acc_1000;
    dout_1000 = Kd * (err_1000 - err_prev_1000);
    
    pid_temp = (pout_1000 + iout_1000 + dout_1000)/12;
    pid_temp += 1000;
    pid_temp /= 2;

    // duty cycle offset
    if(pid_temp < 500 - PWM_DEAD_ZONE){
        pid_temp -= PWM_DEAD_ZONE_OFFSET;
    }
    else if(pid_temp > 500 + PWM_DEAD_ZONE){
        pid_temp += PWM_DEAD_ZONE_OFFSET;
    }
    else{
        pid_temp = 500;
    }

    // pwm limit
    if(pid_temp > 1000){
        pid_pwm = 1000;
    }
    else if (pid_temp < 0){
        pid_pwm = 0;
    }
    else {
        pid_pwm = pid_temp;
    }

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pid_pwm);
    err_prev_1000 = err_1000;

    VOFA_JustFloat_Send(&huart2, (float)location_1000, (float)target_1000, (float)pid_pwm, (float)pout_1000, (float)iout_1000, (float)dout_1000);
    return;
}
