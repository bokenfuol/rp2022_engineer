/**
 * @file        ground_control.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        8-December-2021
 * @brief       �ؿ����
 */
#include "control.h"
#include "ground_control.h"
#include "cmsis_os.h"

ground_control_t ground_control = {
	.target_pitch = 0,
	.target_tranlation = 0,
	.measure_pitch = 0,
	.measure_tranlation = 0,
	.reset = RESET_NOT_OK,
};

/**
 * @date        8-December-2021
 * @brief       �ؿ�ģ������
 */
void Ground_Task(void const * argument)
{
  for(;;)
  {
		if(control.master_reset == RESET_OK)
			general_ground_control();
		ground_check();
		ground_control.measure_pitch = ground.motor[GROUND_MOTOR_L].info.total_angle;
		ground_control.measure_tranlation = -ground.motor[GROUND_MOTOR_T].info.total_angle;
    osDelay(1);
  }	
}


/**
 * @date        8-December-2021
 * @brief       �ؿ��ܿ�
 */
void general_ground_control()
{
	/*��ֵ*/
	ground_control.target_pitch = constrain(ground_control.target_pitch,0,MAX_GROUND_PITCH);
	ground_control.target_tranlation = constrain(ground_control.target_tranlation,0,MAX_GROUND_TRANLATION);
	/*��Ŀ��ֵ*/
	ground.motor[GROUND_MOTOR_L].position_pid.target = ground_control.target_pitch;
	ground.motor[GROUND_MOTOR_T].position_pid.target = -ground_control.target_tranlation;
}

 /**
 * @date        14-January-2021
 * @brief       �ؿ�װ�ø�λ
 */
uint32_t ground_reset_time=0,ground_tranlstion_reset=0;
reset_state_t ground_reset()
{
	/*���״̬����״̬*/
	if(IS_MOTORx_ONLINE(ground.motor[GROUND_MOTOR_L])&&IS_MOTORx_ONLINE(ground.motor[GROUND_MOTOR_R])
	&&IS_MOTORx_ONLINE(ground.motor[GROUND_MOTOR_T])&&!ground_control.reset)
	{
		
		/*����תʱӦ���ø�С���������*/
		ground.motor[GROUND_MOTOR_L].position_pid.out_max = 1000;
		ground.motor[GROUND_MOTOR_L].speed_pid.out_max = 3000;
		

		ground.motor[GROUND_MOTOR_T].position_pid.out_max = 2000;
		ground.motor[GROUND_MOTOR_T].speed_pid.out_max = 3500;
		
		
		if(!ground_tranlstion_reset)
		{
			ground.motor[GROUND_MOTOR_T].position_pid.target = 60000;
			/*�ȼ���ת*/
			if(distance(ground.motor[GROUND_MOTOR_T].info.last_angle,ground.motor[GROUND_MOTOR_T].info.angle)<115)
				ground_reset_time++;
			else
				ground_reset_time = 0;	
			
			if(ground_reset_time>500)
			{
				ground.motor[GROUND_MOTOR_T].info.total_angle = 0;
				ground.motor[GROUND_MOTOR_T].position_pid.target = 0;	
				
				ground.motor[GROUND_MOTOR_L].position_pid.target = -60000;
				
				ground_tranlstion_reset = 1;
				ground_reset_time = 0;	
			}
		}
		else
		{
			if(distance(ground.motor[GROUND_MOTOR_L].info.last_angle,ground.motor[GROUND_MOTOR_L].info.angle)<115)
				ground_reset_time++;	
			else
				ground_reset_time = 0;	
			
			if(ground_reset_time>1000)
			{
				/*��λ�ɹ���ָ��������������*/
				ground.motor[GROUND_MOTOR_L].position_pid.out_max = 2000;
				ground.motor[GROUND_MOTOR_L].speed_pid.out_max = 12000;
				ground.motor[GROUND_MOTOR_R].position_pid.out_max = 2000;
				ground.motor[GROUND_MOTOR_R].speed_pid.out_max = 12000;
				ground.motor[GROUND_MOTOR_T].position_pid.out_max = 3000;
				ground.motor[GROUND_MOTOR_T].speed_pid.out_max = 3500;
				
				ground.motor[GROUND_MOTOR_L].info.total_angle = 0;
				ground.motor[GROUND_MOTOR_L].position_pid.target = 0;	
		
				ground_tranlstion_reset = 0;
				ground_reset_time = 0;
				return RESET_OK;
			}
		}
		return RESET_NOT_OK;				
	}
	return ground_control.reset;
}

 /**
 * @date        14-January-2021
 * @brief       �ؿ�װ�ó�ʼ��
 */
void ground_init()
{
	ground_control.target_tranlation = 0;
	if(distance(ground_control.measure_tranlation,0)<115)
		ground_control.target_pitch = 0;
}

