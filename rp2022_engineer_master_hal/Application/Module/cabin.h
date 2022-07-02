#ifndef __CABIN_H
#define __CABIN_H

#include "config.h"
#include "motor.h"
#include "keylight.h"

#define CABIN_SPEED_TARGET 5000
#define CABIN_LIFT_TARGET 3875

typedef enum{
	CABIN_MOTOR_L,
	CABIN_MOTOR_R,
  CABIN_MOTOR_CNT	
}cabin_motor_cnt_t;

typedef struct cabin_struct{
	speed_motor_t 				motor[CABIN_MOTOR_CNT];
	keylight_info_t				*keylight_up;
	keylight_info_t				*keylight_down;
}cabin_t;

extern cabin_t cabin;

void cabin_check(void);

#endif
