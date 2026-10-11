/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "I2C.h"
#include "Newhaven_LCD.h"
#include "TMP117.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */


/**
 *  convert decimal number to char
 *  temperature should be in [ -99.999, 99.999  ]
 */
void convertIntToChar(int16_t temperature, char* numbers, uint8_t numbersLen)
{
	//TODO check that numberslen >= 5
	temperature = temperature/100; // discard last two numbers -> [ -99.9, 99.9 ]
	uint8_t charIndex = numbersLen - 2;
	int16_t rest = 0;
    int16_t quotient = temperature;

    rest = quotient % 10;
    numbers[charIndex] = (uint8_t)(rest) + 0x30; // convert number to char
    numbers[--charIndex] = '.'; // dot
    quotient = quotient / 10;
    charIndex--;

	while(quotient != 0)
	{
	    rest = quotient % 10;
		numbers[charIndex] = (uint8_t)(rest) + 0x30; // convert number to char
		quotient = quotient / 10;
		charIndex--;
	}

	if(temperature >= 0)
	{
		numbers[charIndex] = '+';
	} else {
		numbers[charIndex] = '-';
	}
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
  initI2C();
  uint8_t displaySlaveAddr = 0x78;
  GpioPtr displayRstPin = createPin(GPIOA, GPIO_PIN_7);
  NewhavenPtr display = createLCD(displaySlaveAddr, displayRstPin);
  reset(display);
  HAL_Delay(20);
  initLCD(display);
  HAL_Delay(50);
  clearLCD(display);
  GpioPtr blinkypin = createPin(GPIOB, GPIO_PIN_1);
  HAL_Delay(50);
  TempSensorPtr tempSensor = createTempSensor(0x90);
  initSensor(tempSensor);
  int16_t temperature_mC = 0x0;
  char numbers_arr[8];
  //TODO add asserts handling
  while (1)
  {
     toggle(blinkypin);
     temperature_mC = getTemperature(tempSensor);
     clearLCD(display);
     HAL_Delay(20);
     convertIntToChar(temperature_mC, numbers_arr, 8);
     numbers_arr[0] = 'T';
     numbers_arr[1] = ':';
     numbers_arr[7] = 'C';
     outputToLCD(display, numbers_arr, 8);
	 HAL_Delay(2000);
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
