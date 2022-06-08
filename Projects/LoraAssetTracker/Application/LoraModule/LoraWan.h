/*
 * LoraWan.h
 *
 *  Created on: 08-Jun-2022
 *      Author: Ranga
 */

#ifndef APPLICATION_LORAMODULE_LORAWAN_H_
#define APPLICATION_LORAMODULE_LORAWAN_H_

#include <stdint.h>

uint32_t LoraWan_u32Init(void);
void LoraWan_vRunStateMachine(void);

#endif /* APPLICATION_LORAMODULE_LORAWAN_H_ */
