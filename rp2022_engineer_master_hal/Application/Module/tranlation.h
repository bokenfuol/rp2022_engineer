#ifndef __TRANLATION_H
#define __TRANLATION_H

#include "config.h"
#include "motor.h"

#define MAX_TRANLASTION 23100

#define LEFT_TRANLASION 2000
#define MID_TRANLASION 11835
#define RIGHT_TRALASION 21000
 
typedef enum{
	TRANLATION_MOTOR,
  TRANLATION_MOTOR_CNT	
}tranlation_motor_cnt_t;

typedef struct tranlation_struct{
	position_motor_t 				motor[TRANLATION_MOTOR_CNT];
}tranlation_t;

extern tranlation_t tranlation;

void tranlation_check(void);

#endif
