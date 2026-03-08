#include "sensor_if.h"
#include "app_config.h"
#include "main.h"
#include "cmsis_os2.h"

extern ADC_HandleTypeDef hadc1;

static uint32_t adcValues[2];

static float ADC_ToVoltage(uint32_t counts)
{
    return ((float)counts * ADC_VREF) / ADC_MAX_COUNTS;
}

static float ConvertTempC(uint32_t counts)
{
    float volts = ADC_ToVoltage(counts);
    return volts / TEMP_SENSOR_VOLTS_PER_C;
}

static float ConvertGasPercent(uint32_t counts)
{
    float volts = ADC_ToVoltage(counts);
    float pct = (volts / ADC_VREF) * GAS_SENSOR_MAX_PERCENT;

    if (pct < 0.0f)
    {
        pct = 0.0f;
    }
    if (pct > 100.0f)
    {
        pct = 100.0f;
    }
    return pct;
}

void SensorIF_Init(void)
{
    adcValues[0] = 0U;
    adcValues[1] = 0U;
}

void SensorIF_ReadAll(SensorFrame_t *frame)
{
    if (frame == NULL)
    {
        return;
    }

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10U);
    adcValues[0] = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10U);
    adcValues[1] = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    frame->rawTemp = adcValues[0];
    frame->rawGas = adcValues[1];
    frame->temperatureC = ConvertTempC(frame->rawTemp);
    frame->gasPercent = ConvertGasPercent(frame->rawGas);
    frame->vibrationActive = (HAL_GPIO_ReadPin(VIB_GPIO_Port, VIB_Pin) == GPIO_PIN_SET) ? 1U : 0U;
    frame->levelActive = (HAL_GPIO_ReadPin(LEVEL_GPIO_Port, LEVEL_Pin) == GPIO_PIN_SET) ? 1U : 0U;
    frame->tickMs = osKernelGetTickCount();
}
