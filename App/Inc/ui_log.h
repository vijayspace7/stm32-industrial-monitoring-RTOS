#ifndef UI_LOG_H
#define UI_LOG_H

#include <stdarg.h>

void Log_Init(void);
void Log_Printf(const char *fmt, ...);

#endif
