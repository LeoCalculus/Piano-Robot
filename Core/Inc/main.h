/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SD_SCK_Pin GPIO_PIN_2
#define SD_SCK_GPIO_Port GPIOE
#define SD_CD_Pin GPIO_PIN_3
#define SD_CD_GPIO_Port GPIOE
#define SD_MISO_Pin GPIO_PIN_5
#define SD_MISO_GPIO_Port GPIOE
#define SD_MOSI_Pin GPIO_PIN_6
#define SD_MOSI_GPIO_Port GPIOE
#define LCD_MOSI_Pin GPIO_PIN_1
#define LCD_MOSI_GPIO_Port GPIOC
#define SOL1_ISENSE_Pin GPIO_PIN_0
#define SOL1_ISENSE_GPIO_Port GPIOA
#define SOL2_ISENSE_Pin GPIO_PIN_2
#define SOL2_ISENSE_GPIO_Port GPIOA
#define MT_ISENSE_Pin GPIO_PIN_4
#define MT_ISENSE_GPIO_Port GPIOA
#define LCD_SCK_Pin GPIO_PIN_2
#define LCD_SCK_GPIO_Port GPIOB
#define MT1_L_Bridge_Pin GPIO_PIN_10
#define MT1_L_Bridge_GPIO_Port GPIOE
#define MT1_R_Bridge_Pin GPIO_PIN_11
#define MT1_R_Bridge_GPIO_Port GPIOE
#define MT2_L_Bridge_Pin GPIO_PIN_12
#define MT2_L_Bridge_GPIO_Port GPIOE
#define MT2_R_Bridge_Pin GPIO_PIN_13
#define MT2_R_Bridge_GPIO_Port GPIOE
#define BLE_TX_Pin GPIO_PIN_14
#define BLE_TX_GPIO_Port GPIOB
#define BLE_RX_Pin GPIO_PIN_15
#define BLE_RX_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_11
#define LCD_CS_GPIO_Port GPIOD
#define LCD_RST_Pin GPIO_PIN_12
#define LCD_RST_GPIO_Port GPIOD
#define LCD_RS_Pin GPIO_PIN_13
#define LCD_RS_GPIO_Port GPIOD
#define PD14_Pin GPIO_PIN_14
#define PD14_GPIO_Port GPIOD
#define PD15_Pin GPIO_PIN_15
#define PD15_GPIO_Port GPIOD
#define SWD_TX_Pin GPIO_PIN_6
#define SWD_TX_GPIO_Port GPIOC
#define SWD_RX_Pin GPIO_PIN_7
#define SWD_RX_GPIO_Port GPIOC
#define PC8_Pin GPIO_PIN_8
#define PC8_GPIO_Port GPIOC
#define PC9_Pin GPIO_PIN_9
#define PC9_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_8
#define LED3_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_9
#define LED2_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOA
#define PA11_Pin GPIO_PIN_11
#define PA11_GPIO_Port GPIOA
#define PA12_Pin GPIO_PIN_12
#define PA12_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define PA15_Pin GPIO_PIN_15
#define PA15_GPIO_Port GPIOA
#define PC10_Pin GPIO_PIN_10
#define PC10_GPIO_Port GPIOC
#define PC11_Pin GPIO_PIN_11
#define PC11_GPIO_Port GPIOC
#define PC12_Pin GPIO_PIN_12
#define PC12_GPIO_Port GPIOC
#define SOLENOID2_4_Pin GPIO_PIN_0
#define SOLENOID2_4_GPIO_Port GPIOD
#define SOLENOID2_3_Pin GPIO_PIN_1
#define SOLENOID2_3_GPIO_Port GPIOD
#define SOLENOID2_2_Pin GPIO_PIN_2
#define SOLENOID2_2_GPIO_Port GPIOD
#define SOLENOID2_1_Pin GPIO_PIN_3
#define SOLENOID2_1_GPIO_Port GPIOD
#define SOLENOID1_8_Pin GPIO_PIN_4
#define SOLENOID1_8_GPIO_Port GPIOD
#define SOLENOID1_7_Pin GPIO_PIN_5
#define SOLENOID1_7_GPIO_Port GPIOD
#define SOLENOID1_6_Pin GPIO_PIN_6
#define SOLENOID1_6_GPIO_Port GPIOD
#define SOLENOID1_5_Pin GPIO_PIN_7
#define SOLENOID1_5_GPIO_Port GPIOD
#define SOLENOID1_4_Pin GPIO_PIN_3
#define SOLENOID1_4_GPIO_Port GPIOB
#define MT2_ENB_Pin GPIO_PIN_4
#define MT2_ENB_GPIO_Port GPIOB
#define MT2_ENA_Pin GPIO_PIN_5
#define MT2_ENA_GPIO_Port GPIOB
#define MT1_ENB_Pin GPIO_PIN_6
#define MT1_ENB_GPIO_Port GPIOB
#define MT1_ENA_Pin GPIO_PIN_7
#define MT1_ENA_GPIO_Port GPIOB
#define SOLENOID1_3_Pin GPIO_PIN_8
#define SOLENOID1_3_GPIO_Port GPIOB
#define SOLENOID1_2_Pin GPIO_PIN_9
#define SOLENOID1_2_GPIO_Port GPIOB
#define SOLENOID1_1_Pin GPIO_PIN_0
#define SOLENOID1_1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
