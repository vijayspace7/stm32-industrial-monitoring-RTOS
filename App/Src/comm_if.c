#include "comm_if.h"
#include "main.h"
#include "cmsis_os2.h"
#include <string.h>

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi1;

#define DEMO_I2C_DEVICE_ADDR_7BIT   0x50U
#define DEMO_SPI_PING_TX            0xA5U

void CommIF_Init(void)
{
}

void CommIF_Service(CommStatus_t *status)
{
    HAL_StatusTypeDef halStatus;
    uint8_t whoami = 0x5AU; /* demo placeholder when a simple EEPROM is fitted */
    uint8_t tx = DEMO_SPI_PING_TX;
    uint8_t rx = 0x00U;

    if (status == NULL)
    {
        return;
    }

    memset(status, 0, sizeof(*status));

    /* I2C channel check: probe a 24LCxx/AT24Cxx style EEPROM or any 0x50 device. */
    halStatus = HAL_I2C_IsDeviceReady(&hi2c1, (DEMO_I2C_DEVICE_ADDR_7BIT << 1), 2U, 20U);
    if (halStatus == HAL_OK)
    {
        status->i2cOk = 1U;
        status->lastI2cDeviceId = whoami;
    }

    /* SPI channel check: loopback header or external slave can echo this frame. */
    halStatus = HAL_SPI_TransmitReceive(&hspi1, &tx, &rx, 1U, 20U);
    if (halStatus == HAL_OK)
    {
        status->spiOk = 1U;
        status->lastSpiStatus = rx;
    }

    status->tickMs = osKernelGetTickCount();
}
