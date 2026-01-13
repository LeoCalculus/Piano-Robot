#ifndef __GY87SENSOR_H
#define __GY87SENSOR_H

#include <stdint.h>

#define MPU6050_SLAVE_ADDRESS  (0x68<<1)      //MPU6050器件读地址

#define MPU6050_WHO_AM_I        0x75
#define MPU6050_SMPLRT_DIV      0  
#define MPU6050_DLPF_CFG        0
#define MPU6050_GYRO_OUT        0x43 
#define MPU6050_ACC_OUT         0x3B 

int checkExist();
void mpu6050Init();
void readAcc();
extern float accData[3];
extern float gyroData[3];

#endif