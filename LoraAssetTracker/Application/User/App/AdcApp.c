/*
 * AdcApp.c
 *
 *  Created on: 15-May-2022
 *      Author: RANGA
 */



/****************************************************
 *
 *  ADC Application
 *
 */

#include "main.h"

/* Value of analog reference voltage (Vref+), connected to analog voltage   */
/* supply Vdda (unit: mV).                                                  */
#define VDDA_APPLI                       (3300UL)

/* Definitions of data related to this example */
/* Definition of ADCx conversions data table size */
/* Size of array set to ADC sequencer number of ranks converted,            */
/* to have a rank in each array address.                                    */
#define ADC_CONVERTED_DATA_BUFFER_SIZE   (   3UL)


extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;



/* ADC Code START */
/* Variables for ADC conversion data */
__IO uint16_t uhADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]; /* ADC group regular conversion data (array of data) */

/* Variable to report status of DMA transfer of ADC group regular conversions */
/*  0: DMA transfer is not completed                                          */
/*  1: DMA transfer is completed                                              */
/*  2: DMA transfer has not yet been started yet (initial state)              */
__IO uint8_t ubDmaTransferStatus = 2U; /* Variable set into DMA interruption callback */



/**
  * @brief  DMA transfer complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  /* Update status variable of DMA transfer */
  ubDmaTransferStatus = 1;
}

/**
  * @brief  ADC error interruption callback
  * @retval None
  */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* Note: Disable ADC interruption that caused this error before entering in
           infinite loop below. */

  /* In case of error due to overrun: Disable ADC group regular overrun interruption */
  LL_ADC_DisableIT_OVR(ADC1);

  /* Error reporting */
  Error_Handler();
}

void AdcApp_vDMAInit(void)
{
	/* Perform ADC calibration */
	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK)
	{
		/* Calibration Error */
		Error_Handler();
	}

	/* Start ADC group regular conversion */
	/* Note: First start with DMA transfer initialization, following ones
		   with basic ADC start. */
	if (HAL_ADC_Start_DMA(&hadc1,
						(uint32_t *)uhADCxConvertedData,
						ADC_CONVERTED_DATA_BUFFER_SIZE
					   ) != HAL_OK)
	{
		/* Error: ADC conversion start could not be performed */
		Error_Handler();
	}

}

uint16_t Adc_u16GetBattVtg(void)
{
	uint16_t u16VrefAnalog_mVolt = 0;
	uint16_t u16Retrys = 5;
	printf("ADC Start Conversion for Get Battery Vtg\r\n");

	do{
		/* Start ADC group regular conversion */
		if (HAL_ADC_Start(&hadc1) != HAL_OK)
		{
		  /* Error: ADC conversion start could not be performed */
		  Error_Handler();
		}

		HAL_Delay(1);

		if(ubDmaTransferStatus == 1)
		{
			ubDmaTransferStatus = 0;

			u16VrefAnalog_mVolt = __LL_ADC_CALC_VREFANALOG_VOLTAGE(uhADCxConvertedData[0], LL_ADC_RESOLUTION_12B);
		}
	}while((u16VrefAnalog_mVolt == 0) && (--u16Retrys));


    return u16VrefAnalog_mVolt;
}



