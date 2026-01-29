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
#include "gpdma.h"
#include "icache.h"
#include "memorymap.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <application.h>
#include <string.h>
#include <stdio.h>
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

COM_InitTypeDef BspCOMInit;

/* USER CODE BEGIN PV */
uint8_t* msg = NULL;
volatile int rx_complete = 0;  // volatile since modified in ISR
volatile uint16_t valid_rx = 0;
volatile uint16_t old_pos = 0; // size record the index in DMA_target_location, need old pos track the new data
char empty_row[] = "                        ";
float pos_number[] = {50.0f, 20.0f, 10.0f, -69.0f, 62.0f, 90.0f, 0.0f, -30.0f, 20.0f, 55.0f};
int pos_index = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

// this function tells where dma data should be on arrival
void DMA_receive_idle_init(UART_HandleTypeDef* DMA_uart_handle, uint8_t* DMA_target_location);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// init LCD configuration with SPI2 and GPIOB pins 13, 14, 15
LCD_Config lcd_config = {
    .hspi = &hspi2,
    .cs_port = GPIOB,
    .cs_pin = GPIO_PIN_15,
    .rs_port = GPIOB,
    .rs_pin = GPIO_PIN_13,
    .rst_port = GPIOB,
    .rst_pin = GPIO_PIN_14,
};
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
  MX_GPDMA1_Init();
  MX_ICACHE_Init();
  MX_SPI2_Init();
  MX_UART4_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM8_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  /* Initialize LCD directly (without LVGL for this demo) */
  LCD_init(&lcd_config);
  HAL_TIM_Base_Start_IT(&htim4);
  // BT_config(&huart3);
  // TEST: Simple blocking transmit before anything else
  uint8_t test_msg[] = "USART1 OK\r\n";
  HAL_UART_Transmit(&huart1, test_msg, sizeof(test_msg)-1, 1000);

  DMA_receive_idle_init(&huart1, DMA_target_location);
  encoder_start(&htim8);
  HAL_TIM_Base_Start_IT(&htim2); // interrupt for controller
  controller_init();
  // reset encoder value
  encoder_old_position_cm = 0.0f;
  encoder_read_result = 0.0f;

  // begin pwm: 10KHz
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // using pwm at TIM3_CHANNEL1
  // set initial duty cycle:
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0); // here using (99+1)/2 =50 duty cycle
  // also for CHANNEL2:
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  // set duty cycle as well:
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0); 

  
  msg = (uint8_t*)"Still alive";
  
  /* USER CODE END 2 */

  /* Initialize leds */
  BSP_LED_Init(LED_GREEN);

  /* Initialize USER push-button, will be used to trigger an interrupt each time it's pressed.*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
  BspCOMInit.BaudRate   = 115200;
  BspCOMInit.WordLength = COM_WORDLENGTH_8B;
  BspCOMInit.StopBits   = COM_STOPBITS_1;
  BspCOMInit.Parity     = COM_PARITY_NONE;
  BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;
  if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //char telemetry_buf[64];
  while (1)
  {
    /* Blink LED to show MCU is running */
    BSP_LED_Toggle(LED_GREEN);

    // Copy volatile values to local (atomic-ish read)
    //float vel = current_velocity_cm_s;
    //float dist = current_distance_cm;

    // Send telemetry via BT
    // sprintf(telemetry_buf, "D:%.2f V:%.2f\r\n", dist, vel);
    // BT_send_info(&huart1, (uint8_t*)telemetry_buf, strlen(telemetry_buf));
    // BT_send_info(&huart1, (uint8_t*)&vofa, 44);
    // BT_send_info(&huart1, msg, strlen((char*)msg));

    // only update LCD when new BT data received
    if (rx_complete) {
      executeCommand(rx_buffer); // execute command only in main loop not in interrupt
      LCD_draw_string(&lcd_config, 0, 10, empty_row, COLOR_BLACK, COLOR_WHITE);  // Clear old text
      LCD_draw_string(&lcd_config, 0, 10, (char*)rx_buffer, COLOR_BLACK, COLOR_WHITE);  // Draw new
      rx_complete = 0;
    }

    // test for index traversal - ok
    // while (pos_index < sizeof(pos_number)/sizeof(pos_number[0])){
    //     target_position_cm = pos_number[pos_index];
    //     is_moving = 0; // wait for controller update it to 0
    //     while (!is_moving); //move ok
    //     wait_ms(500); // simulate next note delay
    //     pos_index++;
    // }

    if (is_blocked){
      LCD_draw_string(&lcd_config, 0, 5, "Sensor detected!  ", COLOR_BLACK, COLOR_WHITE);
    } else {
      LCD_draw_string(&lcd_config, 0, 5, "Nothing in sensor!", COLOR_BLACK, COLOR_WHITE);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_CSI;
  RCC_OscInitStruct.CSIState = RCC_CSI_ON;
  RCC_OscInitStruct.CSICalibrationValue = RCC_CSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_CSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
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

/* USER CODE BEGIN 4 */

void DMA_receive_idle_init(UART_HandleTypeDef* DMA_uart_handle, uint8_t* DMA_target_location){
  HAL_UARTEx_ReceiveToIdle_DMA(DMA_uart_handle, DMA_target_location, 128); // changable @application.h
}

// DMA call back function for usart_1_rx
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t size){
  uint16_t new_bytes; // new received bytes count
  if (huart == &huart1){
    if (size > old_pos){
      new_bytes = size - old_pos;
      memcpy(rx_buffer, &DMA_target_location[old_pos], new_bytes);
    } else { // there is warp around
      new_bytes = sizeof(DMA_target_location) - old_pos + size;
      memcpy(rx_buffer, &DMA_target_location[old_pos], sizeof(DMA_target_location)-old_pos);
      memcpy(&rx_buffer[128-old_pos], DMA_target_location, size); // remaining
    }
    valid_rx = new_bytes;
    rx_buffer[new_bytes] = '\0'; // when printing string, other old will be neglected.
    rx_complete = 1;
    old_pos = size;
  }
}

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

#ifdef  USE_FULL_ASSERT
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
