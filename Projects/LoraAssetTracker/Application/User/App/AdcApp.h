/*
 * AdcApp.h
 *
 *  Created on: 15-May-2022
 *      Author: RANGA
 */

#ifndef APPLICATION_USER_CORE_ADCAPP_H_
#define APPLICATION_USER_CORE_ADCAPP_H_

#include <stdint.h>

void AdcApp_vDMAInit(void);
uint16_t Adc_u16GetBattVtg(void);


#endif /* APPLICATION_USER_CORE_ADCAPP_H_ */
