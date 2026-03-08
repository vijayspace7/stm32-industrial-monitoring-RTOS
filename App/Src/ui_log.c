#include "ui_log.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

void Log_Init(void)
{
}

void Log_Printf(const char *fmt, ...)
{
    char buffer[160];
    int len;
    va_list args;

    va_start(args, fmt);
    len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    if (len < 0)
    {
        return;
    }
    if ((size_t)len > sizeof(buffer))
    {
        len = sizeof(buffer);
    }

    HAL_UART_Transmit(&huart2, (uint8_t *)buffer, (uint16_t)strlen(buffer), HAL_MAX_DELAY);
}
