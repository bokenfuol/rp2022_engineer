/**
 * @file        cabin.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        8-December-2021
 * @brief       È¡¿ó
 */

 
#include "cabin.h"


cabin_t cabin = {
	.keylight_up = &key_cabin_up,
	.keylight_down = &key_cabin_down,
	.motor = {
		[CABIN_MOTOR_L] = {
			.info = {
				.offline_max_cnt = 50,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x201,
				.CAN_select = HCAN1,
			},
			.speed_pid = {
				.kp = 8,
				.ki = 0,
				.kd = 0,
				.integral_max = 4000,
				.max_abs = 0,
				.out_max = 10000,
			},
		},
		[CABIN_MOTOR_R] = {
			.info = {
				.offline_max_cnt = 50,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x202,
				.CAN_select = HCAN1,
			},
			.speed_pid = {
				.kp = 8,
				.ki = 0,
				.kd = 0,
				.integral_max = 4000,
				.max_abs = 0,
				.out_max = 10000,
			},
		},
	},
};


/**
 * @date        16-November-2021
 * @brief       cabin_check
 */
void cabin_check()
{
	motor.spe_check(&cabin.motor[CABIN_MOTOR_L]);
	motor.spe_check(&cabin.motor[CABIN_MOTOR_R]);
	cabin.keylight_up->state = Keylight_Input(cabin.keylight_up);	
	cabin.keylight_down->state = Keylight_Input(cabin.keylight_down);	
}
