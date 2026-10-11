/*
 * Gpio.h
 *
 *  Created on: Oct 5, 2026
 *      Author: andriy-pc
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "stm32f0xx_hal.h"

typedef struct GpioStr* GpioPtr;

GpioPtr createPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void setState(GpioPtr gpio, GPIO_PinState PinState);
void toggle(GpioPtr gpio);

#endif /* INC_GPIO_H_ */
