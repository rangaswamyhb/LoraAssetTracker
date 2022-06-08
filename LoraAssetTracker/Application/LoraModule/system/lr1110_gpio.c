/*!
 * \file      gpio.c
 *
 * \brief     GPIO driver implementation
 *
 * \remark: Relies on the specific board GPIO implementation as well as on
 *          IO expander driver implementation if one is available on the target
 *          board.
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
#include "gpio-board.h"
#include "stm32wb15xx.h"
#include "stm32wbxx.h"

void GpioInit( Gpio_t *obj, PinNames pin, PinModes mode,  PinConfigs config, PinTypes type, uint32_t value )
{
    GpioMcuInit( obj, pin, mode, config, type, value );
}

void GpioSetContext( Gpio_t *obj, void* context )
{
    GpioMcuSetContext( obj, context );
}

void GpioSetInterrupt( Gpio_t *obj, IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler )
{
    GpioMcuSetInterrupt( obj, irqMode, irqPriority, irqHandler );
}

void GpioRemoveInterrupt( Gpio_t *obj )
{
    GpioMcuRemoveInterrupt( obj );
}

void GpioWrite( Gpio_t *obj, uint32_t value )
{
	GPIO_TypeDef* gpio_port = ( GPIO_TypeDef* ) ( AHB2PERIPH_BASE + ( ( obj->pin & 0xF0 ) << 6 ) );

	HAL_GPIO_WritePin( gpio_port, ( 1 << ( obj->pin & 0x0F ) ), ( value != 0 ) ? GPIO_PIN_SET : GPIO_PIN_RESET );
}

void GpioToggle( Gpio_t *obj )
{
    GPIO_TypeDef* gpio_port = ( GPIO_TypeDef* ) ( AHB2PERIPH_BASE + ( ( obj->pin & 0xF0 ) << 6 ) );

    HAL_GPIO_TogglePin( gpio_port, ( 1 << ( obj->pin & 0x0F ) ) );
}

uint32_t GpioRead( Gpio_t *obj )
{
    GPIO_TypeDef* gpio_port = ( GPIO_TypeDef* ) ( AHB2PERIPH_BASE + ( ( obj->pin & 0xF0 ) << 6 ) );

    return ( HAL_GPIO_ReadPin( gpio_port, ( ( 1 << ( obj->pin & 0x0F ) ) ) ) != GPIO_PIN_RESET ) ? 1 : 0;
}
