/*
 * I2C.c
 *
 *  Created on: Oct 5, 2026
 *      Author: andriy-pc
 */

#include "I2C.h"
#include "stm32f0xx_hal_i2c.h"

I2C_HandleTypeDef hi2c1;

void initI2C()
{
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x00201D2B;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		assert_param(false);
	}
	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		assert_param(false);
	}
	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	{
		assert_param(false);
	}
}

HAL_StatusTypeDef transmitI2C_arr(uint8_t slaveAddr , uint8_t* array, uint8_t arrLen)
{
	HAL_StatusTypeDef status = HAL_ERROR;
	status = HAL_I2C_Master_Transmit(&hi2c1, slaveAddr , array, arrLen, 10);
	return status;
}

HAL_StatusTypeDef receiveI2C_arr(uint8_t slaveAddr , uint8_t* array, uint8_t arrLen)
{
	HAL_StatusTypeDef status = HAL_ERROR;
	status = HAL_I2C_Master_Receive(&hi2c1,slaveAddr, array, arrLen, 10);
	return status;
}

/**
 *  blocking read two bytes
 */
uint16_t readI2CReg(uint8_t slvAddr, uint16_t reg)
{
	HAL_StatusTypeDef status;
	uint8_t pData[2];
	status = HAL_I2C_Mem_Read(&hi2c1, slvAddr, reg, 2, pData, 2, 10);
	uint16_t result =  (((uint16_t)(pData[0])) << 8)  | pData[1];
	return result;
}

uint16_t readI2CReg_steps(uint8_t slvAddr, uint8_t reg)
{
	HAL_StatusTypeDef returnValue;
	uint8_t pData[2];

	returnValue = HAL_I2C_Master_Transmit(&hi2c1, slvAddr, &reg, 1, 5); // send the reg memory location first
	if(returnValue != HAL_OK)
	{
		assert_param(false);
	}

	returnValue = HAL_I2C_Master_Receive(&hi2c1, slvAddr, pData, 2, 10); // read 2 bytes
	if(returnValue != HAL_OK)
	{
		assert_param(false);
	}
	return ((uint16_t)(pData[0]) << 8) | pData[1]; // msb is sent first
}


void writeI2CReg(uint8_t slvAddr, uint8_t regAddr, uint16_t regSize, uint16_t regVal)
{
    uint8_t p[3]={regAddr, (regVal >> 8) & 0xFF  , regVal & 0xFF};
	HAL_StatusTypeDef status;
	status= HAL_I2C_Master_Transmit(&hi2c1, slvAddr, p, 3, 10); // send the reg memory location first
	if(status != HAL_OK)
	{
		assert_param(false);
	}
}
