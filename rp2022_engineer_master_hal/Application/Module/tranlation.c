/**
 * @file        tranlation.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        8-December-2021
 * @brief       ∫·“∆
 */

 
#include "tranlation.h"


tranlation_t tranlation = {
	.motor = {
		[TRANLATION_MOTOR] = {
			.info = {
				.offline_max_cnt = 100,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x203,
				.CAN_select = HCAN1,
			},
			.speed_pid = {
				.kp = 8,
				.ki = 0,
				.kd = 0,
				.integral_max = 0,
				.max_abs = 0,
				.out_max = 10000,
			},
			.position_pid = {
				.kp = 4,
				.ki = 0,
				.kd = 0,
				.integral_max = 0,
				.max_abs = 0,
				.out_max = 3000,				
			},
		},
	},
};


/**
 * @date        16-November-2021
 * @brief       tranlation_check
 */
void tranlation_check()
{
	motor.pos_check(&tranlation.motor[TRANLATION_MOTOR]);
}
