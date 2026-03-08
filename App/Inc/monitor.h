#ifndef MONITOR_H
#define MONITOR_H

#include "app_types.h"

#define EVT_WARN_TEMP        (1U << 0)
#define EVT_ALARM_TEMP       (1U << 1)
#define EVT_WARN_GAS         (1U << 2)
#define EVT_ALARM_GAS        (1U << 3)
#define EVT_ALARM_VIB        (1U << 4)
#define EVT_ALARM_LEVEL      (1U << 5)
#define EVT_SYSTEM_ALARM     (1U << 6)

MonitorResult_t Monitor_Evaluate(const SensorFrame_t *frame);
const char *Monitor_StateString(SystemState_t state);
void Monitor_FormatAlarmString(uint32_t eventBits, char *out, unsigned int outLen);

#endif
