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
#define Solenoid_5_Pin GPIO_PIN_2
#define Solenoid_5_GPIO_Port GPIOE
#define Solenoid_6_Pin GPIO_PIN_3
#define Solenoid_6_GPIO_Port GPIOE
#define Solenoid_7_Pin GPIO_PIN_4
#define Solenoid_7_GPIO_Port GPIOE
#define Solenoid_8_Pin GPIO_PIN_5
#define Solenoid_8_GPIO_Port GPIOE
#define Solenoid_9_Pin GPIO_PIN_6
#define Solenoid_9_GPIO_Port GPIOE
#define Solenoid_10_Pin GPIO_PIN_13
#define Solenoid_10_GPIO_Port GPIOC
#define Solenoid_11_Pin GPIO_PIN_14
#define Solenoid_11_GPIO_Port GPIOC
#define Solenoid_12_Pin GPIO_PIN_15
#define Solenoid_12_GPIO_Port GPIOC
#define SD_CD_Pin GPIO_PIN_4
#define SD_CD_GPIO_Port GPIOA
#define SD_CS_Pin GPIO_PIN_4
#define SD_CS_GPIO_Port GPIOC
#define LCD_CS_Pin GPIO_PIN_5
#define LCD_CS_GPIO_Port GPIOC
#define LCD_RS_Pin GPIO_PIN_0
#define LCD_RS_GPIO_Port GPIOB
#define LCD_RST_Pin GPIO_PIN_7
#define LCD_RST_GPIO_Port GPIOE
#define LED_Pin GPIO_PIN_11
#define LED_GPIO_Port GPIOD
#define GPIO_EXTI5_LEFT_Pin GPIO_PIN_5
#define GPIO_EXTI5_LEFT_GPIO_Port GPIOB
#define GPIO_EXTI5_LEFT_EXTI_IRQn EXTI5_IRQn
#define GPIO_EXTI6_RIGHT_Pin GPIO_PIN_6
#define GPIO_EXTI6_RIGHT_GPIO_Port GPIOB
#define GPIO_EXTI6_RIGHT_EXTI_IRQn EXTI6_IRQn
#define Solenoid_1_Pin GPIO_PIN_7
#define Solenoid_1_GPIO_Port GPIOB
#define Solenoid_2_Pin GPIO_PIN_8
#define Solenoid_2_GPIO_Port GPIOB
#define Solenoid_3_Pin GPIO_PIN_9
#define Solenoid_3_GPIO_Port GPIOB
#define Solenoid_4_Pin GPIO_PIN_0
#define Solenoid_4_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
