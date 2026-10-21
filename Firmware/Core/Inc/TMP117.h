/*
 * TMP117.h
 *
 *  Created on: Oct 5, 2026
 *      Author: andriy-pc
 */

#ifndef INC_TMP117_H_
#define INC_TMP117_H_

#include "stdint.h"
#include "I2C_LL.h"

typedef struct TempSensorStruct* TempSensorPtr;

TempSensorPtr createTempSensor(uint8_t I2Caddress);
void initSensor(TempSensorPtr tempSensor);
int16_t getTemperature(TempSensorPtr tempSensor);

#endif /* INC_TMP117_H_ */
