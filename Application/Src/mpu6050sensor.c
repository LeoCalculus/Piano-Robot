#include <mpu6050sensor.h>
#include <stdint.h>
#include "i2c.h"

float accData[3];
float gyroData[3];

int checkExist(){
    uint8_t readResult = 0;
    // read device's who am i register
    HAL_I2C_Mem_Read(&hi2c2, MPU6050_SLAVE_ADDRESS, MPU6050_WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &readResult, 1, 100);
    if (readResult != 0x68){
        return 1;
    } else {
        return 0;
    }
}

void mpu6050Init(){
    HAL_Delay(100); // wait for power
    uint8_t data;
    // wake up 
    data = 0x00;
    HAL_I2C_Mem_Write(&hi2c2, MPU6050_SLAVE_ADDRESS, 0x6B, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    // sample rate divider
    data = 0x07;
    HAL_I2C_Mem_Write(&hi2c2, MPU6050_SLAVE_ADDRESS, 0x19, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    // DLPF config
    data = 0x06;
    HAL_I2C_Mem_Write(&hi2c2, MPU6050_SLAVE_ADDRESS, 0x1A, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    // gyro config
    data = 0x08;
    HAL_I2C_Mem_Write(&hi2c2, MPU6050_SLAVE_ADDRESS, 0x1B, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    // cccel config
    data = 0x18;
    HAL_I2C_Mem_Write(&hi2c2, MPU6050_SLAVE_ADDRESS, 0x1C, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

void readAcc(){ // store data in global variable
    uint8_t buffer[6]; // buffer to store the acceleration data
    HAL_I2C_Mem_Read(&hi2c2, MPU6050_SLAVE_ADDRESS, MPU6050_ACC_OUT, I2C_MEMADD_SIZE_8BIT, buffer, 6, 100);
    // divide 2048 in terms of g, expected: accelZ should show appro 1g
    // must cast here
    accData[0] = (int16_t)((buffer[0] << 8) | buffer[1]) / 2048.0f;
    accData[1] = (int16_t)((buffer[2] << 8) | buffer[3]) / 2048.0f;
    accData[2] = (int16_t)((buffer[4] << 8) | buffer[5]) / 2048.0f;

    HAL_I2C_Mem_Read(&hi2c2, MPU6050_SLAVE_ADDRESS, MPU6050_GYRO_OUT, I2C_MEMADD_SIZE_8BIT, buffer, 6, 100);
    gyroData[0] = (int16_t)((buffer[0] << 8) | buffer[1]) / 65.5f * 0.0174533f;
    gyroData[1] = (int16_t)((buffer[2] << 8) | buffer[3]) / 65.5f * 0.0174533f;
    gyroData[2] = (int16_t)((buffer[4] << 8) | buffer[5]) / 65.5f * 0.0174533f;

}