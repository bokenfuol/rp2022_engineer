/**
 * @file        heart_beat.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        3-November-2021
 * @brief       ����
 */

#include "control.h"
#include "heart_beat.h"
#include "device.h"
#include "rc_sensor.h"
#include "judge_protocol.h"
#include "vision_sensor.h"
#include "iwdg.h"
#include "cmsis_os.h"

/**
 * @date        29-November-2021
 * @brief       ��������
 */
void Heart_Beat_Task(void const * argument)
{
	 for(;;)
  {	
		/*ι��*/
		HAL_IWDG_Refresh(&hiwdg);
		heart_beat();
    osDelay(1);
  }
}


/**
 * @date        2-November-2021
 * @brief       ��������
 */
void heart_beat()
{
	/*ι��*/
	HAL_IWDG_Refresh(&hiwdg);
	
	//��������
	rc_sensor.heart_beat(&rc_sensor);
	
	vision_sensor.heart_beat(&vision_sensor.rx);
	
	check_judge_offline(&judge);
	
	/*�����������������*/
	for(uint16_t i=0;i<DEVICE_CNT;i++)
		motor.heart_beat(device[i].motor);
}

