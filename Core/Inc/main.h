#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"

void Error_Handler(void);

/* GPIO pin mapping */
#define GREEN_LED_Pin            GPIO_PIN_5
#define GREEN_LED_GPIO_Port      GPIOA

#define RED_LED_Pin              GPIO_PIN_6
#define RED_LED_GPIO_Port        GPIOA

#define BUZZER_Pin               GPIO_PIN_7
#define BUZZER_GPIO_Port         GPIOA

#define VIB_Pin                  GPIO_PIN_0
#define VIB_GPIO_Port            GPIOB

#define LEVEL_Pin                GPIO_PIN_1
#define LEVEL_GPIO_Port          GPIOB

#define RELAY_Pin                GPIO_PIN_6
#define RELAY_GPIO_Port          GPIOB

/* SPI1 chip select for external gateway / flash / industrial front-end */
#define SPI1_CS_Pin              GPIO_PIN_4
#define SPI1_CS_GPIO_Port        GPIOA

#ifdef __cplusplus
}
#endif

#endif
