/**
 * @file        clamp.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        8-December-2021
 * @brief       МазІ
 */
#include "clamp.h"


clamp_t clamp = {
	.pneu = &pneu_clamp,
	.keylight = &key_clamp,
	.motor = {
		[CLAMP_MOTOR_LEFT] = {
			.info = {
				.offline_max_cnt = 50,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x205,
				.CAN_select = HCAN1,
			},
			.speed_pid = {
				.kp = 8,
				.ki = 0,
				.kd = 0,
				.integral_max = 6000,
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
		[CLAMP_MOTOR_RIGHT] = {
			.info = {
				.offline_max_cnt = 50,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x204,
				.CAN_select = HCAN1
			},
			.speed_pid = {
				.kp = 8,
				.ki = 0,
				.kd = 0,
				.integral_max = 6000,
				.max_abs = 0,
				.out_max = 8000,
			},
			.position_pid = {
				.kp = 5.0,
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
 * @brief       clamp_check
 */
void clamp_check()
{
	motor.pos_check(&clamp.motor[CLAMP_MOTOR_LEFT]);
	clamp.motor[CLAMP_MOTOR_RIGHT].info.given = - clamp.motor[CLAMP_MOTOR_LEFT].info.given;
	Pneu_Check(clamp.pneu->state,clamp.pneu);
	clamp.keylight->state = Keylight_Input(clamp.keylight);	;
}
