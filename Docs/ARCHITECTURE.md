# Architecture

## RTOS Objects

### Tasks
- **SensorTask**: acquires sensors periodically
- **MonitorTask**: evaluates thresholds and determines plant status
- **ActuatorTask**: drives LEDs, buzzer and relay based on event bits
- **LoggerTask**: prints data to UART safely

### Queue
- `sensorQueue`: carries latest `SensorFrame_t` from SensorTask to MonitorTask and LoggerTask
- `logQueue`: carries formatted log messages to LoggerTask

### Event group bits
- `EVT_WARN_TEMP`
- `EVT_ALARM_TEMP`
- `EVT_WARN_GAS`
- `EVT_ALARM_GAS`
- `EVT_ALARM_VIB`
- `EVT_ALARM_LEVEL`
- `EVT_SYSTEM_ALARM`

### Mutex
- `uartMutex`: protects UART prints

### Timer
- `buzzerTimer`: toggles buzzer in alarm state

## State model

### NORMAL
All variables below warning thresholds.

### WARNING
Any warning threshold crossed, but no trip condition.

### ALARM
Any alarm threshold crossed or discrete fault asserted.

## Data flow

```text
ADC/GPIO -> SensorTask -> sensorQueue -> MonitorTask -> event bits -> ActuatorTask
                                            |-> logQueue -> LoggerTask -> UART
```

## Design choices

- Queue-based separation keeps sensing independent from decision logic.
- Event groups keep output logic simple.
- UART is isolated behind a logger to avoid scattered `printf` calls.
- Thresholds centralized in one config header.
