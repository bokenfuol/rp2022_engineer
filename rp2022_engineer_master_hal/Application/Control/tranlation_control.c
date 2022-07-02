/**
 * @file        tranlation_control.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        8-December-2021
 * @brief       ���ƿ���
 */
#include "control.h"
#include "tranlation_control.h"
#include "rc_sensor.h"
#include "cmsis_os.h"

tranlation_control_t tranlation_control = {
	.target = 0,
	.measure = 0,
	.reset = RESET_NOT_OK,
};

/**
 * @date        8-December-2021
 * @brief       ����ģ������
 */
void Tranlation_Task(void const * argument)
{
  for(;;)
  {
		if(IS_SYSTEM_RESET)
			general_tranlation_control();
		tranlation_check();
		tranlation_control.measure = tranlation.motor[TRANLATION_MOTOR].info.total_angle;
    osDelay(1);
  }	
}


/**
 * @date        8-December-2021
 * @brief       �����ܿ�
 */
void general_tranlation_control()
{
	/*��ֵ*/
	tranlation_control.target = constrain(tranlation_control.target,0,MAX_TRANLASTION);
	/*��Ŀ��ֵ*/
	tranlation.motor[TRANLATION_MOTOR].position_pid.target = tranlation_control.target;
}

 /**
 * @date        8-December-2021
 * @brief       ���Ƹ�λ
 */
uint32_t tranlation_reset_time;
reset_state_t tranlation_reset()
{
	/*���״̬����״̬*/
	if(IS_MOTORx_ONLINE(tranlation.motor[TRANLATION_MOTOR])&&!tranlation_control.reset)
	{
		/*����תʱӦ���ø�С���������*/
		tranlation.motor[TRANLATION_MOTOR].position_pid.out_max = 1500;
		tranlation.motor[TRANLATION_MOTOR].speed_pid.out_max = 6000;
		tranlation.motor[TRANLATION_MOTOR].position_pid.target = -700000;		
		/*�ȼ���ת*/
		if(distance(tranlation.motor[TRANLATION_MOTOR].info.last_angle,tranlation.motor[TRANLATION_MOTOR].info.angle)<300)
			tranlation_reset_time++;
		else
			tranlation_reset_time = 0;		
		if(tranlation_reset_time>1000)
		{
			/*��λ�ɹ���ָ��������������*/
			tranlation.motor[TRANLATION_MOTOR].position_pid.out_max = 3000;
			tranlation.motor[TRANLATION_MOTOR].speed_pid.out_max = 10000;
			
			tranlation.motor[TRANLATION_MOTOR].info.total_angle = 0;
			tranlation.motor[TRANLATION_MOTOR].position_pid.target = MID_TRANLASION;
			tranlation_control.target =  MID_TRANLASION;
			
			tranlation_reset_time = 0;
			return RESET_OK;	
		}		
		return RESET_NOT_OK;	
	}
	return tranlation_control.reset;
	
}


 /**
 * @date        14-January-2021
 * @brief       ���Ƴ�ʼ��
 */
void tranlation_init()
{
	tranlation_control.target =  MID_TRANLASION;
}



