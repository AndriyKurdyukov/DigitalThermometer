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
	//status = HAL_I2C_Master_Transmit(&hi2c1, slaveAddr , array, arrLen, 10);
	//return status;
	/*
	 *  RM0360, p. 547
	 * To initiate the communication, program the following parameters for the addressed slave in
        the I2C_CR2 register:
        •Addressing mode (7-bit or 10-bit): ADD10
        •Slave address to be sent: SADD[9:0]
        •Transfer direction: RD_WRN
        •In case of 10-bit address read: HEAD10R bit. HEAD10R must be configure to indicate
          if the complete address sequence must be sent, or only the header in case of a
          direction change.
        •The number of bytes to be transferred: NBYTES[7:0]. If this number is equal to or
          greater than 255 bytes, NBYTES[7:0] must initially be filled with 0xFF.
        • The user must then set the START bit in I2C_CR2 register
	  */
	uint8_t byteCnt = 0;
	LL_I2C_SetSlaveAddr(I2C1, slaveAddr);
	LL_I2C_SetTransferRequest(I2C1, LL_I2C_REQUEST_WRITE);
	LL_I2C_SetTransferSize(I2C1, arrLen);
	LL_I2C_GenerateStartCondition(I2C1);
	// RM0360, p. 584 for  I2C interrupt and status register (I2C_ISR), TXE , TXIS, TC(transfer complete)

	//TODO while less than NBYTES transmitted, check if I2C_ISR.TXIS == 1, if yes , write byte to I2C_TXDR
	// see RM0360 p.550
	while(byteCnt < arrLen)
	{
	   if(LL_I2C_IsActiveFlag_TXIS(I2C1))
	   {
	       LL_I2C_TransmitData8(I2C1,  array[byteCnt] );
		   byteCnt++;
	   }
	}
	//TODO if autoend is set, stop is automatically generated
	// uint32_t LL_I2C_IsActiveFlag_STOP(const I2C_TypeDef *I2Cx)
}
