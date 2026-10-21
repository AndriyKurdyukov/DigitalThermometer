/*
 * I2C_LL.c
 *
 *  Created on: Oct 12, 2026
 *      Author: andriy-pc
 */

#include "I2C_LL.h"
#include "stm32f0xx_ll_i2c.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"

void initI2C_LL()
{
	LL_I2C_InitTypeDef I2C_InitStruct = {0};
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	/**I2C1 GPIO Configuration
	  PA9   ------> I2C1_SCL
	  PA10   ------> I2C1_SDA
	 */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Peripheral clock enable */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
	/** I2C Initialization
	 */
	LL_I2C_DisableOwnAddress2(I2C1);  // disabled so we are in master mode
	LL_I2C_DisableGeneralCall(I2C1); // disabled so we are in master mode
	LL_I2C_EnableClockStretching(I2C1);
	I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
	I2C_InitStruct.Timing = 0x00201D2B;

	/* By default, an analog noise filter is present on the SDA
       and SCL inputs. This filter is compliant with the I2C specification, which requires the
       suppression of spikes with pulse width up to 50 ns in Fast-mode and Fast-mode Plus. The
       user can disable this analog filter by setting the ANFOFF bit, and/or select a digital filter by
       configuring the DNF[3:0] bit in the I2C_CR1 register.
    */

	I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
	I2C_InitStruct.DigitalFilter = 0;
	I2C_InitStruct.OwnAddress1 = 0;
	I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
	I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
	LL_I2C_Init(I2C1, &I2C_InitStruct);
	LL_I2C_EnableAutoEndMode(I2C1); //  Automatic end mode : a STOP condition is automatically sent when NBYTES data are transferred.
	LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);
}

/**
 * transmit given amount of data in blocking mode
 */
void transmitI2C_arr_LL(uint8_t slaveAddr , uint8_t* array, uint8_t arrLen)
{
	/*
	 *  RM0360, p. 547
	 * To initiate the communication, program the following parameters for the addressed slave in
        the I2C_CR2 register:
        •Addressing mode (7-bit or 10-bit): ADD10
        •Slave address to be sent: SADD[9:0]
        •Transfer direction: RD_WRN
        •In case of 10-bit address read: HEAD10R bit. HEAD10R must be configure to indicate
          if the complete address sequence must be sent, or only the header in case of a
          direction change.s
        •The number of bytes to be transferred: NBYTES[7:0]. If this number is equal to or
          greater than 255 bytes, NBYTES[7:0] must initially be filled with 0xFF.
        • The user must then set the START bit in I2C_CR2 register
	  */
	uint8_t byteCnt = 0;
	LL_I2C_EnableAutoEndMode(I2C1); // enable automatic stop setting after nbytes have been transmitted
	LL_I2C_SetSlaveAddr(I2C1, slaveAddr);
	LL_I2C_SetTransferRequest(I2C1, LL_I2C_REQUEST_WRITE);
	LL_I2C_SetTransferSize(I2C1, arrLen);
	LL_I2C_GenerateStartCondition(I2C1);
	// RM0360, p. 584 for  I2C interrupt and status register (I2C_ISR), TXE , TXIS, TC(transfer complete)

	// while less than NBYTES transmitted, check if I2C_ISR.TXIS == 1, if yes , write byte to I2C_TXDR
	// see RM0360 p.550
	while(byteCnt < arrLen)
	{
	   if(LL_I2C_IsActiveFlag_TXIS(I2C1) == 1)
	   {
	       LL_I2C_TransmitData8(I2C1,  array[byteCnt] );
		   byteCnt++;
	   }
	}
	while(LL_I2C_IsActiveFlag_STOP(I2C1) != 1 )  //TODO add error handling
	{
		// busy wait until stop flag (STOPF) is set by hardware
	}
	LL_I2C_ClearFlag_STOP(I2C1); // clear stopf bit
}


/*
 * receive given amount of bytes in blocking mode
 */
void receiveI2C_arr_LL(uint8_t slaveAddr, uint8_t* array, uint8_t arrLen)
{
	uint8_t byteCnt = 0;
	LL_I2C_EnableAutoEndMode(I2C1); // enable automatic stop setting after nbytes have been received
	LL_I2C_SetSlaveAddr(I2C1, slaveAddr);
	LL_I2C_SetTransferRequest(I2C1, LL_I2C_REQUEST_READ );
	LL_I2C_SetTransferSize(I2C1, arrLen);
	LL_I2C_GenerateStartCondition(I2C1);

	while(byteCnt < arrLen)
	{
		if(LL_I2C_IsActiveFlag_RXNE(I2C1) == 1)
		{
			array[byteCnt] = LL_I2C_ReceiveData8(I2C1); // read byte
			byteCnt++;
		}
	}

	while(LL_I2C_IsActiveFlag_STOP(I2C1) != 1 )
	{
		// busy wait until stop flag (STOPF) is set by hardware
	}
	LL_I2C_ClearFlag_STOP(I2C1); // clear stopf bit
}

/*
 *  write a 16 bit register
 */
void writeI2CReg_LL(uint8_t slaveAddr, uint8_t regAddr, uint16_t regSize, uint16_t regVal)
{
    uint8_t p[3]={regAddr, (regVal >> 8) & 0xFF, regVal & 0xFF};
    transmitI2C_arr_LL(slaveAddr, p, 3);
}

/*
 *  read a 16 bit register
 */
uint16_t readI2CReg_LL(uint8_t slvAddr, uint8_t reg)
{
	uint8_t pData[2];
	transmitI2C_arr_LL(slvAddr, &reg, 1); // send the reg memory location first
	receiveI2C_arr_LL(slvAddr, pData, 2); // read 2 bytes
	return ((uint16_t)(pData[0]) << 8) | pData[1]; // msb is sent first
}
