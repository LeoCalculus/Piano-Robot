#include <protection.h>
volatile float ADC_converted_value = 0.0f; // initialize global variable
volatile float ADC_voltage = 0.0f; // initialize global variable
volatile uint32_t ADC_dma_buffer[3] = {0}; // initialize DMA buffer


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
    if (hadc->Instance == ADC1) {
        // only read one for now:
        ADC_converted_value = ADC_dma_buffer[0]; // read the converted value
    }

}