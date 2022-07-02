#ifndef __LED_H
#define __LED_H

#include "config.h"

#define GREEN_ON		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET)
#define BLUE_ON			HAL_GPIO_WritePin(LED_BLUE_GPIO_Port,LED_BLUE_Pin,GPIO_PIN_RESET)
#define RED_ON			HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_RESET)
#define ORANGE_ON		HAL_GPIO_WritePin(LED_ORANGE_GPIO_Port,LED_ORANGE_Pin,GPIO_PIN_RESET)

#define GREEN_OFF		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET)
#define BLUE_OFF		HAL_GPIO_WritePin(LED_BLUE_GPIO_Port,LED_BLUE_Pin,GPIO_PIN_SET)
#define RED_OFF			HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET)
#define ORANGE_OFF	HAL_GPIO_WritePin(LED_ORANGE_GPIO_Port,LED_ORANGE_Pin,GPIO_PIN_SET)


void system_led(void);

#endif

