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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <application.h>
#include <file_transfer.h>
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

/* USER CODE BEGIN PV */
int pwmNum = 200;
uint8_t hexSDRead[20] = {0};
FIL file;
uint8_t FileBuffer[512];
UINT BytesRead;
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
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_TIM4_Init();
  MX_SPI2_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
#ifdef USINGOLED
  OLED_Init();
  OLED_WriteString("ELEC-391");
#endif
  LcdInit(); // using LCD
  LcdFillScreen(COLOR_WHITE); 
  LcdDrawString(0, 0, "Hello ELEC 391!", COLOR_BLUE, COLOR_WHITE);

#ifdef USINGMPU6050
  if (checkExist()){ // cannot detect -> light LED
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, RESET);
    OLED_SetCursor(0, 3);
    OLED_WriteString("Cannot find MPU 6050!");
  } else {
    OLED_SetCursor(0, 3);
    OLED_WriteString("MPU 6050 Detected!");
  }
  mpu6050Init();
  int16_t accBuffer[3];
  readAcc(accBuffer);
#endif 

  init();  // Initialize VOFA struct
  //char displayAccBuffer[32];
#ifdef HC05SETUP 
  if (setupBT()){
    OLED_SetCursor(0, 5);
    OLED_WriteString("HC-05 is configured!");
  } else {
    OLED_SetCursor(0, 5);
    OLED_WriteString("Unable to config HC-05!");
  }
#endif

  HC05_ReceiveInfo(rx_data);
  FT_Init();  // Initialize file transfer module
  // start PWM: this PWM runs at 2000Hz, Calculation: 72MHz/(71+1)/(499+1) = 2000Hz
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // TIM3_CH1 as displayed
  // set duty cycle
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 250); // 50% duty cycle, 250 by (499+1)/2=250

    // Initialize accData before starting interrupt
  HAL_TIM_Base_Start_IT(&htim4);  // Start TIM4 interrupt for controllerUpdate
  // OLED_SetCursor(0, 6);
  // OLED_WriteString("PWM ready!");
  // Initialize SD card with FatFs
  FRESULT sdResult = FR_NOT_READY;
  OLED_SetCursor(0, 2);

  // Try init multiple times (5 times)
  for (int i = 0; i < 5; i++) {
    sdResult = SD_Init();
    if (sdResult == FR_OK) {
      OLED_WriteString("SD Card OK!");
      break;
    }
    HAL_Delay(100);
  }

  if (sdResult != FR_OK) {
    snprintf((char*)hexSDRead, sizeof(hexSDRead), "SD err %d", sdResult);
    OLED_WriteString((char*)hexSDRead);
  } else {
    // List MIDI files and display
    OLED_SetCursor(0, 3);
    int numSongs = SD_ListMidiFiles();
    snprintf((char*)hexSDRead, sizeof(hexSDRead), "Found %d songs", numSongs);
    OLED_WriteString((char*)hexSDRead);
    SD_DisplaySongList(0, 0);
  }
  
  OLED_SetCursor(0, 1);
  OLED_WriteString("System Ready");

  if (SD_OpenFile(&file, "test1.mid", FA_READ) == FR_OK){
    SD_ReadFile(&file, FileBuffer, sizeof(FileBuffer), &BytesRead);
    SD_CloseFile(&file);
  }
  HC05_SendBytes(FileBuffer, BytesRead);
  OLED_SetCursor(0, 1);
  OLED_WriteString("Send bytes!   ");

  // OLED_Clear(); // clear the entire screen above features known to be ok

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // Process file transfer binary packets
    if (rx_binary_complete) {
      // Copy packet data BEFORE resetting - prevents race condition with interrupt
      uint8_t packet_copy[80];
      uint16_t packet_len = rx_binary_index;
      memcpy(packet_copy, rx_binary_buffer, packet_len);

      // Reset BEFORE processing so interrupt can receive next packet
      rx_binary_complete = 0;
      rx_binary_index = 0;

      // Now safely process the copy
      FT_ProcessPacket(packet_copy, packet_len);
    }

    // Check for file transfer timeout
    FT_TimeoutCheck();

    // only process when complete message received
  #ifdef USINGHC05
    if(rx_complete){
      // OLED_SetCursor(0, 5);
      // OLED_WriteString((char*)rx_data);  // cast to char*
      executeCommand(rx_data);
      rx_complete = 0;  // clear flag
      // echo back with \r\n for terminal
      HC05_SendInfo((uint8_t*)"Got it!\r\n");
    }
    // HC05_SendInfo((uint8_t*)"Still Alive!\r\n");  // Disabled during file transfer
  #endif



    // dynamic pwm from 40%->100%, 40% -> 200, 100% -> 500
    if (pwmNum < 500) {
      pwmNum++;
    } else {
      pwmNum = 200;
    }
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwmNum);
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
/* Note: USART1 now uses DMA with IDLE line detection (HAL_UARTEx_RxEventCallback)
   This old callback is kept for compatibility but USART1 won't trigger it anymore */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  (void)huart;  /* Unused - USART1 uses DMA now */
}

// DMA TX complete callback - required for continuous DMA transmission
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  if(huart->Instance == USART2){
    // DMA transfer complete - state automatically reset to READY by HAL
    // This callback confirms DMA is working
  }
}

// UART Error callback - critical for handling overrun errors on STM32F1
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
  if(huart->Instance == USART1){
    // Clear all error flags (ORE, NE, FE, PE)
    __HAL_UART_CLEAR_OREFLAG(huart);
    __HAL_UART_CLEAR_NEFLAG(huart);
    __HAL_UART_CLEAR_FEFLAG(huart);
    __HAL_UART_CLEAR_PEFLAG(huart);

    // Restart DMA reception
    HC05_RestartDMA();
  }
}

// DMA RX callback - handles both USART1 (HC05) and USART2 (VOFA)
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart->Instance == USART1) {
    /* HC05 Bluetooth - DMA reception with IDLE line detection */
    /* IMPORTANT: Process data BEFORE restarting DMA (single buffer) */

    if (Size > 0) {
      /* Check if it's a binary protocol packet */
      if (rx_dma_buffer[0] >= 0xF0 && rx_dma_buffer[0] <= 0xF3) {
        /* Binary packet - always copy (main loop copies before processing so safe) */
        uint16_t copy_len = (Size < sizeof(rx_binary_buffer)) ? Size : sizeof(rx_binary_buffer);
        memcpy(rx_binary_buffer, rx_dma_buffer, copy_len);
        rx_binary_index = copy_len;
        rx_binary_complete = 1;
      } else if (!rx_complete) {
        /* Text command - only copy if previous not pending (text can be lost) */
        uint16_t copy_len = (Size < sizeof(rx_data) - 1) ? Size : sizeof(rx_data) - 1;
        memcpy(rx_data, rx_dma_buffer, copy_len);
        rx_data[copy_len] = '\0';
        rx_complete = 1;
      }
    }

    /* Restart DMA AFTER processing - abort ensures clean state */
    HC05_RestartDMA();
  }
  else if (huart->Instance == USART2) {
    // parse vofa message and save pid data
    // first only handle if message >= 5 bytes
    if (Size >= 5) {
      uint8_t command = VOFA_SEND_BUFFER[0];
      float value; // the pid will be stored as float

      switch (command)
      {
      case CMD_SET_LEFT_P:
        memcpy(&value, &VOFA_SEND_BUFFER[1], sizeof(float)); // first copy to value 
        leftHandMotor.P=value;
        break;

      case CMD_SET_LEFT_I:
        memcpy(&value, &VOFA_SEND_BUFFER[1], sizeof(float));
        leftHandMotor.I = value;
        break;

      case CMD_SET_LEFT_D:
        memcpy(&value, &VOFA_SEND_BUFFER[1], sizeof(float));
        leftHandMotor.D = value;
        break;

      case CMD_SET_RIGHT_P:
        memcpy(&value, &VOFA_SEND_BUFFER[1], sizeof(float)); // first copy to value 
        rightHandMotor.P=value;
        break;

      case CMD_SET_RIGHT_I:
        memcpy(&value, &VOFA_SEND_BUFFER[1], sizeof(float));
        rightHandMotor.I = value;
        break;

      case CMD_SET_RIGHT_D:
        memcpy(&value, &VOFA_SEND_BUFFER[1], sizeof(float));
        rightHandMotor.D = value;
        break;
      
      default:
        break;
      }
    }

    // restart receive
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, VOFA_SEND_BUFFER, sizeof(VOFA_SEND_BUFFER));
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
