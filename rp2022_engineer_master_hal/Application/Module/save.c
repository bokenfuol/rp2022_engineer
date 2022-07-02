/**
 * @file        save.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        8-December-2021
 * @brief       æ»‘Æ
 */
#include "save.h"


save_t save = {
	.motor = {
		[SAVE_CARD_MOTOR] = {
			.info = {
				.offline_max_cnt = 50,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x201,
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
 * @brief       save_check
 */
void save_check()
{
	motor.pos_check(&save.motor[SAVE_CARD_MOTOR]);
}
