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
#include "imu_sensor.h"
#include "rc_sensor.h"
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
	imu_sensor.heart_beat(&imu_sensor);
	rc_sensor.heart_beat(&rc_sensor);
	
	/*�����������������*/
	for(uint16_t i=0;i<DEVICE_CNT;i++)
		motor.heart_beat(device[i].motor);
	
	
}

