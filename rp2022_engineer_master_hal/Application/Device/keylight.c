/**
 * @file        keylight.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        14-January-2022
 * @brief       ¹âµç
 */

#include "keylight.h"


keylight_info_t key_clamp = {
	.GPIOx = KEY_CLAMP_GPIO_Port,
	.GPIO_Pin = KEY_CLAMP_Pin,
	.state = OFF,
};


keylight_info_t key_cabin_up = {
	.GPIOx = KEY_CABIN_UP_GPIO_Port,
	.GPIO_Pin = KEY_CABIN_UP_Pin,
	.state = OFF,
};


keylight_info_t key_cabin_mid = {
	.GPIOx = KEY_CABIN_MID_GPIO_Port,
	.GPIO_Pin = KEY_CABIN_MID_Pin,
	.state = OFF,
};


keylight_info_t key_cabin_down = {
	.GPIOx = KEY_CABIN_DOWN_GPIO_Port,
	.GPIO_Pin = KEY_CABIN_DOWN_Pin,
	.state = OFF,
};

bool Keylight_Input(keylight_info_t *self)
{
	return (!HAL_GPIO_ReadPin(self->GPIOx,self->GPIO_Pin));
}
