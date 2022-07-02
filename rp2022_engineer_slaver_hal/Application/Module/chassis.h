#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "config.h"
#include "motor.h"


/*舵向轮机械中值*/
#define LF_MidAngle     6210
#define RF_MidAngle     7498
#define LB_MidAngle     7357
#define RB_MidAngle     695


typedef enum {
    CHAS_FL,//前左
    CHAS_FR,
    CHAS_BL,//后左
    CHAS_BR,
    CHAS_MOTOR_CNT
} chassis_motor_cnt_t;

typedef enum {
	NORMAL,//正常
	DISCHARGE,//卸力
}motor_barke_state_t;

typedef enum { 
	REVERSAL,//反转
	COROTATION,//正转
}veer_state_t;


/*-------------轮组结构体------------*/
typedef struct chassis_motor_struct{
	speed_motor_t 					power_motor;
	position_motor_t				rudder_motor;
	
	uint8_t									id;
	float										mid_angle;
	
	float										speed_target;
	float 									angle_target;
	
	motor_barke_state_t			motor_barke;
	veer_state_t 						veer_state;
}chassis_motor_t;


extern chassis_motor_t chassis_motor[CHAS_MOTOR_CNT];

void chassis_check(void);
void set_target(chassis_motor_t *chas);

#endif
