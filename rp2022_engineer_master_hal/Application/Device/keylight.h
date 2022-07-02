#ifndef __KEYLIGHT_H
#define __KEYLIGHT_H

#include "config.h"
#include "stdbool.h"

#define IF_KEYLIGHT_CLAMP				!HAL_GPIO_ReadPin(KEY_CLAMP_GPIO_Port,KEY_CLAMP_Pin)
#define IF_KEYLIGHT_CABIN_UP		!HAL_GPIO_ReadPin(KEY_CABIN_UP_GPIO_Port,KEY_CABIN_UP_Pin)
#define IF_KEYLIGHT_CABIN_MID		!HAL_GPIO_ReadPin(KEY_CABIN_MID_GPIO_Port,KEY_CABIN_MID_Pin)
#define IF_KEYLIGHT_CABIN_DOWN	!HAL_GPIO_ReadPin(KEY_CABIN_DOWN_GPIO_Port,KEY_CABIN_DOWN_Pin)

typedef struct keylight_info_struct{
	GPIO_TypeDef* GPIOx;
	uint16_t 			GPIO_Pin;
	state_t				state;
	void 					(*input)(struct keylight_info_struct *self);
}keylight_info_t;

extern keylight_info_t key_clamp;
extern keylight_info_t key_cabin_up;
extern keylight_info_t key_cabin_mid;
extern keylight_info_t key_cabin_down;

bool Keylight_Input(keylight_info_t *self);

#endif
