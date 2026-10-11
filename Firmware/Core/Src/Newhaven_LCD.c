/*
 * Newhaven_LCD.c
 *
 *  Created on: Oct 4, 2026
 *      Author: andriy-pc
 */


#include "Newhaven_LCD.h"
#include "I2C.h"

#include <string.h>

struct Newhaven_LCD // necessary info
{
	uint8_t mI2cAddress;
	GpioPtr rstPin;
};

struct Newhaven_LCD displayData; // stack allocated "member" variable storing relevant data
                                 // only one display at a time is possible
NewhavenPtr createLCD(uint8_t i2cAddress, GpioPtr rstPin)
{
	displayData.mI2cAddress = i2cAddress;
	displayData.rstPin = rstPin;
    return &displayData;
}

extern HAL_StatusTypeDef transmitI2C_arr(uint8_t slaveAddr, uint8_t* array, uint8_t arrLen);
extern void setState(GpioPtr gpio, GPIO_PinState PinState);

void initLCD(NewhavenPtr display)
{
	uint8_t pData[10] = {0x0, 0x38,0x39,0x14,0x78,0x5E,0x6D,0x0C, 0x01,0x06};
	HAL_StatusTypeDef status = transmitI2C_arr(displayData.mI2cAddress, pData, 10);
}

void outputToLCD(NewhavenPtr display, char* text, uint8_t textLen)
{
	uint8_t textNew[textLen + 1] ;
	memcpy ( &textNew[1], &text[0], textLen);
	textNew[0] = 0x40;
	transmitI2C_arr(displayData.mI2cAddress, textNew, textLen + 1);
}

void clearLCD(NewhavenPtr display)
{
	uint8_t arr[3] = {0x0,0x01};
	transmitI2C_arr(displayData.mI2cAddress, arr , 2);
}

void reset(NewhavenPtr display)
{
	setState(displayData.rstPin,  GPIO_PIN_SET);  //  RST pin, active low
}
