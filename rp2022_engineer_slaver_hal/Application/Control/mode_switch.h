#ifndef __MODE_SWITCH_H
#define __MODE_SWITCH_H

#include "config.h"
#include "control.h"

typedef struct {
	system_mode_t 				mode;//进入模式标志位
	uint8_t								step;//模式切换步骤
}switch_mode_t;
	

extern switch_mode_t mode_switch;

#endif