/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32wbxx_hal.h"

#include "app_conf.h"
#include "app_entry.h"
#include "app_common.h"

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

#define FW_VERSION_MAJOR		0
#define FW_VERSION_MINOR		0
#define FW_VERSION_PATCH		1

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GNSS_ANT_SWITCH_Pin GPIO_PIN_8
#define GNSS_ANT_SWITCH_GPIO_Port GPIOB
#define PB1_INPUT_SWITCH_Pin GPIO_PIN_9
#define PB1_INPUT_SWITCH_GPIO_Port GPIOB
#define LR_NRESET_Pin GPIO_PIN_0
#define LR_NRESET_GPIO_Port GPIOA
#define VCC_SENSORS_MCU_Pin GPIO_PIN_1
#define VCC_SENSORS_MCU_GPIO_Port GPIOA
#define VCC_24GHZ_SW_Pin GPIO_PIN_3
#define VCC_24GHZ_SW_GPIO_Port GPIOA
#define NSS_Pin GPIO_PIN_4
#define NSS_GPIO_Port GPIOA
#define SCK_Pin GPIO_PIN_5
#define SCK_GPIO_Port GPIOA
#define MISO_Pin GPIO_PIN_6
#define MISO_GPIO_Port GPIOA
#define MOSI_Pin GPIO_PIN_7
#define MOSI_GPIO_Port GPIOA
#define LNA_PON_MCU_Pin GPIO_PIN_8
#define LNA_PON_MCU_GPIO_Port GPIOA
#define EFECT_HALL_OUT_Pin GPIO_PIN_2
#define EFECT_HALL_OUT_GPIO_Port GPIOB
#define LR_BUSY_Pin GPIO_PIN_0
#define LR_BUSY_GPIO_Port GPIOB
#define ACC_INT1_Pin GPIO_PIN_1
#define ACC_INT1_GPIO_Port GPIOB
#define TX1_LED_Pin GPIO_PIN_4
#define TX1_LED_GPIO_Port GPIOE
#define BLE_WIFI_SEL_SWITCH_Pin GPIO_PIN_15
#define BLE_WIFI_SEL_SWITCH_GPIO_Port GPIOA
#define LR_IRQ_Pin GPIO_PIN_4
#define LR_IRQ_GPIO_Port GPIOB
#define LR_IRQ_EXTI_IRQn EXTI4_IRQn
#define RX1_LED_Pin GPIO_PIN_5
#define RX1_LED_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
