/**
 * @file        control.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        3-November-2021
 * @brief       ����
 */
#include "control.h"
#include "mode_switch.h"
#include "chassis_control.h"
#include "rc_sensor.h"
#include "vision_sensor.h"
#include "led.h"
#include "cmsis_os.h"




/*ϵͳ��־�ṹ��*/
control_struct control = {
	.master_reset = RESET_NOT_OK,
	.slaver_reset = RESET_NOT_OK,
	.system_mode = NORMAL_MODE,
	.vision_state = OFF,
};


/**
 * @date        29-November-2021
 * @brief       �ܿ�����
*/
void Control_Task(void const * argument)
{
	for(;;)
  {
		general_control();
    osDelay(1);
  }
}



/**
 * @date        3-November-2021
 * @brief       �ܿ�
 */
void general_control()
{
	/*��λ*/
	general_reset();
	/*ң�������ݴ���*/
	rc_sensor.check(&rc_sensor,&rc_control);
	/*ϵͳָʾ��*/
	system_led();
	/*�Ӿ����ݴ���*/
	vision_sensor.ckeck(&vision_sensor.rx);
}

/**
 * @date        6-November-2021
 * @brief       ��λ
 */
void general_reset()
{
	if(!IS_SYSTEM_RESET&&IS_RC_ONLINE)
	{
		chassis_control.reset = chassis_reset();
		if(chassis_control.reset)
#if slave_mode
			control.slaver_reset = RESET_OK;
#else
			control.master_reset = RESET_OK;
#endif
	}
	else if(!IS_RC_ONLINE)
	{
		chassis_motor[CHAS_BL].power_motor.speed_pid.target = 0;
		chassis_motor[CHAS_BR].power_motor.speed_pid.target = 0;
		chassis_motor[CHAS_FL].power_motor.speed_pid.target = 0;
		chassis_motor[CHAS_FR].power_motor.speed_pid.target = 0;
		control.system_mode = AIR_MODE;
		control.slaver_reset = RESET_NOT_OK;
		mode_switch.mode = NORMAL_MODE;
		mode_switch.step = 1;
	}
	
}

	
