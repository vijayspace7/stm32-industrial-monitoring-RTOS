# STM32CubeMX / STM32CubeIDE Setup

Target board: **NUCLEO-L476RG**

## Enable peripherals
- ADC1 with scan conversion, 2 regular channels
  - Channel 5 -> PA0 (temperature analog)
  - Channel 6 -> PA1 (gas analog)
- USART2 @ 115200 for serial logs
- GPIO outputs for LED / buzzer / relay
- GPIO inputs for vibration and level switch
- FreeRTOS CMSIS V2
- **I2C1** for industrial expansion bus
- **SPI1** for industrial expansion bus

## RTOS tasks
- SensorTask
- MonitorTask
- ActuatorTask
- LoggerTask
- **CommTask**

## Conflict to resolve in CubeMX
On the NUCLEO-L476RG, SPI1 default pins overlap with the earlier LED/buzzer example on PA5/PA6/PA7.

Recommended clean mapping:
- Keep ADC on PA0 / PA1
- Keep USART2 on PA2 / PA3
- Put SPI1 on PA5 / PA6 / PA7 with CS on PA4
- Move status LEDs and buzzer to free GPIO pins such as PB3 / PB4 / PB5
- Keep relay on PB6
- Keep I2C1 on PB8 / PB9

## Optional communication devices
- I2C EEPROM for event log storage
- I2C OLED for local diagnostics
- SPI FRAM / external ADC / RF module
- RS-485 transceiver driven from UART for Modbus RTU
