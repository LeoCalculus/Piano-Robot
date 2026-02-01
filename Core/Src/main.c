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
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define GLOBAL_DEFINITION
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "hc04.h"
#include "oled.h"
#include "user_uart_isr.h"
#include "user_timer_isr.h"
#include "elec391.h"
#include "VOFA.h"
#include "encoder.h"
#include "notes.h"
#include "pid.h"

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  OLED_Init();
  // BT_SetBaudRate_115200(&huart1);
  HAL_Delay(500);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	// UART for HC04
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, BT_DMA_rx_buff, BT_RX_LEN);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, BT_DMA_rx_buff, BT_RX_LEN);
	
	// timer2 for general timing: tik = 1ms
  HAL_TIM_Base_Start_IT(&htim2);

  // timer4 for measurement: tik = 0.5ms
  HAL_TIM_Base_Start_IT(&htim4);
	
	// pwm_ch1 
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  // pwm_ch1n 
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);

  // pwm_ch2 
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  // pwm_ch2n 
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);

  // encoder mode 
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	
	// initialazation

  // scaling for timer 2
  msec_count_frame = 0;
  msec_count_limit = 0;

  // scaling for timer 4
  half_msec_count_measure = 0;

  main_update = 0;
  limit_count_en = 0;
  BT_cmd_type = 0;
  COM_cmd_type = 0;

  speed_10000 = 0;
  rad_s_10000 = 0;
  location_1000 = 0;

  err_prev_1000 = 0;
  err_acc_1000 = 0;
  err_1000 = 0;

  counter_acc = 0;

  Kp = 0.05;
  Ki = 0.03;
  Kd = 0.130;

  float dummy;

  while (1){

    // only update if timer2 ask for it
    if(main_update == 1){
      
      //                            1234567890123456
      OLED_Printf(0, 0, OLED_8X16, "S: %7.3f rpm", speed_10000/10000.0);
      OLED_Printf(0, 16, OLED_8X16, "L: %7.3f deg", location_1000/1000.0);
      OLED_Printf(0, 32, OLED_8X16, "T: %7.3f deg", target_1000/1000.0);
      OLED_Printf(0, 48, OLED_6X8, "%5.3f %5.3f %5.3f", Kp, Ki, Kd);
      
      // limit button detection
      if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET){
        msec_count_limit = 0;
      }
      while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET){
        limit_count_en = 1;
      }
      limit_count_en = 0;

      if(msec_count_limit>50){
        counter_acc = 0;
        msec_count_limit = 0;
      }

      // COM cmd handle
      if(COM_cmd_type == 1){
        
        OLED_Printf(0, 0, 8, "%s", COM_DMA_rx_buff+1); // get rid of type indicator
        COM_cmd_type = 0;
      }
      else if(COM_cmd_type == 2){

        // handling pid tuning here
        sscanf((char*)COM_DMA_rx_buff, ";A%f", &Kp);
        OLED_Printf(0, 0, 8, "%s", COM_DMA_rx_buff+1); // get rid of type indicator
        COM_cmd_type = 0;
      }
      else if(COM_cmd_type == 3){

        // handling pid tuning here
        sscanf((char*)COM_DMA_rx_buff, ";B%f", &Ki);
        OLED_Printf(0, 0, 8, "%s", COM_DMA_rx_buff+1); // get rid of type indicator
        COM_cmd_type = 0;
      }
      else if(COM_cmd_type == 4){

        // handling pid tuning here
        sscanf((char*)COM_DMA_rx_buff, ";C%f", &Kd);
        OLED_Printf(0, 0, 8, "%s", COM_DMA_rx_buff+1); // get rid of type indicator
        COM_cmd_type = 0;
      }
      else if(COM_cmd_type == 5){

        // handling pid tuning here
        sscanf((char*)COM_DMA_rx_buff, ";D%f", &dummy);
        target_1000 = dummy * 1000;
        err_acc_1000 = 0;
        err_prev_1000 = 0;
        OLED_Printf(0, 0, 8, "%s", COM_DMA_rx_buff+1); // get rid of type indicator
        COM_cmd_type = 0;
      }
      else if(COM_cmd_type == 127){
        COM_cmd_type = 0;
      }

      // BT cmd handle
      if(BT_cmd_type == 1){
        
        OLED_Printf(0, 0, 8, "%s", BT_DMA_rx_buff+1); // get rid of type indicator
        BT_cmd_type = 0;
      }
      else if(BT_cmd_type == 2){

        // handling pid tuning here
        sscanf((char*)BT_DMA_rx_buff, ";A%f", &Kp);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (int)Kp);
        OLED_Printf(0, 0, 8, "%s", BT_DMA_rx_buff+1); // get rid of type indicator
        BT_cmd_type = 0;
      }
      else if(BT_cmd_type == 3){

        // handling pid tuning here
        sscanf((char*)BT_DMA_rx_buff, ";B%f", &Ki);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (int)Ki);
        OLED_Printf(0, 0, 8, "%s", BT_DMA_rx_buff+1); // get rid of type indicator
        BT_cmd_type = 0;
      }
      else if(BT_cmd_type == 127){
        BT_cmd_type = 0;
      }

      OLED_Update();
      main_update = 0;
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//   if(huart->Instance == USART1){

//     BT_SendString(CMD2NOTES[BT_cmd-65]);
//     HAL_UART_Receive_IT(&huart1, &BT_cmd, 1);
//   }
// }

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
