/*
 * Gpio.c
 *
 *  Created on: Oct 5, 2026
 *      Author: andriy-pc
 */


#include "Gpio.h"

struct GpioStr
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
};

struct GpioStr gpioArray[3]; // statically allocated array of pins, max 3
uint8_t currGpioArrIndex = 0; // tracking current gpio pin

/**
 *  create pin struct pointer and initialize pin
 *  at most  3 pins can be initialized
 */
GpioPtr createPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(GPIOx == GPIOA)
	{
       __HAL_RCC_GPIOA_CLK_ENABLE();
	}

	if(GPIOx == GPIOB)
	{
	   __HAL_RCC_GPIOB_CLK_ENABLE();
	}

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

	gpioArray[currGpioArrIndex].GPIO_Pin = GPIO_Pin;
	gpioArray[currGpioArrIndex].GPIOx = GPIOx;
	currGpioArrIndex++;
	return &gpioArray[currGpioArrIndex - 1];
}
void setState(GpioPtr gpio, GPIO_PinState PinState)
{
	HAL_GPIO_WritePin(gpio->GPIOx, gpio->GPIO_Pin, PinState);
}
void toggle(GpioPtr gpio)
{
	HAL_GPIO_TogglePin(gpio->GPIOx, gpio->GPIO_Pin);
}
