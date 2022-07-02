#ifndef __MODE_SWITCH_H
#define __MODE_SWITCH_H

#include "config.h"
#include "control.h"

typedef struct {
	system_mode_t 				mode;//����ģʽ��־λ
	uint8_t								step;//ģʽ�л�����
}switch_mode_t;
	

extern switch_mode_t mode_switch;

#endif