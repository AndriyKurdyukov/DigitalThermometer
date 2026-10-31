<img width="320" height="320" alt="photo" src="https://github.com/AndriyKurdyukov/DigitalThermometer/blob/main/pictures/digital_thermometer.jpg" />

# DigitalThermometer

## Description
A simple digital thermometer with hardware and firmware made from scratch. Kicad was used for PCB design and STM Cube IDE for firmware.

## Why?
I  wanted to refresh my electronics skills and decided to make a simple first KICAD project combining both firmware and hardware. I am obviously more familiar with the firmware side of things because of my job.

## Hardware
An STM32F030F4P6 microcontroller (4 KB RAM, 16 KB FLASH) was used to read out temperature values from an TMP117 temperature sensor over I2C.
A Newhaven NHD-C0220BIZ LCD display was used for displaying temperature values, using I2C communication as well.

## Firmware
A simple while loop reading out values from temperature sensor and sending them to the display, no RTOS used and everything in blocking mode. Since I2C HAL bloats memory so much, I rewrote it using Lower Layer. Maybe I should do the same for the GPIO.

Fimrware can be flashed on the microcontroller with any Nucleo board using ST-Link over SWD. 
