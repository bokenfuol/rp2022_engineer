/**
 * @file        cabin_control.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        8-December-2021
 * @brief       ������
 */
#include "control.h"
#include "cabin_control.h"
#include "cmsis_os.h"
#include "keylight.h"

cabin_control_t cabin_control = {
	.target_L = 0,
	.target_R = 0,
	.measure_L = 0,
	.measure_R = 0,
	.state_keylight_up = OFF,
	.state_keylight_down = OFF,
	.reset = RESET_NOT_OK,
};

/**
 * @date        8-December-2021
 * @brief       ���ģ������
 */
void Mine_Task(void const * argument)
{
  for(;;)
  {
		if(control.master_reset == RESET_OK)
			general_cabin_control();
		cabin_check();
		cabin_control.measure_L = cabin.motor[CABIN_MOTOR_L].info.speed;
		cabin_control.measure_R = cabin.motor[CABIN_MOTOR_R].info.speed;
		cabin_control.state_keylight_up = cabin.keylight_up->state;
		cabin_control.state_keylight_down = cabin.keylight_down->state;	
    osDelay(1);
  }	
}


/**
 * @date        8-December-2021
 * @brief       ����ܿ�
 */
void general_cabin_control()
{
	/*��Ŀ��ֵ*/
	cabin.motor[CABIN_MOTOR_L].speed_pid.target = cabin_control.target_L;
	cabin.motor[CABIN_MOTOR_R].speed_pid.target = cabin_control.target_R;
}


 /**
 * @date        14-January-2021
 * @brief       ��ָ�λ
 */
reset_state_t cabin_reset()
{
	cabin.motor[CABIN_MOTOR_L].speed_pid.target = 0;
	cabin.motor[CABIN_MOTOR_R].speed_pid.target = 0;
	cabin_control.target_L = 0;
	cabin_control.target_R = 0;
	return RESET_OK;
}

 /**
 * @date        14-January-2021
 * @brief       ��ֳ�ʼ��
 */
void cabin_init()
{
	cabin_control.target_L = 0;
	cabin_control.target_R = 0;
}

