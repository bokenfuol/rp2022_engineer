/**
 * @file        vision_sensor.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        12-November-2021
 * @brief       视觉
 */

#include "vision_sensor.h"
#include "mode_switch.h"
#include "auto_process.h"
#include "judge_protocol.h"
#include "device.h"
#include "control.h"


/*视觉通信结构体*/
vision_sensor_t vision_sensor = {
	.rx = {
		.offline_max_cnt = 50,
		.float_1 = 0,
		.float_2 = 0,
		.int_1 = 0,
		.int_2 = 0,
		.int_3 = 0,
		.int_4 = 0,
	},
	.tx = {
		.sof = VISION_SOF,
		.float_1 = 0,
		.float_2 = 0,
		.int_1 = 0,
		.int_2 = 0,
		.int_3 = 0,
		.int_4 = 0,
	},
	.ckeck = vision_check,
	.heart_beat = vision_heart_beat,
	.output = vision_output,	
};



/**
 * @date        12-November-2021
 * @brief       vision_check
 */
void vision_check(vison_sensor_info_t *vis)
{
	
}


/**
 * @date        12-November-2021
 * @brief       vision_output
 */
void vision_output(vison_sensor_info_t *vis)
{
	 control.motor_online |= control.master_motor_online;
	for(uint8_t i = 0 ; i < 8 ; i ++)
		control.motor_online |= device[i].motor->work_state<<(i+11);
	if(auto_process.silver_flag)
		vis->int_1 = 10;
	else	
		vis->int_1 = mode_switch.mode;
	vis->int_2 = auto_process.save_solider;
	vis->int_3 = control.motor_online;
	control.motor_online = 0;
	//第一次掉矿
	if(judge.ext_game_status.game_progress == 4&&judge.ext_game_status.stage_remain_time>360)
		vis->int_4 = 1;
	//第二次掉矿
	else if(judge.ext_game_status.game_progress == 4&&judge.ext_game_status.stage_remain_time<300&&judge.ext_game_status.stage_remain_time>180)
		vis->int_4 = 2;
	else
		vis->int_4 = 0;
}


/**
 * @date        12-November-2021
 * @brief       视觉通信心跳
 */
void vision_heart_beat(vison_sensor_info_t *vis)
{
	vis->offline_cnt++;
	if(vis->offline_cnt > vis->offline_max_cnt)
	{
		vis->offline_cnt = vis->offline_max_cnt;
		vis->vision_sensor_state = DEV_OFFLINE;

	}
	else if(vis->vision_sensor_state == DEV_OFFLINE)
		vis->vision_sensor_state = DEV_ONLINE;
}


