/*
 * Newhaven_LCD.h
 *  Library for Newhaven NHD-C0220BIZ LCD display
 *  Created on: Oct 4, 2026
 *      Author: andriy-pc
 */

#ifndef INC_NEWHAVEN_LCD_H_
#define INC_NEWHAVEN_LCD_H_

#include "stdint.h"
#include "Gpio.h"

typedef struct Newhaven_LCD* NewhavenPtr;

NewhavenPtr createLCD(uint8_t i2cAddress, GpioPtr rstPin);

void reset(NewhavenPtr display);  // reset pin

void initLCD(NewhavenPtr display);

void outputToLCD(NewhavenPtr display, char* text, uint8_t textLen);

void clearLCD(NewhavenPtr display);

void homeCursor(NewhavenPtr display);

void setCursor(NewhavenPtr display);

#endif /* INC_NEWHAVEN_LCD_H_ */
