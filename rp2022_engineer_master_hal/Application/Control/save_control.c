/**
 * @file        save_control.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        8-December-2021
 * @brief       ��Ԯ����
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
 * @brief       ��Ԯģ������
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
 * @brief       ��Ԯ�ܿ�
 */
void general_save_control()
{
	/*��ֵ*/
	save_control.target = constrain(save_control.target,0,MAX_SAVE);
	/*��Ŀ��ֵ*/
	save.motor[SAVE_CARD_MOTOR].position_pid.target = save_control.target;
}

 /**
 * @date        8-December-2021
 * @brief       ��Ԯ��λ
 */
uint32_t save_reset_time;
reset_state_t save_reset()
{
	if(save.motor[SAVE_CARD_MOTOR].info.work_state==DEV_ONLINE&&!save_control.reset)
	{
		/*����תʱӦ���ø�С���������*/
		save.motor[SAVE_CARD_MOTOR].position_pid.out_max = 2000;
		save.motor[SAVE_CARD_MOTOR].speed_pid.out_max = 3000;
		save.motor[SAVE_CARD_MOTOR].position_pid.target = -600000;		
		
		
		/*����ת*/
		if(distance(save.motor[SAVE_CARD_MOTOR].info.last_angle,save.motor[SAVE_CARD_MOTOR].info.angle)<115)
			save_reset_time++;
		else
			save_reset_time = 0;		
		if(save_reset_time>1000)
		{		
			/*��λ�ɹ���ָ��������������*/
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
 * @brief       ��Ԯ��ʼ��
 */
void save_init()
{
	save_control.target = 0;
}

