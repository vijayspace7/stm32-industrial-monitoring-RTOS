#ifndef SENSOR_IF_H
#define SENSOR_IF_H

#include "app_types.h"

void SensorIF_Init(void);
void SensorIF_ReadAll(SensorFrame_t *frame);

#endif
