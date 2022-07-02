/**
 * @file        lifting_control.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        8-December-2021
 * @brief       抬升控制
 */
#include "control.h"
#include "lifting_control.h"
#include "cabin.h"
#include "cmsis_os.h"

lifting_control_t lifting_control = {
	.target = 0,
	.measure = 0,
	.state_pneu = OFF,
	.reset = RESET_NOT_OK,
};

/**
 * @date        8-December-2021
 * @brief       抬升模块任务
 */
void Lifting_Task(void const * argument)
{
  for(;;)
  {
		if(IS_SYSTEM_RESET)
			general_lifting_control();
		lifting_check();
		lifting_control.measure = -lifting.motor[LIFTING_MOTOR_L].info.total_angle;	
    osDelay(1);
  }	
}


/**
 * @date        8-December-2021
 * @brief       抬升总控
 */
void general_lifting_control()
{
	/*限值*/
	lifting_control.target = constrain(lifting_control.target,0,MAX_LIFT);
	/*设目标值*/
	lifting.motor[LIFTING_MOTOR_L].position_pid.target = -lifting_control.target;
	lifting.motor[LIFTING_MOTOR_R].position_pid.target = lifting_control.target;
	lifting.pneu->state = lifting_control.state_pneu;
}

 /**
 * @date        8-December-2021
 * @brief       抬升复位
 */
uint32_t lifting_reset_time;
reset_state_t lifting_reset()
{
	/*检测状态与电机状态*/
	if(IS_MOTORx_ONLINE(lifting.motor[LIFTING_MOTOR_L])&&IS_MOTORx_ONLINE(lifting.motor[LIFTING_MOTOR_R])&&!lifting_control.reset)
	{
		lifting_control.state_pneu = OFF;
		/*检测堵转时应该用更小的输出参数*/
		lifting.pneu->init(lifting.pneu);
		lifting.motor[LIFTING_MOTOR_L].position_pid.out_max = 1000;
		lifting.motor[LIFTING_MOTOR_L].speed_pid.out_max = 400;
		lifting.motor[LIFTING_MOTOR_L].speed_pid.max_abs = 0;
		lifting.motor[LIFTING_MOTOR_L].position_pid.target = -60000;
		lifting.motor[LIFTING_MOTOR_R].position_pid.out_max = 1000;
		lifting.motor[LIFTING_MOTOR_R].speed_pid.out_max = 400;
		lifting.motor[LIFTING_MOTOR_R].speed_pid.max_abs = 0;
		lifting.motor[LIFTING_MOTOR_R].position_pid.target = 60000;	
		
		cabin.motor[CABIN_MOTOR_L].speed_pid.target = -1000;
		cabin.motor[CABIN_MOTOR_R].speed_pid.target = 1000;
		
		
		/*检测堵转*/
		if(distance(lifting.motor[LIFTING_MOTOR_L].info.last_angle,lifting.motor[LIFTING_MOTOR_L].info.angle)<115\
		&&distance(lifting.motor[LIFTING_MOTOR_R].info.last_angle,lifting.motor[LIFTING_MOTOR_R].info.angle)<115)
			lifting_reset_time++;
		else
			lifting_reset_time = 0;		
		if(lifting_reset_time>1000)
		{
			/*复位成功后恢复输出参数并返回*/
			lifting.motor[LIFTING_MOTOR_L].position_pid.out_max = 4000;
			lifting.motor[LIFTING_MOTOR_L].speed_pid.out_max = 10000-1000;
			lifting.motor[LIFTING_MOTOR_L].speed_pid.max_abs = -2000;
			lifting.motor[LIFTING_MOTOR_R].position_pid.out_max = 4000;
			lifting.motor[LIFTING_MOTOR_R].speed_pid.max_abs = 2000;
			lifting.motor[LIFTING_MOTOR_R].speed_pid.out_max = 10000-1000;
			
			lifting.motor[LIFTING_MOTOR_L].info.total_angle = 250;
			lifting.motor[LIFTING_MOTOR_L].position_pid.target = -10000;
			lifting.motor[LIFTING_MOTOR_R].info.total_angle = -250;
			lifting.motor[LIFTING_MOTOR_R].position_pid.target = 10000;	


			cabin.motor[CABIN_MOTOR_L].speed_pid.target = 1000;
			cabin.motor[CABIN_MOTOR_R].speed_pid.target = -1000;
			lifting_control.target = 10000;
			
			lifting_reset_time = 0;
			return RESET_OK;
		}
		return RESET_NOT_OK;
	}
	return lifting_control.reset;
}


/**
 * @date        14-January-2021
 * @brief       抬升初始化
 */
void lifting_init()
{
	lifting_control.target = 0;
	lifting_control.state_pneu = OFF;
}
