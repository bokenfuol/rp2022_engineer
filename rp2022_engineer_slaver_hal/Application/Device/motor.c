/**
 * @file        motor.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        3-November-2021
 * @brief       电机
 */


#include "motor.h"
#include "rc_sensor.h"

motor_function_pointer_t motor = {
	.update = motor_update,
	.output = motor_output,
	.heart_beat = motor_heart_beat, 
	.spe_check = speed_motor_check,
	.ang_check = angle_motor_check,
	.pos_check = position_motor_check,
};

/**
 * @date        3-November-2021
 * @brief      	速度环PID输出
*/
void speed_motor_check(speed_motor_t *motor)
{
	//限值
	motor->speed_pid.target = constrain(motor->speed_pid.target , -8000, 8000);
	//PID计算输出
	motor->speed_pid.measure = motor->info.speed;
	if(!IS_MOTOR_ONLINE||!IS_RC_ONLINE)
	{
		motor->speed_pid.integral = 0;
		motor->speed_pid.target = 0;
		motor->speed_pid.measure = 0;	
	}
	motor->info.given = (int16_t)PIDControl(&motor->speed_pid);
}


/**
 * @date        3-November-2021
 * @brief       角度PID输出
 */
void angle_motor_check(position_motor_t *motor)
{
	//PID计算输出
	motor->position_pid.measure = motor->info.angle/8192.f*360.f;
	if(motor->position_pid.target - motor->position_pid.measure >180)
		motor->position_pid.target -= 360;
	else if(motor->position_pid.target - motor->position_pid.measure <= -180)
		motor->position_pid.target += 360;
	if(!IS_MOTOR_ONLINE||!IS_RC_ONLINE)
	{
		motor->position_pid.target = 0;
		motor->position_pid.measure = 0;
		motor->speed_pid.integral = 0;
		motor->speed_pid.target = 0;
		motor->speed_pid.measure = 0;	
	}
	motor->speed_pid.target = PIDControl(&motor->position_pid);
	motor->speed_pid.measure = motor->info.speed;
	motor->info.given = (int16_t)PIDControl(&motor->speed_pid);
}



/**
 * @date        16-November-2021
 * @brief       位置环PID输出
 */
void position_motor_check(position_motor_t *motor)
{
	//PID计算输出
	float offset_angle=0;
	offset_angle = (motor->info.angle - motor->info.last_angle)/8192.f*360.f;
	//过零点判断
	angle_constrain(&offset_angle);
	motor->info.total_angle += offset_angle;
	motor->info.last_angle = motor->info.angle;
	
	//PID计算输出
	motor->position_pid.measure = motor->info.total_angle;
	if(!IS_MOTOR_ONLINE||!IS_RC_ONLINE)
	{
		motor->position_pid.target = 0;
		motor->position_pid.measure = 0;
		motor->speed_pid.integral = 0;
		motor->speed_pid.target = 0;
		motor->speed_pid.measure = 0;	
	}
	motor->speed_pid.target = PIDControl(&motor->position_pid);
	motor->speed_pid.measure = motor->info.speed;
	motor->info.given = (int16_t)PIDControl(&motor->speed_pid);
}


/**
 * @date        2-November-2021
 * @brief       单个电机输出
 */
void motor_output(motor_info_t *motor_info,uint8_t *txBuf)
{
	switch(motor_info->motor_id)
	{
		case 0x201:
		case 0x205:
			*txBuf = (motor_info->given>>8) & 0xff;
			*(txBuf+1) = motor_info->given & 0xff;
		break;
		case 0x202:
		case 0x206:
			*(txBuf+2) = (motor_info->given>>8) & 0xff;
			*(txBuf+3) = motor_info->given & 0xff;	
		break;
		case 0x203:
		case 0x207:
			*(txBuf+4) = (motor_info->given>>8) & 0xff;
			*(txBuf+5) = motor_info->given & 0xff;	
		break;	
		case 0x204:
		case 0x208:
			*(txBuf+6) = (motor_info->given>>8) & 0xff;
			*(txBuf+7) = motor_info->given & 0xff;	
		break;		
	}
}


/**
 * @date        2-November-2021
 * @brief       电机数据更新
 */
void motor_update(motor_info_t *motor_info,uint8_t *rxBuf)
{
	motor_info->angle = (rxBuf[0]<<8) | rxBuf[1];
	motor_info->speed = (rxBuf[2]<<8) | rxBuf[3];
	motor_info->current = (rxBuf[4]<<8) | rxBuf[5];
	motor_info->tempreture = rxBuf[6];
	motor_info->offline_cnt = 0;
	if(motor_info->work_state == DEV_OFFLINE)
		motor_info->last_angle = motor_info->angle;
}


/**
 * @date        3-November-2021
 * @brief       角度限位
 */
void angle_constrain(float *angle)
{
	if(*angle>180)
		*angle -= 360;
	else if(*angle<=-180)
		*angle += 360;
}


/**
 * @date        3-November-2021
 * @brief       电机心跳
 */
void motor_heart_beat(motor_info_t *motor_info)
{
	motor_info->offline_cnt++;
	if(motor_info->offline_cnt > motor_info->offline_max_cnt)
	{
		motor_info->offline_cnt = motor_info->offline_max_cnt;
		motor_info->work_state = DEV_OFFLINE;
	}
	else if(motor_info->work_state == DEV_OFFLINE)
		motor_info->work_state = DEV_ONLINE;
}
