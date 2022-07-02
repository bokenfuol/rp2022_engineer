/**
 * @file        heart_beat.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        3-November-2021
 * @brief       心跳
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
 * @brief       心跳任务
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
 * @brief       心跳主控
 */
void heart_beat()
{
	/*喂狗*/
	HAL_IWDG_Refresh(&hiwdg);
	
	//主管心跳
	imu_sensor.heart_beat(&imu_sensor);
	rc_sensor.heart_beat(&rc_sensor);
	
	/*遍历电机、产生心跳*/
	for(uint16_t i=0;i<DEVICE_CNT;i++)
		motor.heart_beat(device[i].motor);
	
	
}

