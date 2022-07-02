/**
 * @file        save_control.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        8-December-2021
 * @brief       救援控制
 */
#include "control.h"
#include "save_control.h"
#include "cmsis_os.h"

save_control_t save_control = {
	.target = 0,
	.measure = 0,
	.reset = RESET_NOT_OK,
};

/**
 * @date        8-December-2021
 * @brief       救援模块任务
 */
void Save_Task(void const * argument)
{
  for(;;)
  {

		if(control.master_reset == RESET_OK)
			general_save_control();
		save_check();
		save_control.measure = save.motor[SAVE_CARD_MOTOR].info.total_angle;
    osDelay(1);
  }	
}


/**
 * @date        8-December-2021
 * @brief       救援总控
 */
void general_save_control()
{
	/*限值*/
	save_control.target = constrain(save_control.target,0,MAX_SAVE);
	/*设目标值*/
	save.motor[SAVE_CARD_MOTOR].position_pid.target = save_control.target;
}

 /**
 * @date        8-December-2021
 * @brief       救援复位
 */
uint32_t save_reset_time;
reset_state_t save_reset()
{
	if(save.motor[SAVE_CARD_MOTOR].info.work_state==DEV_ONLINE&&!save_control.reset)
	{
		/*检测堵转时应该用更小的输出参数*/
		save.motor[SAVE_CARD_MOTOR].position_pid.out_max = 2000;
		save.motor[SAVE_CARD_MOTOR].speed_pid.out_max = 3000;
		save.motor[SAVE_CARD_MOTOR].position_pid.target = -600000;		
		
		
		/*检测堵转*/
		if(distance(save.motor[SAVE_CARD_MOTOR].info.last_angle,save.motor[SAVE_CARD_MOTOR].info.angle)<115)
			save_reset_time++;
		else
			save_reset_time = 0;		
		if(save_reset_time>1000)
		{		
			/*复位成功后恢复输出参数并返回*/
			save_reset_time = 0;
			save.motor[SAVE_CARD_MOTOR].position_pid.out_max = 6000;
			save.motor[SAVE_CARD_MOTOR].speed_pid.out_max = 3000;
			
			save.motor[SAVE_CARD_MOTOR].info.total_angle = -10;
			save.motor[SAVE_CARD_MOTOR].position_pid.target = 0;
			save_reset_time = 0;
			return RESET_OK;
		}
		return RESET_NOT_OK;	
	}
	return save_control.reset;
}


/**
 * @date        14-January-2021
 * @brief       救援初始化
 */
void save_init()
{
	save_control.target = 0;
}

