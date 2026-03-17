#include <protection.h>
volatile float ADC_converted_value = 0.0f; // initialize global variable
volatile float ADC_voltage = 0.0f; // initialize global variable
volatile uint32_t ADC_dma_buffer[3] = {0}; // initialize DMA buffer
volatile uint32_t ADC_raw[3] = {0}; // safe copy of ADC values
volatile uint8_t ADC_data_ready = 0; // flag: new data available

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
    if (hadc->Instance == ADC1) {
        // copy all channels when DMA transfer is complete (safe to read)
        ADC_raw[0] = ADC_dma_buffer[0];
        ADC_raw[1] = ADC_dma_buffer[1];
        ADC_raw[2] = ADC_dma_buffer[2];
        ADC_data_ready = 1;
    }
}