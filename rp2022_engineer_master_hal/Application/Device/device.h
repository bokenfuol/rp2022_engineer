#ifndef __DEVICE_H
#define __DEVICE_H

#include "config.h"
#include "motor.h"

typedef enum{
	DEVICE_01,
	DEVICE_02,
	DEVICE_03,
	DEVICE_04,
	DEVICE_05,
	DEVICE_06,
	DEVICE_07,
	DEVICE_08,
	DEVICE_09,
	DEVICE_10,
	DEVICE_11,
	DEVICE_CNT
}device_cnt_t;


typedef struct{
	motor_info_t						*motor;
}device_t;


extern device_t device[DEVICE_CNT];


#endif
