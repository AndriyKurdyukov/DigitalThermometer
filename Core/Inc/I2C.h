/*
 * I2C.h
 *
 *  Created on: Oct 4, 2026
 *      Author: andriy-pc
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "stdint.h"
#include "stm32f0xx_hal.h"

void initI2C();
HAL_StatusTypeDef transmitI2C_arr(uint8_t slaveAddr , uint8_t* array, uint8_t arrLen);
void writeReg(uint8_t slaveAddr);

#endif /* INC_I2C_H_ */
