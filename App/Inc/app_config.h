#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include <stdint.h>

#define APP_SENSOR_PERIOD_MS         500U
#define APP_MONITOR_PERIOD_MS        200U
#define APP_ACTUATOR_PERIOD_MS       100U
#define APP_LOGGER_PERIOD_MS         100U

#define APP_UART_BAUDRATE            115200U
#define APP_LOG_QUEUE_LENGTH         16U
#define APP_SENSOR_QUEUE_LENGTH      8U
#define APP_LOG_LINE_MAX             128U
#define APP_COMM_PERIOD_MS           1000U

#define TEMP_WARNING_C               45.0f
#define TEMP_ALARM_C                 60.0f

#define GAS_WARNING_PERCENT          40.0f
#define GAS_ALARM_PERCENT            65.0f

#define VIB_ACTIVE_LEVEL             1U
#define LEVEL_ACTIVE_LEVEL           1U

#define ADC_MAX_COUNTS               4095.0f
#define ADC_VREF                     3.3f

/* LM35 style conversion: 10mV / degree C */
#define TEMP_SENSOR_VOLTS_PER_C      0.010f

/* Example mapping of 0-3.3V to 0-100% gas indicator */
#define GAS_SENSOR_MAX_PERCENT       100.0f

#endif
