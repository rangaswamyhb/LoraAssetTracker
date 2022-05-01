/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "ipcc.h"
#include "rf.h"
#include "rng.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

int __io_putchar(int ch)
{
	if(ch == NULL)
		return 0;

	HAL_UART_Transmit(&huart1, &ch, 1, 100);
}

int __io_getchar(void)
{
	uint8_t ch = 0;
	HAL_UART_Receive(&huart1, &ch, 1, 100);
	return ch;
}

void PrintMsg(uint8_t * u8Msg)
{
	if(u8Msg == NULL)
		return;

	HAL_UART_Transmit(&huart1, u8Msg, (uint16_t) strlen((char *)u8Msg), 1000);
}



#if 1
uint8_t hal_spi_in_out( const uint8_t in_data )
{

#if 1

	uint8_t data = 0;

//    HAL_SPI_Transmit( &hspi1, (uint8_t *) &data, 1, 1000);
//    HAL_SPI_Receive( &hspi1, (uint8_t *) &data, 1, 1000);

    HAL_SPI_TransmitReceive(&hspi1, &in_data, &data, 1, 1000);
//    HAL_SPI_Transmit( &hspi1, (uint8_t *) &aa[0], 1, 1000);
//    HAL_SPI_Receive( &hspi1, (uint8_t *) &data, 1, 1000);

    return data;
#else



//        while( LL_SPI_IsActiveFlag_TXE( &huart1 ) == 0 )
//        {
//        };
//        LL_SPI_TransmitData8( &huart1, ( uint8_t )( out_data & 0xFF ) );
//
//        while( LL_SPI_IsActiveFlag_RXNE( &huart1 ) == 0 )
//        {
//        };
//        return LL_SPI_ReceiveData8( &huart1 );

#endif
}




void my_lr1110_hal_reset( void )
{

	HAL_GPIO_WritePin(LR_NRESET_GPIO_Port, LR_NRESET_Pin, 0);
	HAL_Delay(100);
	HAL_GPIO_WritePin(LR_NRESET_GPIO_Port, LR_NRESET_Pin, 1);
	HAL_Delay(500);



//    hal_gpio_set_value( lr1110_context->reset, 0 );
//    hal_mcu_wait_us( 5000 );
//    hal_gpio_set_value( lr1110_context->reset, 1 );
//    hal_mcu_wait_us( 5000 );
//
//    // Wait 200ms until internal lr1110 fw is ready
//    hal_mcu_wait_us( 200000 );
//    radio_mode = RADIO_AWAKE;
//
//    return LR1110_HAL_STATUS_OK;
}



void my_lr1110_hal_wait_on_busy( void )
{

    while( HAL_GPIO_ReadPin(LR_BUSY_GPIO_Port, LR_BUSY_Pin) == 1 )
    {
    };
}

void my_lr1110_hal_check_device_ready(void )
{

	        HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, 0);
	        HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, 1);
        my_lr1110_hal_wait_on_busy( );
//    }
//    else
//    {
//        // Busy is HIGH in sleep mode, wake-up the device with a small glitch on NSS
//        hal_gpio_set_value( lr1110_context->nss, 0 );
//        hal_gpio_set_value( lr1110_context->nss, 1 );
//        lr1110_hal_wait_on_busy( lr1110_context->busy );
//        radio_mode = RADIO_AWAKE;
//    }
}

void my_lr1110_hal_wakeup( void )
{
    my_lr1110_hal_check_device_ready(  );
}



void my_lr1110_hal_write( const uint8_t* command, const uint16_t command_length,
                                      const uint8_t* data, const uint16_t data_length )
{
	my_lr1110_hal_check_device_ready();

	HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, 0);

    for( uint16_t i = 0; i < command_length; i++ )
    {
        hal_spi_in_out( command[i] );
    }

    for( uint16_t i = 0; i < data_length; i++ )
	{
		hal_spi_in_out( data[i] );
	}

    HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, 1);

    my_lr1110_hal_wait_on_busy();

//	if( my_lr1110_hal_wakeup( context ) == LR1110_HAL_STATUS_OK )
//	{
//		hal_gpio_set_value( ( ( lr1110_t* ) context )->nss.pin, 0 );
//		for( uint16_t i = 0; i < command_length; i++ )
//		{
//			hal_spi_in_out( ( ( lr1110_t* ) context )->spi_id, command[i] );
//		}
//		for( uint16_t i = 0; i < data_length; i++ )
//		{
//			hal_spi_in_out( ( ( lr1110_t* ) context )->spi_id, data[i] );
//		}
//		hal_gpio_set_value( ( ( lr1110_t* ) context )->nss.pin, 1 );
//
//		return lr1110_hal_wait_on_busy( context, 5000 );
//	}


#if 0
#if defined( USE_LR1110_CRC_OVER_SPI )
    // Compute the CRC over command array first and over data array then
    uint8_t cmd_crc = lr1110_hal_compute_crc( 0xFF, command, command_length );
    cmd_crc         = lr1110_hal_compute_crc( cmd_crc, data, data_length );
#endif



    my_lr1110_hal_check_device_ready( );

    // Put NSS low to start spi transaction
//    hal_gpio_set_value( lr1110_context->nss, 0 );
    HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, 0);

    for( uint16_t i = 0; i < command_length; i++ )
    {
        hal_spi_in_out( command[i] );
    }
    for( uint16_t i = 0; i < data_length; i++ )
    {
        hal_spi_in_out( data[i] );
    }

#if defined( USE_LR1110_CRC_OVER_SPI )
    // Send the CRC byte at the end of the transaction
    hal_spi_in_out( lr1110_context->spi_id, cmd_crc );
#endif

    // Put NSS high as the spi transaction is finished
//    hal_gpio_set_value( lr1110_context->nss, 1 );
    HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, 1);

    // LR1110_SYSTEM_SET_SLEEP_OC=0x011B opcode. In sleep mode the radio busy line is held at 1 => do not test it
    if( ( command[0] == 0x01 ) && ( command[1] == 0x1B ) )
    {
//        radio_mode = RADIO_SLEEP;

        // add a incompressible delay to prevent trying to wake the radio before it is full asleep
        HAL_Delay(500); //( 500 );
    }
#endif
}

void my_lr1110_hal_read( const uint8_t* command, const uint16_t command_length,
                                     uint8_t* data, const uint16_t data_length )
{
    my_lr1110_hal_check_device_ready(  );

    // Put NSS low to start spi transaction
    //    hal_gpio_set_value( lr1110_context->nss, 0 );
    HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, 0);

    HAL_Delay(1);

    for( uint16_t i = 0; i < command_length; i++ )
    {
        hal_spi_in_out( command[i] );
    }

//    hal_gpio_set_value( lr1110_context->nss, 1 );
    HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, 1);

    if( data_length > 0 )
    {
    	my_lr1110_hal_wait_on_busy(  );
    	HAL_Delay(10);
//        my_lr1110_hal_check_device_ready(  );
//        hal_gpio_set_value( lr1110_context->nss, 0 );
        HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, 0);

        HAL_Delay(1);

        hal_spi_in_out( 0x00 );

        for( uint16_t i = 0; i < data_length; i++ )
        {
            data[i] = hal_spi_in_out( 0 );
        }

        // Put NSS high as the spi transaction is finished
//        hal_gpio_set_value( lr1110_context->nss, 1 );
        HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, 1);
    }

    my_lr1110_hal_wait_on_busy();

}


void GetLr1110Version(char * version)
{
//	uint8_t cBuffer[2] = {0, };
//	uint8_t rbuffer[10] = { 0 };
//
//	cBuffer[0] = 0x01;
//	cBuffer[1] = 0x01;
//
//	my_my_lr1110_hal_read(cBuffer, 2, rbuffer, 4);
//
//	memcpy(version, rbuffer, 0x04);


    uint8_t         cbuffer[2];
    uint8_t         rbuffer[4] = { 0x00 };


    cbuffer[0] = ( uint8_t )( 257 >> 8 );
    cbuffer[1] = ( uint8_t )( 257 >> 0 );

    my_my_lr1110_hal_read( cbuffer, 2, rbuffer, 4 );

    memcpy(version, rbuffer, 0x04);

}



void my1_lr1110_hal_reset( void )
{
	PrintMsg("LR1110 --->\r\n");

    HAL_GPIO_WritePin(LR_NRESET_GPIO_Port, LR_NRESET_Pin, 0);
    HAL_Delay( 500 );
    HAL_GPIO_WritePin(LR_NRESET_GPIO_Port, LR_NRESET_Pin, 1);

//    HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, 0);
    HAL_GPIO_WritePin(NSS_GPIO_Port, NSS_Pin, 1);

//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
//    HAL_Delay( 100 );
////    hal_gpio_set_value( ( ( lr1110_t* ) context )->reset.pin, 1 );
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);

//	HAL_UART_Transmit(&huart1,"LR1110 Busy Pin Check\n",strlen("LR1110 Busy Pin Check\n"),100);


	while( HAL_GPIO_ReadPin(LR_BUSY_GPIO_Port, LR_BUSY_Pin) == 1 )
	{
		PrintMsg("LR1110 Busy Pin High\r\n");
	}
	PrintMsg(" OK\r\n");

//    return LR1110_HAL_STATUS_OK;
    while( HAL_GPIO_ReadPin(LR_BUSY_GPIO_Port, LR_BUSY_Pin) == 1 );
    PrintMsg("LR1110 RESET OK\r\n");
//		return LR1110_HAL_STATUS_OK;

//		while(hal_gpio_get_value(lr1110.event.pin) == 0)
//		{
//			hal_uart_tx(HAL_PRINTF_UART_ID,"LR1110 Event Pin Low\n",strlen("LR1110 Event Pin Low\n"));
//		}

//		hal_uart_tx(HAL_PRINTF_UART_ID,"LR1110 Event Pin High\n",strlen("LR1110 Event Pin High\n"));
//	HAL_DBG_TRACE_PRINTF("\r\n Initiate Wakeup sequence: ");
    HAL_Delay(1000);
//    if( my_lr1110_hal_wakeup(&lr1110) == LR1110_HAL_STATUS_OK )
//    {
//        HAL_DBG_TRACE_PRINTF("Pass");
//    }
//    else
//    {
//        HAL_DBG_TRACE_PRINTF("Fail");
//    }

//    HAL_DBG_TRACE_PRINTF("\r\n");
//    while(1);
//    while(1);



}

#endif



void spdt_2g4_on( void )
{
	HAL_GPIO_WritePin(VCC_24GHZ_SW_GPIO_Port, VCC_24GHZ_SW_Pin, 1);
}

void spdt_2g4_off( void )
{
	HAL_GPIO_WritePin(VCC_24GHZ_SW_GPIO_Port, VCC_24GHZ_SW_Pin, 0);
}

void set_wifi_antenna( void )
{
    /* SWITCH_WIFI_BLE_Pin = 1 ==> Wi-Fi */
//    hal_gpio_set_value( SWITCH_WIFI_BLE, 1 );
	HAL_GPIO_WritePin(BLE_WIFI_SEL_SWITCH_GPIO_Port, BLE_WIFI_SEL_SWITCH_Pin, 1);

}

void set_ble_antenna( void )
{
    /* SWITCH_WIFI_BLE_Pin = 0 ==> BLE */
//    hal_gpio_set_value( SWITCH_WIFI_BLE, 0 );
    HAL_GPIO_WritePin(BLE_WIFI_SEL_SWITCH_GPIO_Port, BLE_WIFI_SEL_SWITCH_Pin, 0);
}


void my_lr1110_get_version( char * hw, char * type, uint16_t * fw )
{
    uint8_t         cbuffer[2];
    uint8_t         rbuffer[4] = { 0x00 };

    cbuffer[0] = ( uint8_t )( 1 );
    cbuffer[1] = ( uint8_t )( 1 );

    my_lr1110_hal_read( cbuffer, 2, rbuffer, 4 );


	* hw   = rbuffer[0];
	* type = rbuffer[1];
	* fw    = ( ( uint16_t ) rbuffer[2] << 8 ) + ( uint16_t ) rbuffer[3];

}


void my_lr1110_reset(void)
{
	printf("LR1110 -- Reset\r\n");
	HAL_GPIO_WritePin(LR_NRESET_GPIO_Port, LR_NRESET_Pin, 0);
	HAL_Delay( 1 );
	HAL_GPIO_WritePin(LR_NRESET_GPIO_Port, LR_NRESET_Pin, 1);
}


extern int lora_ping_pong( void );
extern int lora_wan( void );

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
  /* Config code for STM32_WPAN (HSE Tuning must be done before system clock configuration) */
  MX_APPE_Config();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* IPCC initialisation */
   MX_IPCC_Init();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RF_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
  MX_RNG_Init();
  MX_DMA_Init();
  /* USER CODE BEGIN 2 */

  printf("\r\n\r\n\r\n");
  printf("===================================================\r\n");
  printf("||       Lora Asset Tracker v%d.%d.%d                ||\r\n",
		  FW_VERSION_MAJOR, FW_VERSION_MINOR, FW_VERSION_PATCH);
  printf("===================================================\r\n\r\n");

  my_lr1110_reset();
  HAL_Delay(500);
  my_lr1110_reset();

  uint8_t hw = 0, type = 0;
  uint16_t fw = 0;
  my_lr1110_get_version( &hw, &type, &fw );
  printf( "LR1110 : hw:%#02X / type:%#02X / fw:%#04X\n\r", hw, type, fw );


  /* Uncomment for Lora Ping Ping */
//  lora_ping_pong();

  /* Uncomment for Lorawan */
  //lora_wan();

  /* USER CODE END 2 */

  /* Init code for STM32_WPAN */
  MX_APPE_Init();
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_APPE_Process();

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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCCEx_EnableLSCO(RCC_LSCOSOURCE_LSE);
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
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS|RCC_PERIPHCLK_RFWAKEUP;
  PeriphClkInitStruct.RFWakeUpClockSelection = RCC_RFWKPCLKSOURCE_LSE;
  PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSI;
  PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE1;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN Smps */

  /* USER CODE END Smps */
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

	printf( "Wrong parameters value: file %s on line %lu\n", ( const char* )file, line );
	/* Infinite loop */
	while( 1 )
	{
	}

  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

