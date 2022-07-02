#ifndef __MOTOR_H
#define __MOTOR_H

#include "config.h"
#include "pid.h"

#define IS_MOTOR_ONLINE 		(motor->info.work_state == DEV_ONLINE)
#define IS_MOTORx_ONLINE(x)	(x.info.work_state == DEV_ONLINE)

typedef enum{
	HCAN1,
	HCAN2,
}can_select_t;

//底盘舵向电机信息结构体
typedef struct motor_info_struct{
	uint16_t	angle;
	int16_t		speed;
	int16_t		current;
	int16_t 		tempreture;
	
	uint16_t 	last_angle;	
	float			total_angle;
	
	int16_t  	given;
	
	uint8_t		offline_cnt;
	uint8_t		offline_max_cnt;
	dev_work_state_t	work_state;

	uint16_t motor_id;
	can_select_t CAN_select;
}motor_info_t;

typedef struct speed_motor_struct{
	motor_info_t							    info;
	pid_ctrl											speed_pid;
}speed_motor_t;

typedef struct position_motor_struct{
	motor_info_t						 			info;
	pid_ctrl											speed_pid;
	pid_ctrl											position_pid;
}position_motor_t;

typedef struct{
	void										(*update)(struct motor_info_struct *info,uint8_t *rxBuf);
	void										(*output)(struct motor_info_struct *info,uint8_t *txBuf);
	void										(*heart_beat)(struct motor_info_struct *motor);
	void										(*spe_check)(struct speed_motor_struct *motor);
	void										(*ang_check)(struct position_motor_struct *motor);
	void										(*pos_check)(struct position_motor_struct *motor);
}motor_t;

extern motor_t motor;

void speed_motor_check(speed_motor_t *motor);
void position_motor_check(position_motor_t *motor);
void angle_motor_check(position_motor_t *motor);
void motor_output(motor_info_t *motor_info,uint8_t *txBuf);
void motor_update(motor_info_t *motor_info,uint8_t *rxBuf);
void angle_constrain(float *angle);
void motor_heart_beat(motor_info_t *motor_info);

#endif
