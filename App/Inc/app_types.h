#ifndef APP_TYPES_H
#define APP_TYPES_H

#include <stdint.h>

typedef enum
{
    SYS_NORMAL = 0,
    SYS_WARNING,
    SYS_ALARM
} SystemState_t;

typedef struct
{
    uint32_t rawTemp;
    uint32_t rawGas;
    float temperatureC;
    float gasPercent;
    uint8_t vibrationActive;
    uint8_t levelActive;
    uint32_t tickMs;
} SensorFrame_t;

typedef struct
{
    uint8_t i2cOk;
    uint8_t spiOk;
    uint8_t lastI2cDeviceId;
    uint8_t lastSpiStatus;
    uint32_t tickMs;
} CommFrame_t;

typedef struct
{
    SystemState_t state;
    uint32_t eventBits;
} MonitorResult_t;

#endif
