/*
 * LoraPingPong.h
 *
 *  Created on: 08-Jun-2022
 *      Author: Ranga
 */

#ifndef APPLICATION_LORAMODULE_LORAPINGPONG_H_
#define APPLICATION_LORAMODULE_LORAPINGPONG_H_

#include <stdint.h>

uint8_t LoraPingPong_u8GetRssiValue(void);
uint8_t LoraPingPong_u8GetSnrValue(void);

void LoraPingPong_vInit(void);
void LoraPingPong_vRunStateMachine(void);


#endif /* APPLICATION_LORAMODULE_LORAPINGPONG_H_ */
