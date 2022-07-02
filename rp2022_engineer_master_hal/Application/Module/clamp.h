#ifndef __CLAMP_H
#define __CLAMP_H

#include "config.h"
#include "motor.h"
#include "pneumatic.h"
#include "keylight.h"


#define MAX_CLAMP					4100

#define AIR_CLAMP					1300
#define GOLDERN_CALMP		 	0
#define SILVER_CLAMP		 	367
#define GROUND_CLAMP		 	0
#define CONVERSION_CLAMP	2000

#define BACK_CLAMP				3625

typedef enum{
  CLAMP_MOTOR_LEFT,
	CLAMP_MOTOR_RIGHT,
  CLAMP_MOTOR_CNT	
}clamp_motor_cnt_t;

typedef struct clamp_struct{
	position_motor_t 				motor[CLAMP_MOTOR_CNT];
	pneu_info_t							*pneu;
	keylight_info_t					*keylight;
}clamp_t;

extern clamp_t clamp;

void clamp_check(void);

#endif
