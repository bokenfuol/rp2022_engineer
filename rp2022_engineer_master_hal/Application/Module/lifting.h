#ifndef __LIFTING_H
#define __LIFTING_H

#include "config.h"
#include "motor.h"
#include "pneumatic.h"


//#define MAX_LIFT	20000
#define MAX_LIFT	19115

#define AIR_LIFT 				MAX_LIFT
#define GOLDERN_LIFT 		8000
#define SILVER_LIFT 		17602
#define CONVERSION_LIFT MAX_LIFT
#define BLOCK_LIFT 			MAX_LIFT
#define SAVE_LIFT				5527
#define SWITCH_LIFT 		10000

typedef enum{
  LIFTING_MOTOR_L,
	LIFTING_MOTOR_R,
  LIFTING_MOTOR_CNT	
}lifting_motor_cnt_t;

typedef struct lifting_struct{
	position_motor_t 				motor[LIFTING_MOTOR_CNT];
	pneu_info_t							*pneu;
}lifting_t;

extern lifting_t lifting;

void lifting_check(void);

#endif
