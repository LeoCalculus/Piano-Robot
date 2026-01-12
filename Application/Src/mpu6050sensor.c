#include <mpu6050sensor.h>
#include <stdint.h>
#include "i2c.h"

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

    // Wake up (disable sleep mode)
    data = 0x00;
    HAL_I2C_Mem_Write(&hi2c2, MPU6050_SLAVE_ADDRESS, 0x6B, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);

    // Sample rate divider
    data = 0x07;
    HAL_I2C_Mem_Write(&hi2c2, MPU6050_SLAVE_ADDRESS, 0x19, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);

    // DLPF config
    data = 0x06;
    HAL_I2C_Mem_Write(&hi2c2, MPU6050_SLAVE_ADDRESS, 0x1A, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);

    // Gyro config (±500°/s)
    data = 0x08;
    HAL_I2C_Mem_Write(&hi2c2, MPU6050_SLAVE_ADDRESS, 0x1B, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);

    // Accel config (±16g)
    data = 0x18;
    HAL_I2C_Mem_Write(&hi2c2, MPU6050_SLAVE_ADDRESS, 0x1C, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

void readAcc(int16_t* inputBuffer){
    uint8_t buffer[6]; // buffer to store the acceleration data
    HAL_I2C_Mem_Read(&hi2c2, MPU6050_SLAVE_ADDRESS, MPU6050_ACC_OUT, I2C_MEMADD_SIZE_8BIT, buffer, 6, 100);
    
    int16_t accelX = (buffer[0] << 8) | buffer[1];
    int16_t accelY = (buffer[2] << 8) | buffer[3];
    int16_t accelZ = (buffer[4] << 8) | buffer[5];
    inputBuffer[0] = accelX;
    inputBuffer[1] = accelY;
    inputBuffer[2] = accelZ;
}