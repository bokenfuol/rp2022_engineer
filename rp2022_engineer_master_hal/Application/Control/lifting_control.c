/**
 * @file        lifting_control.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        8-December-2021
 * @brief       ̧������
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
 * @brief       ̧��ģ������
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
 * @brief       ̧���ܿ�
 */
void general_lifting_control()
{
	/*��ֵ*/
	lifting_control.target = constrain(lifting_control.target,0,MAX_LIFT);
	/*��Ŀ��ֵ*/
	lifting.motor[LIFTING_MOTOR_L].position_pid.target = -lifting_control.target;
	lifting.motor[LIFTING_MOTOR_R].position_pid.target = lifting_control.target;
	lifting.pneu->state = lifting_control.state_pneu;
}

 /**
 * @date        8-December-2021
 * @brief       ̧����λ
 */
uint32_t lifting_reset_time;
reset_state_t lifting_reset()
{
	/*���״̬����״̬*/
	if(IS_MOTORx_ONLINE(lifting.motor[LIFTING_MOTOR_L])&&IS_MOTORx_ONLINE(lifting.motor[LIFTING_MOTOR_R])&&!lifting_control.reset)
	{
		lifting_control.state_pneu = OFF;
		/*����תʱӦ���ø�С���������*/
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
		
		
		/*����ת*/
		if(distance(lifting.motor[LIFTING_MOTOR_L].info.last_angle,lifting.motor[LIFTING_MOTOR_L].info.angle)<115\
		&&distance(lifting.motor[LIFTING_MOTOR_R].info.last_angle,lifting.motor[LIFTING_MOTOR_R].info.angle)<115)
			lifting_reset_time++;
		else
			lifting_reset_time = 0;		
		if(lifting_reset_time>1000)
		{
			/*��λ�ɹ���ָ��������������*/
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
 * @brief       ̧����ʼ��
 */
void lifting_init()
{
	lifting_control.target = 0;
	lifting_control.state_pneu = OFF;
}
