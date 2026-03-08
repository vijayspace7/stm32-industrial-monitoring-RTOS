#include "monitor.h"
#include "app_config.h"
#include <stdio.h>
#include <string.h>

MonitorResult_t Monitor_Evaluate(const SensorFrame_t *frame)
{
    MonitorResult_t result;
    result.state = SYS_NORMAL;
    result.eventBits = 0U;

    if (frame == NULL)
    {
        result.state = SYS_ALARM;
        result.eventBits = EVT_SYSTEM_ALARM;
        return result;
    }

    if (frame->temperatureC >= TEMP_WARNING_C)
    {
        result.eventBits |= EVT_WARN_TEMP;
        result.state = SYS_WARNING;
    }
    if (frame->temperatureC >= TEMP_ALARM_C)
    {
        result.eventBits |= EVT_ALARM_TEMP;
        result.eventBits |= EVT_SYSTEM_ALARM;
        result.state = SYS_ALARM;
    }

    if (frame->gasPercent >= GAS_WARNING_PERCENT)
    {
        result.eventBits |= EVT_WARN_GAS;
        if (result.state == SYS_NORMAL)
        {
            result.state = SYS_WARNING;
        }
    }
    if (frame->gasPercent >= GAS_ALARM_PERCENT)
    {
        result.eventBits |= EVT_ALARM_GAS;
        result.eventBits |= EVT_SYSTEM_ALARM;
        result.state = SYS_ALARM;
    }

    if (frame->vibrationActive)
    {
        result.eventBits |= EVT_ALARM_VIB;
        result.eventBits |= EVT_SYSTEM_ALARM;
        result.state = SYS_ALARM;
    }

    if (frame->levelActive)
    {
        result.eventBits |= EVT_ALARM_LEVEL;
        result.eventBits |= EVT_SYSTEM_ALARM;
        result.state = SYS_ALARM;
    }

    return result;
}

const char *Monitor_StateString(SystemState_t state)
{
    switch (state)
    {
        case SYS_NORMAL:  return "NORMAL";
        case SYS_WARNING: return "WARNING";
        case SYS_ALARM:   return "ALARM";
        default:          return "UNKNOWN";
    }
}

void Monitor_FormatAlarmString(uint32_t eventBits, char *out, unsigned int outLen)
{
    int first = 1;

    if ((out == NULL) || (outLen == 0U))
    {
        return;
    }

    out[0] = '\0';

    if (eventBits & EVT_ALARM_TEMP)
    {
        snprintf(out + strlen(out), outLen - strlen(out), "%sTEMP", first ? "" : ",");
        first = 0;
    }
    if (eventBits & EVT_ALARM_GAS)
    {
        snprintf(out + strlen(out), outLen - strlen(out), "%sGAS", first ? "" : ",");
        first = 0;
    }
    if (eventBits & EVT_ALARM_VIB)
    {
        snprintf(out + strlen(out), outLen - strlen(out), "%sVIBRATION", first ? "" : ",");
        first = 0;
    }
    if (eventBits & EVT_ALARM_LEVEL)
    {
        snprintf(out + strlen(out), outLen - strlen(out), "%sLEVEL", first ? "" : ",");
        first = 0;
    }

    if (first)
    {
        snprintf(out, outLen, "NONE");
    }
}
