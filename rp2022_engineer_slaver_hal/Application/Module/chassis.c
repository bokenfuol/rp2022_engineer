/**
 * @file        chassis_motor.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        2-November-2021
 * @brief       底盘
 */

#include "chassis.h"
#include "rc_sensor.h"

//4:201 3:204 1:202 2:203

/**
 * @date        12-November-2021
 * @brief       底盘轮组结构体
 */
chassis_motor_t chassis_motor[] = {
/**
 * @brief       左前轮组
 */
	[CHAS_FL] = {
		.id = CHAS_FL,
		.veer_state = COROTATION,
		.mid_angle = LF_MidAngle/8192.f*360.f,
		.motor_barke = NORMAL,
/*-------------------------动力电机----------------------------*/		
		.power_motor = {
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
				.integral_max = 6000,
				.out_max = 10000,
			},
		},	
/*-------------------------舵向电机-----------==----------------*/
		.rudder_motor = {
			.info = {
				.offline_max_cnt = 50,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x207,
				.CAN_select = HCAN2,
			},
			.speed_pid = {
				.kp = 20,
				.ki = 0.1,
				.kd = 0,
				.integral_max = 1500,
				.out_max = 30000,
			},
			.position_pid = {
				.kp = 40,
				.ki = 0,
				.kd = 0,
				.integral_max = 0,
				.out_max = 4000,				
			},
		},	
	},
/*------------------------------------------------------------*/

	
	
	
/**
 * @brief       右前轮组
 */	
	[CHAS_FR] = {
		.id = CHAS_FR,
		.veer_state = COROTATION,
		.mid_angle = RF_MidAngle/8192.f*360.f,
		.motor_barke = NORMAL,
/*-------------------------动力电机----------------------------*/		
		.power_motor = {
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
				.integral_max = 6000,
				.out_max = 10000,
			},
		},	
/*-------------------------舵向电机-----------==----------------*/
		.rudder_motor = {
			.info = {
				.offline_max_cnt = 50,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x208,
				.CAN_select = HCAN2,
			},
			.speed_pid = {
				.kp = 20,
				.ki = 0.1,
				.kd = 0,
				.integral_max = 1500,
				.out_max = 30000,
			},
			.position_pid = {
				.kp = 40,
				.ki = 0,
				.kd = 0,
				.integral_max = 0,
				.out_max = 4000,				
			},			
		},	
	},
/*------------------------------------------------------------*/



	
/**
 * @brief       左后轮组
 */
	[CHAS_BL] = {
		.id = CHAS_BL,
		.veer_state = COROTATION,
		.mid_angle = LB_MidAngle/8192.f*360.f,
		.motor_barke = NORMAL,
/*-------------------------动力电机----------------------------*/		
		.power_motor = {
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
				.integral_max = 6000,
				.out_max = 10000,
			},
		},	
/*-------------------------舵向电机-----------==----------------*/
		.rudder_motor = {
			.info = {
				.offline_max_cnt = 50,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x206,
				.CAN_select = HCAN2,
			},
			.speed_pid = {
				.kp = 20,
				.ki = 0.1,
				.kd = 0,
				.integral_max = 1500,
				.out_max = 30000,
			},
			.position_pid = {
				.kp = 40,
				.ki = 0,
				.kd = 0,
				.integral_max = 0,
				.out_max = 4000,				
			},						
		},	
	},
/*------------------------------------------------------------*/



	
/**
 * @brief       右后轮组
 */	
	[CHAS_BR] = {
		.id = CHAS_BR,
		.veer_state = COROTATION,
		.mid_angle = RB_MidAngle/8192.f*360.f,
		.motor_barke = NORMAL,
/*-------------------------动力电机----------------------------*/		
		.power_motor = {
			.info = {
				.offline_max_cnt = 50,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x203,
				.CAN_select = HCAN2,
			},
			.speed_pid = {
				.kp = 8.5,
				.ki = 0.33,
				.kd = 0,
				.integral_max = 6000,
				.out_max = 10000,
			},
		},	
/*-------------------------舵向电机-----------==----------------*/
		.rudder_motor = {
			.info = {
				.offline_max_cnt = 50,
				.work_state = DEV_OFFLINE,
				.motor_id = 0x205,
				.CAN_select = HCAN2,
			},
			.speed_pid = {
				.kp = 20,
				.ki = 0.1,
				.kd = 0,
				.integral_max = 1500,
				.out_max = 30000,
			},
			.position_pid = {
				.kp = 40,
				.ki = 0,
				.kd = 0,
				.integral_max = 0,
				.out_max = 4000,				
			},	
		},	
	},
/*------------------------------------------------------------*/
};





/**
 * @date        2-November-2021
 * @brief       chassis_check
*/
void chassis_check()
{
	set_target(&chassis_motor[CHAS_FL]);
	set_target(&chassis_motor[CHAS_FR]);
	set_target(&chassis_motor[CHAS_BL]);
	set_target(&chassis_motor[CHAS_BR]);	
	
	motor.ang_check(&chassis_motor[CHAS_FL].rudder_motor);
	motor.ang_check(&chassis_motor[CHAS_FR].rudder_motor);
	motor.ang_check(&chassis_motor[CHAS_BL].rudder_motor);
	motor.ang_check(&chassis_motor[CHAS_BR].rudder_motor);
	
	motor.spe_check(&chassis_motor[CHAS_FL].power_motor);
	motor.spe_check(&chassis_motor[CHAS_FR].power_motor);
	motor.spe_check(&chassis_motor[CHAS_BL].power_motor);
	motor.spe_check(&chassis_motor[CHAS_BR].power_motor);
	
	/*判断卸力*/
	if(chassis_motor[CHAS_FL].motor_barke == DISCHARGE)
		chassis_motor[CHAS_FL].power_motor.info.given = 0;
	if(chassis_motor[CHAS_FR].motor_barke == DISCHARGE)
		chassis_motor[CHAS_FR].power_motor.info.given = 0;
	if(chassis_motor[CHAS_BL].motor_barke == DISCHARGE)
		chassis_motor[CHAS_BL].power_motor.info.given = 0;
	if(chassis_motor[CHAS_FR].motor_barke == DISCHARGE)
		chassis_motor[CHAS_FR].power_motor.info.given = 0;
}


/**
 * @date        2-November-2021
 * @brief       底盘电机目标值设定
 */
void set_target(chassis_motor_t *chas)
{
	if(IS_MOTORx_ONLINE(chas->rudder_motor)&&IS_RC_ONLINE)
	{
		chas->power_motor.speed_pid.target = chas->speed_target;
		chas->rudder_motor.position_pid.target = chas->angle_target;
	}
}
 

