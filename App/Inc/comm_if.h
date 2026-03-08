#ifndef COMM_IF_H
#define COMM_IF_H

#include <stdint.h>

typedef struct
{
    uint8_t i2cOk;
    uint8_t spiOk;
    uint8_t lastI2cDeviceId;
    uint8_t lastSpiStatus;
    uint32_t tickMs;
} CommStatus_t;

void CommIF_Init(void);
void CommIF_Service(CommStatus_t *status);

#endif
