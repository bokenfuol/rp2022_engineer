#ifndef __SAVE_H
#define __SAVE_H

#include "config.h"
#include "motor.h"
#include "pneumatic.h"


#define MAX_SAVE	68000


//英雄
#define SAVE_CARD_HERO							33995
//麦轮步兵
#define SAVE_CARD_STANDARD1					43503
//全向轮步兵
#define SAVE_CARD_STANDARD2					MAX_SAVE
//舵轮步兵
#define SAVE_CARD_STANDARD3					MAX_SAVE

typedef enum{
  SAVE_CARD_MOTOR,
  SAVE_MOTOR_CNT	
}save_motor_cnt_t;

typedef struct save_struct{
	position_motor_t 				motor[SAVE_MOTOR_CNT];
}save_t;

extern save_t save;

void save_check(void);

#endif
