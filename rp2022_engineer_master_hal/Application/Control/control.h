#ifndef __CONTROL_H
#define __CONTROL_H

#include "config.h"

#define IS_SYSTEM_RESET	(control.master_reset == RESET_OK)

typedef enum
{
	NORMAL_MODE=0,//常规模式
	AIR_MODE=1,//空接模式
	SILVERORE_MODE=2,//银矿模式
	GOLDENORE_MODE=3,//金矿模式
	GROUND_MODE=4,//地矿模式
	CONVERSION_MODE=5,//兑换模式
	SAVE_MODE=6,//救援模式
	BARRIER_MODE=7,//障碍块模式
	BLOCK_MODE=8,//格挡模式
	SPINNING_MODE=9,//陀螺模式
}system_mode_t;








typedef struct {
	reset_state_t				 	master_reset;	//主板复位标志
	reset_state_t					slaver_reset;	//从板复位标志
	system_mode_t 				system_mode;	//系统任务
}control_struct;


extern control_struct control;

void general_control(void);
void general_reset(void);


#endif

