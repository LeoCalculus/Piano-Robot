#ifndef __PROTECTION_H
#define __PROTECTION_H

#include <adc.h>
#include <stdint.h>
#include <stm32h5xx_it.h>
#include <stm32h5xx_hal_adc.h>

extern volatile float ADC_converted_value; // global variable to store ADC value
extern volatile float ADC_voltage; // global variable to store converted voltage
extern volatile uint32_t ADC_dma_buffer[1]; // DMA buffer for ADC readings

// use the call back function to handle ADC conversion complete interrupt
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

#endif