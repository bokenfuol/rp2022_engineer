/**
 * @file        ground.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        8-December-2021
 * @brief       µØ¿ó
 */
#include "ground.h"


ground_t ground = {
	.motor = {
			[GROUND_MOTOR_L] = {
			.info = {
				.offline_max_cnt = 50,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x203,
				.CAN_select = HCAN2,
			},
			.speed_pid = {
				.kp = 8.5,
				.ki = 0,
				.kd = 0,
				.integral_max = 4000,
				.max_abs = 0,
				.out_max = 10000,
			},
			.position_pid = {
				.kp = 6,
				.ki = 0,
				.kd = 0,
				.integral_max = 0,
				.max_abs = 0,
				.out_max = 8000,				
			},
		},
		[GROUND_MOTOR_R] = {
			.info = {
				.offline_max_cnt = 50,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x202,
				.CAN_select = HCAN2,
			},
			.speed_pid = {
				.kp = 8.5,
				.ki = 0.33,
				.kd = 0,
				.integral_max = 4000,
				.max_abs = 0,
				.out_max = 10000,
			},
			.position_pid = {
				.kp = 4,
				.ki = 0,
				.kd = 0,
				.integral_max = 0,
				.max_abs = 0,
				.out_max = 8000,				
			},
		},
		[GROUND_MOTOR_T] = {
			.info = {
				.offline_max_cnt = 50,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x204,
				.CAN_select = HCAN2,
			},
			.speed_pid = {
				.kp = 8.5,
				.ki = 0.33,
				.kd = 0,
				.integral_max = 4000,
				.max_abs = 0,
				.out_max = 10000,
			},
			.position_pid = {
				.kp = 4,
				.ki = 0,
				.kd = 0,
				.integral_max = 0,
				.max_abs = 0,
				.out_max = 8000,				
			},
		},
	},
};


/**
 * @date        16-November-2021
 * @brief       ground_check
 */
void ground_check()
{
	motor.pos_check(&ground.motor[GROUND_MOTOR_L]);
	ground.motor[GROUND_MOTOR_R].info.given = - ground.motor[GROUND_MOTOR_L].info.given;
	motor.pos_check(&ground.motor[GROUND_MOTOR_T]);
}
