# Industrial Sensory Monitoring and Alert System

A complete STM32 + FreeRTOS project for an **industrial sensory monitoring and alert system**.

This updated package targets **STM32L476RG (NUCLEO-L476RG)** and uses an **analog temperature sensor** so the design can keep **I2C and SPI communication channels available** for expansion.

## What changed in this revision
- Temperature sensing is explicitly based on **LM35 / TMP36 analog output**.
- Added **I2C1** initialization and communication health check task.
- Added **SPI1** initialization and communication health check task.
- Added documentation for the bus expansion concept.

## Inputs
- **Temperature sensor** on ADC1 channel (analog LM35 / TMP36)
- **Gas / smoke sensor** on ADC1 channel (analog)
- **Vibration switch / fault input** on GPIO external input
- **Tank level / pressure switch** on GPIO external input

## Outputs
- **Green LED**: healthy system
- **Red LED**: alarm active
- **Buzzer**: audible alert
- **Relay output**: trip / shutdown output
- **UART log**: live telemetry and alarms over serial terminal

## Communication channels
- **I2C1** expansion task for EEPROM / OLED / digital sensor / gateway link
- **SPI1** expansion task for FRAM / external ADC / radio / gateway link

## RTOS features used
- Tasks
- Queues
- Event groups
- Mutex for UART logging
- Software timers

## Project structure

```text
industrial_monitor_project/
├── App/
│   ├── Inc/
│   │   ├── app_config.h
│   │   ├── app_types.h
│   │   ├── comm_if.h
│   │   ├── monitor.h
│   │   ├── sensor_if.h
│   │   └── ui_log.h
│   └── Src/
│       ├── app_freertos.c
│       ├── comm_if.c
│       ├── monitor.c
│       ├── sensor_if.c
│       └── ui_log.c
├── Core/
│   ├── Inc/
│   │   └── main.h
│   └── Src/
│       ├── main.c
│       ├── stm32l4xx_hal_msp.c
│       └── stm32l4xx_it.c
├── Docs/
│   ├── ARCHITECTURE.md
│   ├── BILL_OF_MATERIALS.md
│   ├── CUBEMX_SETUP.md
│   └── WIRING.md
└── Scripts/
    └── sample_serial_output.txt
```

## Recommended hardware
- STM32 NUCLEO-L476RG
- LM35 or TMP36 analog temperature sensor
- MQ-2 / MQ-135 analog gas sensor module
- SW-420 vibration module
- Float switch / pressure switch
- Active buzzer + transistor driver
- Relay module + transistor driver
- Optional I2C EEPROM / OLED
- Optional SPI FRAM / external peripheral

## Important board note
This package includes both the original GPIO example and SPI1 support. On the NUCLEO-L476RG, **SPI1 overlaps with PA5/PA6/PA7**, which were also used earlier for LED/buzzer.

So treat this zip as a **complete project template plus integration-ready source set**. In CubeMX, remap the LED/buzzer GPIOs to spare pins so SPI1 can be used cleanly.

## Firmware flow
1. `SensorTask` samples the analog and digital field sensors.
2. `MonitorTask` evaluates thresholds and alarm state.
3. `ActuatorTask` drives LED, buzzer, and relay.
4. `LoggerTask` prints sensor and alarm messages to UART.
5. `CommTask` verifies **I2C1** and **SPI1** channels periodically and reports link health.

## Example serial output
```text
[BOOT] Industrial monitoring system started
[BOOT] Temperature sensor: LM35/TMP36 analog on ADC1_IN5
[BOOT] I2C1 and SPI1 channels enabled for expansion
[SENS] T=28.54C GAS=12.48% VIB=0 LVL=0
[STAT] NORMAL
[COMM] I2C=OK DEV=0x5A SPI=MISS RX=0x00
```


