/*
 * TMP117.c
 *
 *  Created on: Oct 5, 2026
 *      Author: andriy-pc
 */

#include "TMP117.h"

#define ONE_SHOT_MODE 0x3 << 10
#define CONFIG_REG    0x1
#define TEMP_REG      0x0
#define DEVICE_ID_REG        0xF
#define AVG_8         0x1 << 5
#define LCB_IN_mC     7.8125

struct TempSensorStruct
{
	uint8_t I2Caddress;
};

struct TempSensorStruct currentSensor;  // at most one sensor can be created

TempSensorPtr createTempSensor(uint8_t I2Caddress)
{
	currentSensor.I2Caddress = I2Caddress;
	return &currentSensor;
}

extern void writeI2CReg(uint8_t slvAddr, uint8_t regAddr, uint16_t regSize, uint16_t regVal);
extern uint16_t readI2CReg_steps(uint8_t slvAddr, uint16_t reg);

//extern HAL_StatusTypeDef transmitI2C_arr(uint8_t slaveAddr , uint8_t* array, uint8_t arrLen);
//extern HAL_StatusTypeDef receiveI2C_arr(uint8_t slaveAddr , uint8_t* array, uint8_t arrLen);

void initSensor(TempSensorPtr tempSensor)
{
	uint16_t deviceID = readI2CReg_steps(currentSensor.I2Caddress, DEVICE_ID_REG);
	uint16_t configReg = ONE_SHOT_MODE | AVG_8;
	writeI2CReg(currentSensor.I2Caddress, CONFIG_REG, 2, configReg);
}

/**
 * read raw temperature value from the register
 */
uint16_t readRawTemp(TempSensorPtr tempSensor)
{
	writeI2CReg(currentSensor.I2Caddress, CONFIG_REG, 2, ONE_SHOT_MODE | AVG_8);
	HAL_Delay(125); // tmp117, p.12,  124 ms for 8 averages
	uint16_t  temperature = readI2CReg_steps(currentSensor.I2Caddress, TEMP_REG );
	return temperature;
}

/**
 * get temperature in miliCelsius, rounded
 */
int16_t getTemperature(TempSensorPtr tempSensor)  //TODO use fixed point math, 0.0078125 = 1/128 , shift by 7
{
	/*int16_t rawTemp = (int16_t)readRawTemp(tempSensor);  //TODO use signed value, it is twos complement value, handle negative values as well
	float temp_mC = (float)(rawTemp) * LCB_IN_mC;
	return (int16_t)(temp_mC); */
	int16_t rawSensorVal = readRawTemp(tempSensor);
	int16_t integer_part =  (int16_t)(rawSensorVal >> 7);
	int16_t decimal_part =  (int16_t)(rawSensorVal & 0x7F); // decimal part, should be weighed with 1/128
	int16_t decimal_as_int =  (int16_t) ( (float)(decimal_part)  * LCB_IN_mC );
	int16_t temperature_in_mC = 1000 * integer_part + decimal_as_int;
	return temperature_in_mC;
}
