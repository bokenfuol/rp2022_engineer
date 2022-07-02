/**
 * @file        clamp_control.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        8-December-2021
 * @brief       夹爪控制
 */
#include "control.h"
#include "clamp_control.h"
#include "rc_sensor.h"
#include "cmsis_os.h"


clamp_control_t clamp_control = {
	.target = 0,
	.measure = 0,
	.state_keylight = OFF,
	.state_pneu = OFF,
	.reset = RESET_NOT_OK,
};

/**
 * @date        8-December-2021
 * @brief       夹爪模块任务
 */
void Clamp_Task(void const * argument)
{
  for(;;)
  {
		if(IS_SYSTEM_RESET)
			general_clamp_control();
		clamp_check();
		clamp_control.measure = clamp.motor[CLAMP_MOTOR_LEFT].info.total_angle;
		clamp_control.state_keylight = clamp.keylight->state;
    osDelay(1);
  }	
}


/**
 * @date        8-December-2021
 * @brief       夹爪总控
 */
void general_clamp_control()
{
	/*限值*/
	clamp_control.target = constrain(clamp_control.target,0,MAX_CLAMP);
	/*设目标值*/
	clamp.motor[CLAMP_MOTOR_LEFT].position_pid.target = clamp_control.target;
	clamp.pneu->state = clamp_control.state_pneu;

}


 /**
 * @date        8-December-2021
 * @brief       夹爪复位
 */
uint32_t clamp_reset_time;
reset_state_t clamp_reset()
{
	/*检测状态与电机状态*/
	if(IS_MOTORx_ONLINE(clamp.motor[CLAMP_MOTOR_LEFT])&&IS_MOTORx_ONLINE(clamp.motor[CLAMP_MOTOR_RIGHT])&&!clamp_control.reset)
	{
		clamp_control.state_pneu = OFF;
		/*检测堵转时应该用更小的输出参数*/
		clamp.motor[CLAMP_MOTOR_LEFT].position_pid.out_max = 1000;
		clamp.motor[CLAMP_MOTOR_LEFT].speed_pid.out_max = 5000;
		clamp.motor[CLAMP_MOTOR_LEFT].position_pid.target = -60000;
			
		
		/*先检测堵转*/
		if(distance(clamp.motor[CLAMP_MOTOR_LEFT].info.last_angle,clamp.motor[CLAMP_MOTOR_LEFT].info.angle)<115)
			clamp_reset_time++;
		else
			clamp_reset_time = 0;	
		
		if(clamp_reset_time>1000)
		{
			/*复位成功后恢复输出参数并返回*/
			clamp.motor[CLAMP_MOTOR_LEFT].position_pid.out_max = 1500;
			clamp.motor[CLAMP_MOTOR_LEFT].speed_pid.out_max = 10000;
			
			clamp.motor[CLAMP_MOTOR_LEFT].info.total_angle = 0;
			clamp.motor[CLAMP_MOTOR_LEFT].position_pid.target = MAX_CLAMP/2;	
			clamp_control.target = MAX_CLAMP/2;	
			
			clamp_reset_time = 0;
			return RESET_OK;
		}	
		return RESET_NOT_OK;
	}
	return clamp_control.reset;
}


 /**
 * @date        14-January-2021
 * @brief       夹爪初始化
 */
void clamp_init()
{
	clamp_control.state_pneu = OFF;
	clamp_control.target = MAX_CLAMP;
}


