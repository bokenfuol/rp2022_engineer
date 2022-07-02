/**
 * @file        lifting.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        8-December-2021
 * @brief       Ì§Éı
 */

 
#include "lifting.h"


lifting_t lifting = {
	.pneu = &pneu_protract,
	.motor = {
		[LIFTING_MOTOR_L] = {
			.info = {
				.offline_max_cnt = 50,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x205,
				.CAN_select = HCAN2,
			},
			.speed_pid = {
				.kp = 8,
				.ki = 0.1,
				.kd = 0,
				.integral_max = 6000,
				.max_abs = 0,
				.out_max = 8000,
			},
			.position_pid = {
				.kp = 6.2,
				.ki = 0,
				.kd = 0,
				.integral_max = 0,
				.max_abs = 0,
				.out_max = 5000,				
			},
		},
		[LIFTING_MOTOR_R] = {
			.info = {
				.offline_max_cnt = 50,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x206,
				.CAN_select = HCAN2,
			},
			.speed_pid = {
				.kp = 8,
				.ki = 0.1,
				.kd = 0,
				.integral_max = 6000,
				.max_abs = 0,
				.out_max = 8000,
			},
			.position_pid = {
				.kp = 6.2,
				.ki = 0,
				.kd = 0,
				.integral_max = 0,
				.max_abs = 0,
				.out_max = 5000,				
			},
		},
	},
};


/**
 * @date        16-November-2021
 * @brief       lifting_check
 */
void lifting_check()
{
	motor.pos_check(&lifting.motor[LIFTING_MOTOR_L]);
	motor.pos_check(&lifting.motor[LIFTING_MOTOR_R]);
	Pneu_Check(lifting.pneu->state,lifting.pneu);
}
