/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "gpdma.h"
#include "i2c.h"
#include "icache.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <application.h>
#include <ff.h>
#include <config.h>
#include <user_timer.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// lcd buffer variables 
char buffer_message[280] = {0};
char LCD_Send_buf[32];

// sd card variables
FATFS fs; // file system object
FIL file; // file object
FRESULT res; // result code from file operations
UINT bytes_written; // number of bytes written

// state machine variables
uint8_t state = 0; // state of the system

extern uint8_t menu_update_flag;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_GPDMA1_Init();
  MX_USART6_UART_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  MX_SPI4_Init();
  MX_TIM1_Init();
  MX_I2C2_SMBUS_Init();
  MX_SPI3_Init();
  MX_TIM3_Init();
  MX_ICACHE_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  // init HC-04 UART receive to idle mode with DMA
  hc04_receive_to_idle_init(&huart1, rx_message_buffer, sizeof(rx_message_buffer));
  // init LCD
  LCD_init();
  LCD_draw_string(0, 1, "Loading...       ", COLOR_BLACK, COLOR_WHITE);
  // calib the ADC
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  // start ADC with DMA (also enables conversion complete interrupt)
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_dma_buffer, 3);

#ifndef DEBUGMODE // debug mode do not use SD card or menu
  // SD card init
  hc04_send_string((uint8_t*)"Mounting SD card...");
  HAL_Delay(1000);
  res = f_mount(&fs, "", 1);
  HAL_Delay(200);
  if (res != FR_OK) {
    snprintf(buffer_message, sizeof(buffer_message), "Failed to mount SD card: %d", res);
    hc04_send_string((uint8_t*)buffer_message);
  }
  else {
    hc04_send_string((uint8_t*)"SD card mounted successfully");
  }
  HAL_Delay(500);

  sd_list_files();

  sd_print_file("HEY.txt");

  sd_parse_array("music1.txt");
#endif
  // timer tick init
  user_timer_tick_init();

  // Timer5 0.5 ms for measurements
  HAL_TIM_Base_Start_IT(&htim5); 

  // Timer2 1 ms for general timing 
  HAL_TIM_Base_Start_IT(&htim2); 

  // Encoders
  encoder_start(&htim3); 
  encoder_start(&htim4); 

  // enable pwm:
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

  // initially not moving, so set compare to 500 (0% duty cycle)
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 500); 
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 500);

  // // test for current
  // HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
  // HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
  // HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
  // HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);

  controller_init(); // initialize controller state

#ifndef DEBUGMODE
  menu_init();
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) 
  {
    // toggle LED pin to show main loop is not blocking 
    // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);

#ifndef DEBUGMODE
    // check incoming message with \n:
    if (rx_complete) {
      menu_process_message(rx_message, rx_valid);
    }
    rx_complete = 0;

    // ISR accumulated bytes without \n
    if (rx_message_index > 0){
      if (menu_try_dispatch_binary(rx_message, rx_message_index)) {
        rx_message_index = 0;
      }
    }

    // update menu display
    if(menu_update_flag == 1){
      menu_update();
      menu_update_flag = 0; 
    }
#else 
    // in debug mode just display the current position from encoder
    // disable the DEBUG on LCD rn
    char pos_buf[32];
    int32_t temp;
    encoder_read_value(&htim4, &temp);
    current_distance_mm = encoder_parse_distance_mm(temp);
    snprintf(pos_buf, sizeof(pos_buf), "Position cnt: %d mm", temp);
    LCD_draw_string(0, 1, pos_buf, COLOR_BLACK, COLOR_WHITE);
    

    // also read the ADC value:
    ADC_voltage = (float)(ADC_dma_buffer[0] + ADC_dma_buffer[1] + ADC_dma_buffer[2]) * 3.3f / (4095.0f * 50.0f); // convert ADC value to voltage
    const float ADC_current = ADC_voltage / 0.005f; // convert voltage to current (assuming 0.1 ohm shunt resistor)
    char adc_buf[32];
    snprintf(adc_buf, sizeof(adc_buf), "ADC current: %.4f A", ADC_current);
    LCD_draw_string(0, 2, adc_buf, COLOR_BLACK, COLOR_WHITE);
#endif
  }
  
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 125;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the programming delay
  */
  __HAL_FLASH_SET_PROGRAM_DELAY(FLASH_PROGRAMMING_DELAY_2);
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CKPER;
  PeriphClkInitStruct.CkperClockSelection = RCC_CLKPSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
