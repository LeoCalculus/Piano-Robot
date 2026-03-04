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
  /* USER CODE BEGIN 2 */
  // init HC-04 UART receive to idle mode with DMA
  hc04_receive_to_idle_init(&huart1, rx_message_buffer, sizeof(rx_message_buffer));
  // init LCD
  LCD_init();

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

  menu_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) 
  {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);
    // send message:
    // hc04_send_string((uint8_t*)"Hello from STM32H5!\n");
    // HAL_Delay(500);
    
    // check incoming message:
    if (rx_complete) {
      // snprintf(buffer_message, sizeof(buffer_message), "Received from HC-04: %s", rx_message);
      // hc04_send_string((uint8_t*)buffer_message); // send back the received message to HC-04 for
      // LCD_draw_string(0, 16, buffer_message, COLOR_BLACK, COLOR_WHITE); // display received message on LCD

      // execute_command(rx_message);
      if (menu_get_state() == MENU_STATE_TRANSMIT && rx_valid > 0 && rx_message[0] >= 0xF0 && rx_message[0] <= 0xF3)
      {
        FT_ProcessPacket(rx_message, rx_valid);
      }
      else if (menu_get_state() == MENU_STATE_TRANSMIT_RAM && rx_valid > 0)
      {
        /* Sentinel-based RAM song accumulation */
        if (!ram_rx_started)
        {
          /* Look for start sentinel (514.114f) in received data */
          const float start_sentinel = RAM_SENTINEL_START;
          int found = 0;
          for (int i = 0; i <= (int)rx_valid - 4; i++)
          {
            if (memcmp(&rx_message[i], &start_sentinel, 4) == 0)
            {
              ram_rx_started = 1;
              ram_rx_offset = 0;
              /* Copy any data after sentinel into song_ram */
              int remaining = rx_valid - (i + 4);
              if (remaining > 0)
              {
                uint32_t copy_len = ((uint32_t)remaining < RAM_SONG_MAX_BYTES) ? (uint32_t)remaining : RAM_SONG_MAX_BYTES;
                memcpy((uint8_t *)song_ram, &rx_message[i + 4], copy_len);
                ram_rx_offset = copy_len;
              }
              found = 1;
              break;
            }
          }
          if (!found)
          {
            /* Not song data — treat as text command (allows :a back) */
            execute_command(rx_message);
          }
        }
        else
        {
          /* Accumulate song data into song_ram */
          uint32_t space = RAM_SONG_MAX_BYTES - ram_rx_offset;
          uint32_t copy_len = (rx_valid < space) ? rx_valid : space;
          memcpy(&((uint8_t *)song_ram)[ram_rx_offset], rx_message, copy_len);
          ram_rx_offset += copy_len;

          /* Check last 4 bytes for end sentinel (114.514f) */
          const float end_sentinel = RAM_SENTINEL_END;
          if (ram_rx_offset >= 4)
          {
            float last_float;
            memcpy(&last_float, &((uint8_t *)song_ram)[ram_rx_offset - 4], 4);
            if (last_float == end_sentinel)
            {
              ram_rx_offset -= 4; /* Trim sentinel */
              ram_rx_complete = 1;
            }
          }

          /* Also complete if buffer is full */
          if (ram_rx_offset >= RAM_SONG_MAX_BYTES)
          {
            ram_rx_complete = 1;
          }
        }
      }
      else
      {
        execute_command(rx_message); // text command
      }
    
    }
    rx_complete = 0; // reset the flag

    // FT mode: process binary FT packets (wait for complete packet before processing)
    if (menu_get_state() == MENU_STATE_TRANSMIT && rx_message_index > 0)
    {
      uint16_t idx = rx_message_index;
      uint16_t need = 0;
      if (rx_message[0] >= 0xF0 && rx_message[0] <= 0xF3)
      {
        switch (rx_message[0]) {
          case FT_CMD_FILE_START: need = FT_FILE_START_SIZE; break;
          case FT_CMD_FILE_DATA:  need = (idx >= 4) ? (uint16_t)(5 + rx_message[3]) : 0xFFFF; break;
          case FT_CMD_FILE_END:   need = FT_FILE_END_SIZE; break;
          case FT_CMD_FILE_ABORT: need = FT_FILE_ABORT_SIZE; break;
        }
        if (need > 0 && idx >= need)
        {
          rx_message_index = 0;
          FT_ProcessPacket(rx_message, idx);
        }
      }
      else
      {
        // non-FT data in binary mode — likely text command (e.g. :a to go back)
        rx_message[idx] = '\0';
        if (idx > 0 && rx_message[idx - 1] == '\n') rx_message[idx - 1] = '\0';
        rx_message_index = 0;
        execute_command(rx_message);
      }
    }

    // RAM mode: process binary data that has no \n (accumulated in rx_message via rx_message_index)
    if (redirect_to_ram && rx_message_index > 0)
    {
      uint16_t bin_len = rx_message_index;
      rx_message_index = 0; // reset so callback can accumulate new data

      if (!ram_rx_started)
      {
        /* Look for start sentinel (514.114f) */
        const float start_sentinel = RAM_SENTINEL_START;
        int found = 0;
        for (int i = 0; i <= (int)bin_len - 4; i++)
        {
          if (memcmp(&rx_message[i], &start_sentinel, 4) == 0)
          {
            ram_rx_started = 1;
            ram_rx_offset = 0;
            int remaining = bin_len - (i + 4);
            if (remaining > 0)
            {
              uint32_t copy_len = ((uint32_t)remaining < RAM_SONG_MAX_BYTES) ? (uint32_t)remaining : RAM_SONG_MAX_BYTES;
              memcpy((uint8_t *)song_ram, &rx_message[i + 4], copy_len);
              ram_rx_offset = copy_len;
            }
            found = 1;
            break;
          }
        }
        if (!found)
        {
          /* Not song data — treat as text command (e.g. :a to go back) */
          rx_message[bin_len] = '\0';
          if (bin_len > 0 && rx_message[bin_len - 1] == '\n') rx_message[bin_len - 1] = '\0';
          execute_command(rx_message);
        }
      }
      else
      {
        /* Accumulate song data into song_ram */
        uint32_t space = RAM_SONG_MAX_BYTES - ram_rx_offset;
        uint32_t copy_len = (bin_len < space) ? bin_len : space;
        memcpy(&((uint8_t *)song_ram)[ram_rx_offset], rx_message, copy_len);
        ram_rx_offset += copy_len;

        /* Check for end sentinel (114.514f) */
        const float end_sentinel = RAM_SENTINEL_END;
        if (ram_rx_offset >= 4)
        {
          float last_float;
          memcpy(&last_float, &((uint8_t *)song_ram)[ram_rx_offset - 4], 4);
          if (last_float == end_sentinel)
          {
            ram_rx_offset -= 4;
            ram_rx_complete = 1;
          }
        }

        if (ram_rx_offset >= RAM_SONG_MAX_BYTES)
        {
          ram_rx_complete = 1;
        }
      }
    }

    // update menu display
    menu_update();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_CSI;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV2;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
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
