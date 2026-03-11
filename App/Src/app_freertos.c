#include "main.h"
#include "cmsis_os2.h"
#include "app_config.h"
#include "app_types.h"
#include "sensor_if.h"
#include "monitor.h"
#include "ui_log.h"
#include "comm_if.h"
#include <stdio.h>
#include <string.h>

osThreadId_t sensorTaskHandle;
osThreadId_t monitorTaskHandle;
osThreadId_t actuatorTaskHandle;
osThreadId_t loggerTaskHandle;
osThreadId_t commTaskHandle;

osMessageQueueId_t sensorQueueHandle;
osMessageQueueId_t logQueueHandle;
osEventFlagsId_t eventFlagsHandle;
osMutexId_t uartMutexHandle;
osTimerId_t buzzerTimerHandle;

static SensorFrame_t g_latestFrame;
static MonitorResult_t g_latestResult;
static uint8_t g_buzzerOutputState = 0U;

typedef struct
{
    char line[APP_LOG_LINE_MAX];
} LogMessage_t;

static void SensorTask(void *argument);
static void MonitorTask(void *argument);
static void ActuatorTask(void *argument);
static void LoggerTask(void *argument);
static void CommTask(void *argument);
static void BuzzerTimerCallback(void *argument);
static void QueueLog(const char *text);

void MX_FREERTOS_Init(void)
{
    const osThreadAttr_t sensorTaskAttr = {
        .name = "sensorTask",
        .priority = (osPriority_t) osPriorityNormal,
        .stack_size = 512 * 4
    };
    const osThreadAttr_t monitorTaskAttr = {
        .name = "monitorTask",
        .priority = (osPriority_t) osPriorityAboveNormal,
        .stack_size = 512 * 4
    };
    const osThreadAttr_t actuatorTaskAttr = {
        .name = "actuatorTask",
        .priority = (osPriority_t) osPriorityNormal,
        .stack_size = 384 * 4
    };
    const osThreadAttr_t loggerTaskAttr = {
        .name = "loggerTask",
        .priority = (osPriority_t) osPriorityLow,
        .stack_size = 512 * 4
    };
    const osThreadAttr_t commTaskAttr = {
        .name = "commTask",
        .priority = (osPriority_t) osPriorityBelowNormal,
        .stack_size = 512 * 4
    };
    const osMutexAttr_t uartMutexAttr = {
        .name = "uartMutex"
    };
    const osTimerAttr_t buzzerTimerAttr = {
        .name = "buzzerTimer"
    };

    sensorQueueHandle = osMessageQueueNew(APP_SENSOR_QUEUE_LENGTH, sizeof(SensorFrame_t), NULL);
    logQueueHandle = osMessageQueueNew(APP_LOG_QUEUE_LENGTH, sizeof(LogMessage_t), NULL);
    eventFlagsHandle = osEventFlagsNew(NULL);
    uartMutexHandle = osMutexNew(&uartMutexAttr);
    buzzerTimerHandle = osTimerNew(BuzzerTimerCallback, osTimerPeriodic, NULL, &buzzerTimerAttr);

    sensorTaskHandle = osThreadNew(SensorTask, NULL, &sensorTaskAttr);
    monitorTaskHandle = osThreadNew(MonitorTask, NULL, &monitorTaskAttr);
    actuatorTaskHandle = osThreadNew(ActuatorTask, NULL, &actuatorTaskAttr);
    loggerTaskHandle = osThreadNew(LoggerTask, NULL, &loggerTaskAttr);
    commTaskHandle = osThreadNew(CommTask, NULL, &commTaskAttr);
}

static void SensorTask(void *argument)
{
    SensorFrame_t frame;
    (void)argument;

    SensorIF_Init();
    QueueLog("[BOOT] Industrial monitoring system started\r\n");
    QueueLog("[BOOT] Temperature sensor: LM35/TMP36 analog on ADC1_IN5\r\n");
    QueueLog("[BOOT] I2C1 and SPI1 channels enabled for expansion\r\n");

    for (;;)
    {
        SensorIF_ReadAll(&frame);
        g_latestFrame = frame;
        osMessageQueuePut(sensorQueueHandle, &frame, 0U, 0U);

        char line[APP_LOG_LINE_MAX];
        snprintf(line, sizeof(line), "[SENS] T=%.2fC GAS=%.2f%% VIB=%u LVL=%u\r\n",
                 frame.temperatureC,
                 frame.gasPercent,
                 frame.vibrationActive,
                 frame.levelActive);
        QueueLog(line);

        osDelay(APP_SENSOR_PERIOD_MS);
    }
}

static void MonitorTask(void *argument)
{
    SensorFrame_t frame;
    (void)argument;

    for (;;)
    {
        if (osMessageQueueGet(sensorQueueHandle, &frame, NULL, osWaitForever) == osOK)
        {
            char line[APP_LOG_LINE_MAX];
            char alarmBuf[64];
            g_latestResult = Monitor_Evaluate(&frame);

            osEventFlagsClear(eventFlagsHandle, 0xFFFFFFFFU);
            osEventFlagsSet(eventFlagsHandle, g_latestResult.eventBits);

            if (g_latestResult.state == SYS_ALARM)
            {
                Monitor_FormatAlarmString(g_latestResult.eventBits, alarmBuf, sizeof(alarmBuf));
                snprintf(line, sizeof(line), "[ALRM] %s\r\n", alarmBuf);
            }
            else if (g_latestResult.state == SYS_WARNING)
            {
                snprintf(line, sizeof(line), "[STAT] WARNING\r\n");
            }
            else
            {
                snprintf(line, sizeof(line), "[STAT] NORMAL\r\n");
            }
            QueueLog(line);
        }
    }
}

static void ActuatorTask(void *argument)
{
    uint32_t bits;
    (void)argument;

    for (;;)
    {
        bits = osEventFlagsGet(eventFlagsHandle);

        if (bits & EVT_SYSTEM_ALARM)
        {
            HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);
            osTimerStart(buzzerTimerHandle, 300U);
        }
        else if (bits & (EVT_WARN_TEMP | EVT_WARN_GAS))
        {
            HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
            osTimerStop(buzzerTimerHandle);
            HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
            osTimerStop(buzzerTimerHandle);
            HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
        }

        osDelay(APP_ACTUATOR_PERIOD_MS);
    }
}

static void LoggerTask(void *argument)
{
    LogMessage_t msg;
    (void)argument;

    Log_Init();

    for (;;)
    {
        if (osMessageQueueGet(logQueueHandle, &msg, NULL, osWaitForever) == osOK)
        {
            osMutexAcquire(uartMutexHandle, osWaitForever);
            Log_Printf("%s", msg.line);
            osMutexRelease(uartMutexHandle);
        }
    }
}

static void CommTask(void *argument)
{
    CommStatus_t status;
    char line[APP_LOG_LINE_MAX];
    (void)argument;

    CommIF_Init();

    for (;;)
    {
        CommIF_Service(&status);
        snprintf(line, sizeof(line),
                 "[COMM] I2C=%s DEV=0x%02X SPI=%s RX=0x%02X\r\n",
                 status.i2cOk ? "OK" : "MISS",
                 status.lastI2cDeviceId,
                 status.spiOk ? "OK" : "MISS",
                 status.lastSpiStatus);
        QueueLog(line);
        osDelay(APP_COMM_PERIOD_MS);
    }
}

static void BuzzerTimerCallback(void *argument)
{
    (void)argument;
    g_buzzerOutputState ^= 1U;
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin,
                      g_buzzerOutputState ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void QueueLog(const char *text)
{
    LogMessage_t msg;

    if (text == NULL)
    {
        return;
    }

    memset(&msg, 0, sizeof(msg));
    strncpy(msg.line, text, sizeof(msg.line) - 1U);
    osMessageQueuePut(logQueueHandle, &msg, 0U, 0U);
}
