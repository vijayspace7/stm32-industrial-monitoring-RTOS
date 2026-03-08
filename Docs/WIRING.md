# Wiring Guide

## Analog sensing path

### 1) Temperature sensor (analog)
Use **LM35** or **TMP36** so that the temperature channel stays on ADC and **I2C/SPI remain free** for communications.

- LM35 VCC -> 5V or 3.3V (follow your module requirements)
- LM35 GND -> GND
- LM35 VOUT -> **PA0 / ADC1_IN5**

> If you use bare LM35 with 5V supply, make sure the output never exceeds 3.3V at the MCU ADC pin. A practical industrial lab build usually powers it from 3.3V or uses conditioning.

### 2) Gas / smoke sensor (analog)
- MQ-2 / MQ-135 analog out -> **PA1 / ADC1_IN6**
- Sensor VCC and GND as required by module

### 3) Vibration digital input
- SW-420 DO -> **PB0**

### 4) Level / pressure switch input
- Digital switch -> **PB1**

## Outputs
- Green LED -> **PA5**
- Red LED -> **PA6**
- Buzzer driver -> **PA7**
- Relay driver -> **PB6**

## Communication channels added

### I2C1 expansion bus
- **PB8** -> I2C1_SCL
- **PB9** -> I2C1_SDA
- Example device: AT24C64 / 24LC64 EEPROM at address **0x50**
- Add 4.7k pull-ups to 3.3V if the module does not already include them

### SPI1 expansion bus
- **PA5** -> SPI1_SCK
- **PA6** -> SPI1_MISO
- **PA7** -> SPI1_MOSI
- **PA4** -> SPI1_CS

## Important pin conflict note
The example keeps PA5/PA6/PA7 for LEDs/buzzer from the original template, but SPI1 also maps there on the NUCLEO-L476RG.

For a real build, choose one of these:
1. Move LED/buzzer GPIOs to spare pins and keep SPI1 on PA5/PA6/PA7/PA4.
2. Keep the simple LED/buzzer mapping and disable SPI1 in hardware.

The source package includes SPI1 init and a communication task so you can adapt it quickly in CubeMX.
