#ifndef __CONTROL_H
#define __CONTROL_H

#include "config.h"

#define IS_SYSTEM_RESET	(control.master_reset == RESET_OK)

typedef enum
{
	NORMAL_MODE,//常规模式
	AIR_MODE,//空接模式
	SILVERORE_MODE,//银矿模式
	GOLDENORE_MODE,//金矿模式
	GROUND_MODE,//地矿模式
	CONVERSION_MODE,//兑换模式
	SAVE_MODE,//救援模式
	BARRIER_MODE,//障碍块模式
	BLOCK_MODE,//格挡模式
	SPINNING_MODE,//陀螺模式
}system_mode_t;



typedef struct {
	reset_state_t				 	master_reset;	//主板复位标志
	reset_state_t					slaver_reset;	//从板复位标志
	system_mode_t 				system_mode;	//系统任务
	
	state_t				 				vision_state;	//视觉处理数据指令
		
	int32_t								master_motor_online;
	int32_t								motor_online;
}control_struct;


extern control_struct control;

void general_control(void);
void general_reset(void);


#endif

